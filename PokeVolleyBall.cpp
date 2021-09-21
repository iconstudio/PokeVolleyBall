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

auto SPRITE_BALL = game_framework.make_sprite(TEXT("res\\ball.png"), 1, 32, 32);
auto SPRITE_PIKA = game_framework.make_sprite(TEXT("res\\bigpikaboth_strip2.png"), 2, 40, 40);


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

	// 사용자 코드입니다:
	game_framework.input_register(VK_ESCAPE); // 나가기 및 취소
	game_framework.input_register(VK_RETURN); // 확인
	game_framework.input_register(VK_SPACE); // 점프 및 확인

	game_framework.input_register(VK_LEFT);
	game_framework.input_register(VK_RIGHT);
	game_framework.input_register(VK_UP);
	game_framework.input_register(VK_DOWN);
	game_framework.input_register(VK_CHAR_Z); // Z
	game_framework.input_register(VK_CHAR_X); // X
	game_framework.input_register(VK_CHAR_C); // C
	
	game_framework.input_register(VK_CHAR_R); // R - 게임 다시 시작 (장면이 sceneGame일때만 작동)
	game_framework.input_register(VK_F1); // 도움말
	game_framework.input_register(VK_F2); // 게임 전체 다시 시작
	game_framework.input_register(VK_F3); // 현재 장면만 다시 시작


	var room_0 = game_framework.state_push<sceneGame>();
	//var find_0 = room_0->instance_id(0);


	// 게임 빌드
	game_framework.build();

	// 기본 메시지 루프입니다:
	bool done = false;

	while (true) {
		game_framework.on_create();

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
		game_framework.on_destroy();

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

		// 창 크기 조절
		case WM_SIZE:
		{
			game_client.width = LOWORD(lParam);
			game_client.height = HIWORD(lParam);
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

		// 렌더링 타이머
		case WM_TIMER:
		{
			game_framework.delta_inspect();
			Render::refresh(hwnd);
			game_framework.update();
			game_framework.delta_start();
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


////////////////////////////////////////////////////////////////////////////////////////////

sceneGame::sceneGame()
	: turn(TURN::player), ball(nullptr), player(nullptr), enemy(nullptr) {
}

void sceneGame::on_create() {
	ball = instance_create<oVolleyBall>(BALL_PLAYER_X_START, BALL_PLAYER_Y_START);
	player = instance_create<oPlayerPoke>(PIKA_X_START_PLAYER, PIKA_Y_START);
	enemy = instance_create<oEnemyPoke>(PIKA_X_START_ENEMY, PIKA_Y_START);

	player->ball = shared_ptr<oVolleyBall>(ball);
	enemy->ball = shared_ptr<oVolleyBall>(ball);

	parent::on_create();
}

void sceneGame::on_destroy() {
	parent::on_destroy();
}

void sceneGame::on_update(double frame_advance) {
	parent::on_update(frame_advance);
}

void sceneGame::on_update_later(double frame_advance) {
	parent::on_update_later(frame_advance);
}

void sceneGame::on_render(HDC canvas) {
	parent::on_render(canvas);
}

////////////////////////////////////////////////////////////////////////////////////////////

oGraviton::oGraviton(GameScene* nclan, double nx, double ny)
	: parent(nclan, nx, ny), hspeed(0.0), vspeed(0.0), gravity(GRAVITY), hbounce(0.0), vbounce(0.0) {
}

// 중력 개체 동작
void oGraviton::on_update(double frame_advance) {
	if (frame_advance <= 0)
		return;

	double checky = y + box.bottom;
	if (checky < GROUND_Y) {
		vspeed += gravity;
	}

	auto yspeed = km_per_hr(vspeed) * frame_advance;

	if (vspeed < 0) {
		y += yspeed;
	} else {
		checky = y + box.bottom + yspeed + 1;

		if (checky < GROUND_Y) {
			y += yspeed;
		} else {
			thud();
		}
	}
}

void oGraviton::jump(double power) {
	vspeed = -power;
}

void oGraviton::contact() {
	y = (double)(GROUND_Y - box.bottom - 1L);
}

void oGraviton::thud() {
	vspeed = 0;
	contact();
}

oVolleyBall::oVolleyBall(GameScene* nclan, double nx, double ny)
	: parent(nclan, nx, ny) {
	auto sprite = game_framework.find_sprite(SPRITE_BALL);
	sprite_set(sprite);

	hbounce = 0.5;
	vbounce = 0.5;
}

oPokemon::oPokemon(GameScene* nclan, double nx, double ny)
	: parent(nclan, nx, ny), jumping(false), sliding(false)
	, stamina(PIKA_STAMINA_MAX), wake_time(0.0), wake_period(0.3) {
	look_at(LOOKDIR::RIGHT);
}

oPokemon::~oPokemon() {
	if (ball)
		ball.reset();
}

void oPokemon::on_create() {
}

void oPokemon::on_update(double frame_advance) {
	parent::on_update(frame_advance);

	auto xspeed = km_per_hr(hspeed) * frame_advance;

	if (xspeed != 0.0) {
		if (xspeed < 0) { // 왼쪽 이동
			auto checkx = x + box.left + xspeed - 1;

			if (checkx < x_min) {
				if (sliding && FENCE_APGUREUGI_BOUNCE_MIN_XVELOCITY <= -hspeed) { // 구르기 중에 벽에 닿으면 팅긴다.
					auto gabx = abs(x - x_min);
					x = x_min - box.left;
					//x += abs(gabx + xspeed); // 남은 비거리 동안 반대쪽으로 이동
					hspeed *= -1; // 속도 역시 반대쪽으로 팅김
				} else {
					x = x_min - box.left;
					hspeed = 0; // 정지
				}
			} else {
				x += xspeed;
			}
		} else { // 오른쪽 이동
			auto checkx = x + box.right + xspeed + 1;

			if (x_max < checkx) {
				if (sliding && FENCE_APGUREUGI_BOUNCE_MIN_XVELOCITY <= hspeed) { // 구르기 중에 벽에 닿으면 팅긴다.
					auto gabx = abs(x_max - x);
					x = x_max - box.right;
					//x += abs(gabx - xspeed); // 남은 비거리 동안 반대쪽으로 이동
					hspeed *= -1; // 속도 역시 반대쪽으로 팅김
				} else {
					x = x_max - box.right;
					hspeed = 0; // 정지
				}
			} else {
				x += xspeed;
			}
		}
	}

	if (stamina < PIKA_STAMINA_MAX) {
		stamina += frame_advance;
	}

	if (0 < wake_time) {
		wake_time -= frame_advance;
	}

	// 공과 충돌했을 때만 기술을 사용할 수 있다.
	auto collide_with_ball = collide_with(ball);
	if (collide_with_ball) {

	}
}

void oPokemon::move(int direction, double frame_advance) {
	if (direction < 0) { // 왼쪽
		if (hspeed < 0) {
			if (-PIKA_MOVE_XVELOCITY < hspeed) {
				hspeed -= PIKA_MOVE_XACCEL;
			} else {
				hspeed = -PIKA_MOVE_XVELOCITY;
			}
		} else {
			hspeed -= PIKA_MOVE_XACCEL;
		}
	} else { // 오른쪽
		if (0 < hspeed) {
			if (hspeed < PIKA_MOVE_XVELOCITY) {
				hspeed += PIKA_MOVE_XACCEL;
			} else {
				hspeed = PIKA_MOVE_XVELOCITY;
			}
		} else {
			hspeed += PIKA_MOVE_XACCEL;
		}
	}
	//x += PIKA_MOVE_XVELOCITY * frame_advance * direction;
}

void oPokemon::jump(double power) {
	parent::jump(power);

	jumping = true;
}

bool oPokemon::can_action() {
	return (wake_time <= 0);
}

bool oPokemon::can_rollingforward() {
	return (STAMINA_SPEND_APGUREUGI <= stamina && can_action());
}

bool oPokemon::can_blink() {
	return (STAMINA_SPEND_BLINK <= stamina && can_action());
}

void oPokemon::rollingforward() {
	sliding = true;

	hspeed = PIKA_APGUREUGI_XVELOCITY * (int)dir;
	stamina -= STAMINA_SPEND_APGUREUGI;
	gravity = PIKA_APGUREUGI_GRAVITY;
	jump(PIKA_APGUREUGI_YVELOCITY);
}

void oPokemon::blink() {

}

void oPokemon::thud_recover() {
	wake_time = PIKA_APGUREUGI_REBOUND_DURATION;
}

void oPokemon::blink_recover() {

}

void oPokemon::thud() {
	if (sliding) { // 구르기 먼저 판정
		if (PIKA_APGUREUGI_BOUNCE_STD_YVELOCITY < vspeed) { // 높이 뛰면 살짝 튕긴다.
			contact();

			hspeed *= 0.5;
			jump(PIKA_APGUREUGI_BOUNCE_YVELOCITY);
		} else { // 그렇지 않으면 땅에 착지한다.
			parent::thud();
			sliding = false;

			hspeed = 0.0;
			gravity = GRAVITY;
			jumping = false;
			thud_recover();
		}
	} else if (jumping) {
		parent::thud();
		jumping = false;
	}
}

void oPokemon::look_at(LOOKDIR direction) {
	dir = direction;
	if (direction == LOOKDIR::LEFT)
		image_index = 0.0;
	else
		image_index = 1.0;
}

// 플레이어
oPlayerPoke::oPlayerPoke(GameScene* nclan, double nx, double ny)
	: parent(nclan, nx, ny) {
	auto sprite = game_framework.find_sprite(SPRITE_PIKA);
	sprite_set(sprite);
	look_at(LOOKDIR::RIGHT);
	x_min = PLAYER_X_MIN;
	x_max = PLAYER_X_MAX;
}

// 플레이어 동작
void oPlayerPoke::on_update(double frame_advance) {
	parent::on_update(frame_advance);

	int check_left = game_framework.input_check(VK_LEFT);
	int check_right = game_framework.input_check(VK_RIGHT);
	int check_up = game_framework.input_check(VK_UP);
	int check_down = game_framework.input_check(VK_DOWN);

	int check_hor = (check_right - check_left);

	if (!sliding) {
		if (can_action() && check_hor != 0) {
			move(check_hor, frame_advance);
			look_at((LOOKDIR)check_hor);
		} else {
			hspeed *= 0.2;
		}

		if (can_action() && !jumping) {
			if (game_framework.input_check(VK_CHAR_X)) { // 점프
				jump(PIKA_JUMP_VELOCITY);
			} else if (can_rollingforward() && game_framework.input_check(VK_CHAR_C)) { // 앞구르기
				rollingforward();
			}
		}
	} else {

	}
}

// 적 초기화
oEnemyPoke::oEnemyPoke(GameScene* nclan, double nx, double ny)
	: parent(nclan, nx, ny) {
	auto sprite = game_framework.find_sprite(SPRITE_PIKA);
	sprite_set(sprite);
	look_at(LOOKDIR::LEFT);
	x_min = ENEMY_X_MIN;
	x_max = ENEMY_X_MAX;
}

// 적 인공지능
void oEnemyPoke::on_update(double frame_advance) {
	parent::on_update(frame_advance);
}
