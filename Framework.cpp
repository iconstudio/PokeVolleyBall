#include "pch.h"
#include "Framework.h"
#include "Core.h"


GameFramework::GameFramework() : mouse_x(0), mouse_y(0), delta_time(0.0), state_id(nullptr), painter{} {}

GameFramework::~GameFramework() {}

void GameFramework::input_register(int button) {
	key_checkers.emplace(button, make_shared<GameInput>());
}

bool GameFramework::input_check(int button) {
	var checker = key_checkers.find(button);
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

void GameFramework::build() {}

bool GameFramework::update() {
	return false;
}

void GameFramework::quit() {}

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
	return (!states.empty() && state_handle < states.size() - 1);
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

void GameFramework::on_update(double frame_advance) {
	if (state_id) {
		state_id->on_update(frame_advance);
	}
}

void GameFramework::on_update_later(double frame_advance) {
	if (state_id) {
		state_id->on_update_later(frame_advance);
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

	Ellipse(surface_double, 80, 200, 150, 270);

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
