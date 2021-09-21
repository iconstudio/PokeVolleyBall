#include "pch.h"
#include "Framework.h"
#include "Core.h"


GameFramework::GameFramework()
	: mouse_x(0), mouse_y(0), delta_time(0.0), state_id(nullptr), painter{}, elapsed(0) {}

GameFramework::~GameFramework() {
	for (auto& sprite : sprites)
		sprite.reset();

	sprites.clear();
}

void GameFramework::input_register(const WPARAM virtual_button) {
	key_checkers.emplace(virtual_button, make_shared<GameInput>());
}

bool GameFramework::input_check(const WPARAM virtual_button) {
	auto checker = key_checkers.find(virtual_button);
	if (checker != key_checkers.end()) {
		return checker->second->pressing;
	}

	return false;
}

void GameFramework::init() {
	state_clear();

	input_register(VB_LEFT);
	input_register(VB_MIDDLE);
	input_register(VB_RIGHT);
	input_register(VK_ESCAPE);
}

void GameFramework::build() {
	if (state_remains())
		state_jump(0);
	delta_start();
}

bool GameFramework::update() {
	if (state_is_done()) {
		return false;
	}
	
	delta_time = get_elapsed_second();
	on_update(delta_time);
	on_update_later(delta_time);

	return true;
}

void GameFramework::quit() {
	state_clear();
}

void GameFramework::delta_start() {
	clock_previos = std::chrono::system_clock::now();
}

void GameFramework::delta_inspect() {
	clock_now = std::chrono::system_clock::now();

	elapsed = std::chrono::duration_cast<tick_type>(clock_now - clock_previos).count();
}

double GameFramework::get_elapsed_second() const {
	return ((double)elapsed / (double)tick_type::period::den);
}

size_t GameFramework::make_sprite(HINSTANCE instance, const UINT resource, const UINT number, const int xoff, const int yoff) {
	size_t loc = sprites.size();
	sprites.emplace_back(make_shared<GameSprite>(instance, resource, number, xoff, yoff));

	return loc;
}

size_t GameFramework::make_sprite(const LPCTSTR path, const UINT number, const int xoff, const int yoff) {
	size_t loc = sprites.size();
	sprites.emplace_back(make_shared<GameSprite>(path, number, xoff, yoff));

	return loc;
}

shared_ptr<GameSprite> GameFramework::find_sprite(const size_t index) {
	return sprites.at(index);
}

bool GameFramework::state_is_done() const {
	if (state_id)
		return state_id->done;
	else
		return true;
}

void GameFramework::state_clear() {
	state_id = nullptr;
	state_handle = 0;
	states.clear();
}

bool GameFramework::state_remains() {
	return (!states.empty() && state_id != states.back());
}

void GameFramework::state_jump(const INT index) {
	auto target = states.at(index);

	if (target && target != state_id) {
		target->done = false;
		state_id = target;
		state_handle = index;
	}
}

void GameFramework::state_jump_next() {
	if (state_remains()) {
		state_jump(state_handle + 1);
	}
}

void GameFramework::on_create() {
	if (state_id) {
		state_id->on_create();
	}
}

void GameFramework::on_destroy() {
	if (state_id) {
		state_id->on_destroy();
	}
}

void GameFramework::on_update(const double frame_advance) {
	if (state_id) {
		state_id->on_update(frame_advance);
	}
}

void GameFramework::on_update_later(const double frame_advance) {
	if (state_id) {
		state_id->on_update_later(frame_advance);
	}
}

void GameFramework::on_mousedown(const WPARAM button, const LPARAM cursor) {
	auto vk_status = key_checkers[button];
	vk_status->pressing = true;

	mouse_x = LOWORD(cursor);
	mouse_y = HIWORD(cursor);
}

void GameFramework::on_mouseup(const WPARAM button, const LPARAM cursor) {
	auto vk_status = key_checkers[button];
	vk_status->pressing = false;

	mouse_x = LOWORD(cursor);
	mouse_y = HIWORD(cursor);
}

void GameFramework::on_keydown(const WPARAM key) {
	auto vk_status = key_checkers.find(key);
	if (vk_status != key_checkers.end()) {
		vk_status->second->pressing = true;
	}
}

void GameFramework::on_keyup(const WPARAM key) {
	auto vk_status = key_checkers.find(key);
	if (vk_status != key_checkers.end()) {
		vk_status->second->pressing = false;
	}
}

void GameFramework::on_render(HWND hwnd) {
	HDC surface_app = BeginPaint(hwnd, &painter); // 앱 버퍼
	HDC surface_double = CreateCompatibleDC(surface_app);

	// 메인 DC 그리기 부분
	auto m_hBit = CreateCompatibleBitmap(surface_app, RESOLUTION_W, RESOLUTION_H);
	auto m_oldhBit = (HBITMAP)SelectObject(surface_double, m_hBit);

	// 초기화
	auto m_hPen = CreatePen(PS_NULL, 1, COLOR_WHITE);
	auto m_oldhPen = (HPEN)SelectObject(surface_double, m_hPen);
	auto m_hBR = CreateSolidBrush(COLOR_WHITE);
	auto m_oldhBR = (HBRUSH)SelectObject(surface_double, m_hBR);
	Render::draw_rectangle(surface_double, 0, 0, RESOLUTION_W, RESOLUTION_H);
	Render::draw_end(surface_double, m_oldhBR, m_hBR);
	Render::draw_end(surface_double, m_oldhPen, m_hPen);

	HDC surface_back = CreateCompatibleDC(surface_app);
	HBITMAP m_newBit = CreateCompatibleBitmap(surface_app, RESOLUTION_W, RESOLUTION_H);
	HBITMAP m_newoldBit = (HBITMAP)SelectObject(surface_back, m_newBit);

	// 파이프라인
	if (state_id) {
		state_id->on_render(surface_double);
	}

	// 이중 버퍼 -> 백 버퍼
	BitBlt(surface_back, 0, 0, RESOLUTION_W, RESOLUTION_H, surface_double, 0, 0, SRCCOPY);
	Render::draw_end(surface_double, m_oldhBit, m_hBit);

	// 백 버퍼 -> 화면 버퍼
	BitBlt(surface_app, 0, 0, RESOLUTION_W, RESOLUTION_H, surface_back, 0, 0, SRCCOPY);
	Render::draw_end(surface_back, m_newoldBit, m_newBit);

	DeleteDC(surface_back);
	DeleteDC(surface_double);
	ReleaseDC(hwnd, surface_app);
	EndPaint(hwnd, &painter);
}

void Render::refresh(HWND hwnd) {
	InvalidateRect(hwnd, NULL, FALSE);
}

void Render::transform_set(HDC world, XFORM& info) {
	SetWorldTransform(world, &info);
}

void Render::transform_set_identity(HDC world) {
	SetWorldTransform(world, &transform_identity);
}

void Render::draw_end(HDC handle, HGDIOBJ object_old, HGDIOBJ object_default) {
	SelectObject(handle, object_old);
	DeleteObject(object_default);
}

BOOL Render::draw_rectangle(HDC hDC, int x1, int y1, int x2, int y2) {
	return Rectangle(hDC, x1, y1, x2, y2);
}
