// PokeValleyBall.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include "pch.h"
#include "stdafx.h"
#include "PokeVolleyBall.h"
#include "Client.h"

#define MAX_LOADSTRING 100
#define RENDER_TIMER_ID 0


// 전역 변수:
WCHAR szTitle[MAX_LOADSTRING];								// 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];						// 기본 창 클래스 이름입니다.
WindowsClient game_client{ RESOLUTION_W, RESOLUTION_H };	// 클라이언트 객체입니다.
GameFramework game_framework;

auto SPRITE_LOGO = game_framework.make_sprite(TEXT("res\\kpu2x.png"), 1, 320, 240);
auto SPRITE_TITLE = game_framework.make_sprite(TEXT("res\\title.png"), 1, 160, 120);
auto SPRITE_BALL = game_framework.make_sprite(TEXT("res\\ball.png"), 1, 32, 32);
auto SPRITE_PIKA = game_framework.make_sprite(TEXT("res\\bigpikaboth_strip2.png"), 2, 40, 40);
auto SPRITE_PIKAWALK_L = game_framework.make_sprite(TEXT("res\\bigpikawalk_strip4.png"), 4, 40, 40);
auto SPRITE_PIKAWALK_R = game_framework.make_sprite(TEXT("res\\bigpikawalkright_strip4.png"), 4, 40, 40);

// 출처: https://twitter.com/yuukimokuya/status/1292847226784374785
auto SPRITE_BG = game_framework.make_sprite(TEXT("res\\beach.png"), 1, 600, 0);

auto ROOM_INTRO = game_framework.state_push<sceneIntro>();
auto ROOM_GAME = game_framework.state_push<sceneGame>();
auto ROOM_TITLE = game_framework.state_push<sceneTitle>();
auto ROOM_MAIN = game_framework.state_push<sceneMainMenu>();


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
					  _In_opt_ HINSTANCE hPrevInstance,
					  _In_ LPWSTR    lpCmdLine,
					  _In_ int       nCmdShow) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_POKEVALLEYBALL, szWindowClass, MAX_LOADSTRING);

	// 애플리케이션 초기화를 수행합니다:
	if (!game_client.initialize(hInstance, WndProc, szTitle, szWindowClass, nCmdShow)) {
		return FALSE;
	}

	// 단축키의 목록을 불러옵니다.
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_POKEVALLEYBALL));
	MSG msg{};

	// 게임 초기화
	game_framework.init();

	game_framework.input_register(VK_RETURN); // 확인
	game_framework.input_register(VK_SPACE); // 점프 및 확인

	game_framework.input_register(VK_LEFT);
	game_framework.input_register(VK_RIGHT);
	game_framework.input_register(VK_UP);
	game_framework.input_register(VK_DOWN);
	game_framework.input_register(VK_CHAR_Z); // Z
	game_framework.input_register(VK_CHAR_X); // X
	game_framework.input_register(VK_CHAR_C); // C

	game_framework.input_register(VK_CHAR_P);
	game_framework.input_register(VK_CHAR_R); // R - 게임 다시 시작 (장면이 sceneGame일때만 작동)
	game_framework.input_register(VK_F1); // 도움말
	game_framework.input_register(VK_F2); // 게임 전체 다시 시작
	game_framework.input_register(VK_F3); // 현재 장면만 다시 시작

	// 게임 빌드
	game_framework.build();

	// 메시지 루프:
	bool done = false;

	while (true) {
		while (!game_framework.state_is_done()) {
			if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				if (msg.message == WM_QUIT) {
					done = true;
					break;
				}

				if (!::TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}
			} else {
				
			}
		}

		// 현재 상태가 완료됨
		if (game_framework.state_remains()) {
			game_framework.state_jump_next();
		} else {
			//break;
		}

		if (done)
			break;
	}

	return (int)msg.wParam;
}

////////////////////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		// 창 생성
		case WM_CREATE:
		{
			SetTimer(hwnd, RENDER_TIMER_ID, 1, NULL);
		}
		break;

		// 렌더링 타이머
		case WM_TIMER:
		{
			game_framework.delta_inspect();
			Render::refresh(hwnd);
			game_framework.update();
			game_framework.delta_start();
		}
		break;

		// 마우스 왼쪽 누름
		case WM_LBUTTONDOWN:
		{
			game_framework.on_mousedown(VB_LEFT, lParam);
		}
		break;

		// 마우스 왼쪽 뗌
		case WM_LBUTTONUP:
		{
			game_framework.on_mouseup(VB_LEFT, lParam);
		}
		break;

		// 마우스 오른쪽 누름
		case WM_RBUTTONDOWN:
		{
			game_framework.on_mousedown(VB_RIGHT, lParam);
		}
		break;

		// 마우스 오른쪽 뗌
		case WM_RBUTTONUP:
		{
			game_framework.on_mouseup(VB_RIGHT, lParam);
		}
		break;

		// 마우스 휠 누름
		case WM_MBUTTONDOWN:
		{
			game_framework.on_mousedown(VB_MIDDLE, lParam);
		}
		break;

		// 마우스 휠 뗌
		case WM_MBUTTONUP:
		{
			game_framework.on_mouseup(VB_MIDDLE, lParam);
		}
		break;

		// 키보드 누름
		case WM_KEYDOWN:
		{
			if (wParam == VK_F1) {
				DialogBox(game_client.instance, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
				break;
			}
			
			game_framework.on_keydown(wParam);
		}
		break;

		// 키보드 뗌
		case WM_KEYUP:
		{
			game_framework.on_keyup(wParam);
		}
		break;

		// 렌더링
		case WM_PAINT:
		{
			game_framework.on_render(hwnd);
		}
		break;

		// 창 종료
		case WM_DESTROY:
		{
			KillTimer(hwnd, RENDER_TIMER_ID);
			PostQuitMessage(0);
		}
		break;

		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	switch (message) {
		case WM_INITDIALOG:
			return (INT_PTR)TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
			break;
	}
	return (INT_PTR)FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////

void sceneIntro::on_create() {
	sprite_index = game_framework.find_sprite(SPRITE_LOGO);
}

void sceneIntro::on_destroy() {
	sprite_index.reset();
}

void sceneIntro::on_update(double frame_advance) {
	state.update(frame_advance);

	auto phase = state.get_phase();
	auto ratio = state.get();

	switch (phase) {
		case 0: // 페이드 인
		{
			alpha = ratio;
		}
		break;

		case 1: // 그리기
		{
			bool check_enter = game_framework.input_check_pressed(VK_RETURN)
				|| game_framework.input_check_pressed(VK_SPACE);

			if (check_enter) {
				state.skip();
			}

			alpha = 1.0;
		}
		break;

		case 2: // 페이드 아웃
		{
			alpha = 1.0 - ratio;
		}
		break;

		default:
			break;
	}

	if (state.get_done()) {
		done = true;
	}
}

void sceneIntro::on_render(HDC canvas) {
	sprite_index->draw(canvas, x, y, 0, 0, 1, 1, alpha);
}

void sceneTitle::on_create() {
	sprite_index = game_framework.find_sprite(SPRITE_TITLE);
}

void sceneTitle::on_destroy() {
	sprite_index.reset();
}

void sceneTitle::on_update(double frame_advance) {
	bool check_enter = game_framework.input_check_pressed(VK_RETURN)
		|| game_framework.input_check_pressed(VK_SPACE);

}

void sceneTitle::on_render(HDC canvas) {
	sprite_index->draw(canvas, x, y, 0, 0, 1, 1, 1);
}

void sceneMainMenu::on_create() {}

void sceneMainMenu::on_destroy() {}

void sceneMainMenu::on_update(double frame_advance) {}

void sceneMainMenu::on_render(HDC canvas) {}

////////////////////////////////////////////////////////////////////////////////////////////

void sceneGame::on_create() {
	ball = instance_create<oVolleyBall>(BALL_PLAYER_X_START, BALL_PLAYER_Y_START);
	player = instance_create<oPlayerPoke>(PIKA_X_START_PLAYER, PIKA_Y_START);
	enemy = instance_create<oEnemyPoke>(PIKA_X_START_ENEMY, PIKA_Y_START);

	ready_done = false;
	ready_counter = 0.0;
	wining = false;
	turn = TURN::player;
	player->ball = ball;
	enemy->ball = ball;
	//persistent = true;

	GameScene::on_create();
}

void sceneGame::on_destroy() {
	GameScene::on_destroy();
}

void sceneGame::on_update(double frame_advance) {
	GameScene::on_update(frame_advance);
}

void sceneGame::on_update_later(double frame_advance) {
	GameScene::on_update_later(frame_advance);
}

void sceneGame::on_render(HDC canvas) {
	GameScene::on_render(canvas);
	//TextOut(canvas, 10, 10, TEXT("Paused"), 7);
}

// 플레이어
oPlayerPoke::oPlayerPoke(GameScene* nclan, double nx, double ny)
	: oPikachu(nclan, nx, ny) {
	auto sprite = game_framework.find_sprite(SPRITE_PIKA);
	sprite_init(sprite);
	look_at(LOOKDIR::RIGHT);
	set_xborder(PLAYER_X_MIN, PLAYER_X_MAX);
}

// 플레이어 동작
void oPlayerPoke::on_update(double frame_advance) {
	oPikachu::on_update(frame_advance);

	int check_left = game_framework.input_check(VK_LEFT);
	int check_right = game_framework.input_check(VK_RIGHT);
	int check_up = game_framework.input_check(VK_UP);
	int check_down = game_framework.input_check(VK_DOWN);

	bool check_jump = game_framework.input_check_pressed(VK_SPACE);
	bool check_smash = game_framework.input_check_pressed(VK_CHAR_Z);
	bool check_rolling = game_framework.input_check_pressed(VK_CHAR_X);
	bool check_blink = game_framework.input_check_pressed(VK_CHAR_C);

	if (!is_acting()) {
		int check_hor = (check_right - check_left);
		if (check_hor != 0 && can_move()) {
			walk(check_hor, frame_advance);
			look_at((LOOKDIR)check_hor);
		} else {
			hspeed *= 0.2;
		}
	}

	bool ask_jump = check_jump && can_jump();
	bool ask_smash = check_smash && can_smash();
	bool ask_roll = check_rolling && can_rollingforward();
	bool ask_blink = check_blink && can_blink();

	if (ask_blink) { // 전광석화
		blink();
	} else if (ask_roll) { // 앞구르기
		rollingforward();
	} else if (ask_jump) { // 점프
		jump(PIKA_JUMP_VELOCITY);
	}

	// 공과 충돌했을 때만 기술을 사용할 수 있다.
	auto collide_with_ball = collide_with(ball);
	if (collide_with_ball) {
		if (is_rolling()) {
			if (-BALL_HEADING_APGUREUGI_YVELOCITY < vspeed)
				ball->jump(BALL_HEADING_APGUREUGI_YVELOCITY);
		} else if (is_blinking()) {
			if (-BALL_HEADING_BLINK_YVELOCITY < vspeed)
				ball->jump(BALL_HEADING_BLINK_YVELOCITY);
		} else if (ask_smash) { // 스매시
			int dir = 0;

			bool smash_check_up = (y < ball->y);

			if (smash_check_up) {

			} else {

			}
		} else if (is_jumping() && vspeed < -BALL_HEADING_YVELOCITY) { // 세게 쳐올리기
			double rocketvel = -vspeed * 0.9;
			if (rocketvel < BALL_HEADING_JUMP_YVELOCITY)
				ball->jump(BALL_HEADING_JUMP_YVELOCITY);
			else
				ball->jump(rocketvel);
		} else { // 공을 쳐올리기
			//if (y < ball->y) ball->y = y + box.top;
			ball->jump(BALL_HEADING_YVELOCITY);
		}
	}
}

oEnemyPoke::oEnemyPoke(GameScene* nclan, double nx, double ny)
	: oPikachu(nclan, nx, ny) {
	auto sprite = game_framework.find_sprite(SPRITE_PIKA);
	sprite_init(sprite);
	look_at(LOOKDIR::LEFT);
	set_xborder(ENEMY_X_MIN, ENEMY_X_MAX);
}

void oEnemyPoke::on_update(double frame_advance) {
	oPikachu::on_update(frame_advance);
}

oVolleyBall::oVolleyBall(GameScene* nclan, double nx, double ny)
	: GameInstance(nclan, nx, ny) {
	auto sprite = game_framework.find_sprite(SPRITE_BALL);
	sprite_init(sprite);
	gravity = GRAVITY_BALL;
	set_xborder(PLAYER_X_MIN, ENEMY_X_MAX);
}

void oVolleyBall::on_update(double frame_advance) {
	GameInstance::on_update(frame_advance);

	double checky = y + box.bottom;
	if (FENCE_Y <= checky) { // 네트에 팅겼을 땐 살살 팅긴다.
		auto xspeed = hspeed * frame_advance;
		if (xspeed != 0.0) {
			if (RESOLUTION_W * 0.5 <= x && xspeed < 0) { // 오른쪽 부분에서 팅김
				auto checkx = x + box.left + xspeed - 1;

				if (checkx <= FENCE_X_RIGHT) {
					hspeed *= -0.7;
					vspeed *= -1;
					
					// 정규화
					double speed = point_distance(0, 0, hspeed, vspeed);
					double nx = hspeed / speed;
					double ny = vspeed / speed;

				}
			} else if (RESOLUTION_W * 0.5 <= x && 0 < xspeed) { // 왼쪽 부분에서 팅김
				auto checkx = x + box.right + xspeed + 1;

				if (FENCE_X_LEFT < checkx) {
					hspeed *= -0.7;
					vspeed *= -1;
				}
			}
		}
	}
}

void oVolleyBall::thud_left() {
	hspeed *= -1;
	contact_x_left();
}

void oVolleyBall::thud_right() {
	hspeed *= -1;
	contact_x_right();
}

void oVolleyBall::thud() {
	if (vspeed < 10)
		vspeed = 0;
	else
		vspeed *= -0.9;
	contact_y();
}

////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////

oPikachu::oPikachu(GameScene* nclan, double nx, double ny)
	: GameInstance(nclan, nx, ny), jumping(false), sliding(false), blinking(false)
	, stamina(PIKA_STAMINA_MAX), wake_time(0.0), blink_time(0.0), blink_recover_time(0.0) {
	look_at(LOOKDIR::RIGHT);
	image_speed = 0.0;
}

void oPikachu::on_update(double frame_advance) {
	GameInstance::on_update(frame_advance);

	if (is_blinking()) {
		if (blink_time < PIKA_BLINK_DURATION) {
			blink_time += frame_advance;
		} else {
			blink_recover();
		}
	}

	if (0 <= blink_recover_time)
		blink_recover_time -= frame_advance;
	else
		blink_recover_time = 0;

	if (stamina < PIKA_STAMINA_MAX) {
		stamina += frame_advance;
	}

	if (0 < wake_time) {
		wake_time -= frame_advance;
	}
}

void oPikachu::walk(int direction, double frame_advance) {
	if (direction < 0) { // 왼쪽
		if (hspeed < 0) {
			if (-PIKA_MOVE_XVELOCITY < hspeed) {
				if (0 <= blink_recover_time)
					hspeed -= PIKA_MOVE_XACCEL * frame_advance * 0.1;
				else
					hspeed -= PIKA_MOVE_XACCEL * frame_advance;
			} else {
				hspeed = -PIKA_MOVE_XVELOCITY;
			}
		} else {
			hspeed -= PIKA_MOVE_XACCEL * frame_advance;
		}
	} else { // 오른쪽
		if (0 < hspeed) {
			if (hspeed < PIKA_MOVE_XVELOCITY) {
				if (0 <= blink_recover_time)
					hspeed += PIKA_MOVE_XACCEL * frame_advance * 0.1;
				else
					hspeed += PIKA_MOVE_XACCEL * frame_advance;
			} else {
				hspeed = PIKA_MOVE_XVELOCITY;
			}
		} else {
			hspeed += PIKA_MOVE_XACCEL * frame_advance;
		}
	}
}

void oPikachu::jump(double power) {
	GameInstance::jump(power);

	set_flag_jump(true);
}

void oPikachu::thud_left() {
	if (is_blinking()) {
		// 전광석화 중에 벽에 닿으면 바로 끝난다.
		blink_recover();
	} else if (is_rolling() && FENCE_APGUREUGI_BOUNCE_MIN_XVELOCITY <= -hspeed) {
		// 구르기 중에 벽에 닿으면 팅긴다.
		hspeed *= -0.9; // 속도 역시 반대쪽으로 팅김
	} else {
		hspeed = 0; // 정지
	}
	contact_x_left();
}

void oPikachu::thud_right() {
	if (is_blinking()) {
		// 전광석화 중에 벽에 닿으면 바로 끝난다.
		blink_recover();
	} else if (is_rolling() && FENCE_APGUREUGI_BOUNCE_MIN_XVELOCITY <= hspeed) {
		// 구르기 중에 벽에 닿으면 팅긴다.
		hspeed *= -0.9; // 속도 역시 반대쪽으로 팅김
	} else {
		hspeed = 0; // 정지
	}
	contact_x_right();
}

void oPikachu::thud() {
	if (is_rolling()) { // 구르기 먼저 판정
		if (PIKA_APGUREUGI_BOUNCE_STD_YVELOCITY < vspeed) { // 높이 뛰면 살짝 튕긴다.
			contact_y();

			hspeed *= 0.5;
			jump(PIKA_APGUREUGI_BOUNCE_YVELOCITY);
		} else { // 그렇지 않으면 땅에 착지한다.
			GameInstance::thud();
			set_flag_jump(false);

			hspeed = 0.0;
			gravity = GRAVITY;
			rolling_recover();
		}
	} else if (is_jumping()) {
		GameInstance::thud();
		set_flag_jump(false);
	}
}

void oPikachu::set_flag_jump(bool flag) {
	jumping = flag;
}

void oPikachu::set_flag_roll(bool flag) {
	sliding = flag;
}

void oPikachu::set_flag_blink(bool flag) {
	blinking = flag;
}

bool oPikachu::can_action() const {
	return (wake_time <= 0 && !is_acting());
}

bool oPikachu::can_move() const {
	return (can_action());
}

bool oPikachu::can_jump() const {
	return (can_action() && !is_jumping());
}

bool oPikachu::can_smash() const {
	return (!is_acting());
}

bool oPikachu::can_rollingforward() const {
	return (STAMINA_SPEND_APGUREUGI <= stamina && can_action() && !is_jumping());
}

bool oPikachu::can_blink() const {
	return (STAMINA_SPEND_BLINK <= stamina && can_action());
}

void oPikachu::rollingforward() {
	set_flag_roll(true);

	hspeed = PIKA_APGUREUGI_XVELOCITY * (int)dir;
	stamina -= STAMINA_SPEND_APGUREUGI;
	gravity = PIKA_APGUREUGI_GRAVITY;
	jump(PIKA_APGUREUGI_YVELOCITY);
}

void oPikachu::blink() {
	set_flag_blink(true);

	hspeed = PIKA_BLINK_VELOCITY * (int)dir;
	stamina -= STAMINA_SPEND_BLINK;
	blink_time = 0;
	blink_recover_time = 0;
	gravity = 0;
}

void oPikachu::rolling_recover() {
	set_flag_roll(false);

	wake_time = PIKA_APGUREUGI_REBOUND_DURATION;
}

void oPikachu::blink_recover() {
	set_flag_blink(false);

	hspeed = PIKA_BLINK_REBOUND_VELOCITY * (int)dir;
	gravity = GRAVITY;
	wake_time = PIKA_BLINK_STOP_DURATION;
	blink_recover_time = PIKA_BLINK_RECOVER_DURATION;
}

bool oPikachu::is_acting() const {
	return sliding || blinking;
}

bool oPikachu::is_jumping() const {
	return jumping;
}

bool oPikachu::is_rolling() const {
	return sliding;
}

bool oPikachu::is_blinking() const {
	return blinking;
}

void oPikachu::look_at(LOOKDIR direction) {
	dir = direction;
	if (direction == LOOKDIR::LEFT)
		image_index = 0.0;
	else
		image_index = 1.0;
}

////////////////////////////////////////////////////////////////////////////////////////////

GameInstance::GameInstance(GameScene* nclan, double nx, double ny)
	: GameBehavior(), room(nclan), sprite_index(nullptr), box{ 0, 0, 0, 0 }
	, x(nx), y(ny), image_index(0.0), image_speed(0.0)
	, image_xscale(1.0), image_yscale(1.0), image_angle(0.0)
	, hspeed(0.0), vspeed(0.0), gravity(GRAVITY)
	, x_min(PLAYER_X_MIN), x_max(ENEMY_X_MAX) {}

GameInstance::~GameInstance() {
	if (room) {
		room->instance_kill(this);
		room = nullptr;
	}
	if (sprite_index)
		sprite_index.reset();
}

void GameInstance::sprite_init(shared_ptr<GameSprite>& sprite) {
	sprite_set(sprite);
	CopyRect(&box, &(sprite->bbox));
}

void GameInstance::sprite_set(shared_ptr<GameSprite>& sprite) {
	sprite_index = sprite;
	image_index = 0.0;
}

bool GameInstance::collide_with(GameInstance*& other) {
	if (sprite_index && other && other->sprite_index) {
		auto& otherbox = other->box;
		OffsetRect(&otherbox, other->x, other->y);
		OffsetRect(&box, x, y);

		RECT temp;
		bool result = (bool)IntersectRect(&temp, &box, &otherbox);

		OffsetRect(&otherbox, -(other->x), -(other->y));
		OffsetRect(&box, -x, -y);

		return result;
	}

	return false;
}

void GameInstance::move(const double mx, const double my) {
	x += mx;
	y += my;
}

void GameInstance::jump(double power) {
	vspeed = -power;
}

void GameInstance::contact_x_left() {
	x = x_min - box.left;
}

void GameInstance::contact_x_right() {
	x = x_max - box.right;
}

void GameInstance::contact_y() {
	y = (GROUND_Y - box.bottom - 1.0);
}

void GameInstance::thud_left() {
	hspeed = 0;
	contact_x_left();
}

void GameInstance::thud_right() {
	hspeed = 0;
	contact_x_right();
}

void GameInstance::thud() {
	vspeed = 0;
	contact_y();
}

void GameInstance::set_xborder(const double min, const double max) {
	x_min = min;
	x_max = max;
}

void GameInstance::on_update(double frame_advance) {
	if (frame_advance <= 0)
		return;

	auto xspeed = hspeed * frame_advance;
	if (xspeed != 0.0) {
		if (xspeed < 0) { // 왼쪽 이동
			auto checkx = x + box.left + xspeed - 1;

			if (checkx < x_min) {
				thud_left();
			} else {
				move(xspeed, 0);
			}
		} else { // 오른쪽 이동
			auto checkx = x + box.right + xspeed + 1;

			if (x_max < checkx) {
				thud_right();
			} else {
				move(xspeed, 0);
			}
		}
	}

	double checky = y + box.bottom;
	if (checky < GROUND_Y) {
		vspeed += gravity * frame_advance;
	}

	auto yspeed = vspeed * frame_advance;
	if (vspeed < 0) {
		move(0, yspeed);
	} else {
		checky = y + box.bottom + yspeed + 1;

		if (checky < GROUND_Y) {
			move(0, yspeed);
		} else {
			thud();
		}
	}
}

void GameInstance::on_update_later(double frame_advance) {
	if (sprite_index) {
		double animation_speed;
		auto image_number = sprite_index->number;

		if (1 < image_number && 0.0 != (animation_speed = image_speed * frame_advance)) {
			image_index += animation_speed;

			// clipping
			while (image_index < 0) image_index += image_number;
			while (image_number <= image_index) image_index -= image_number;
		}
	}
}

void GameInstance::on_render(HDC canvas) {
	if (sprite_index) {
		sprite_index->draw(canvas, x, y, image_index, image_angle, image_xscale, image_yscale);
	}
}

