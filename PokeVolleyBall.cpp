// PokeValleyBall.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include "pch.h"
#include "stdafx.h"
#include "PokeVolleyBall.h"
#include "Client.h"


#define MAX_LOADSTRING 100
#define RENDER_TIMER_ID 0
constexpr long GROUND_Y = RESOLUTION_H - 100;


// 전역 변수:
WCHAR szTitle[MAX_LOADSTRING];								// 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];						// 기본 창 클래스 이름입니다.
WindowsClient game_client{ RESOLUTION_W, RESOLUTION_H };	// 클라이언트 객체입니다.
GameFramework game_framework;

auto SPRITE_BALL = game_framework.make_sprite(TEXT("res\\ball.png"), 1, 32, 32);
auto SPRITE_PIKA = game_framework.make_sprite(TEXT("res\\pika.png"), 1, 18, 19);


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);


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
	game_framework.input_register(0x5A); // Z
	game_framework.input_register(0x58); // X
	game_framework.input_register(0x43); // C
	
	game_framework.input_register(0x52); // R - 게임 다시 시작 (장면이 sceneGame일때만 작동)
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
				//game_framework.update();
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
			Render::refresh(hwnd);
			game_framework.update();
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
	: turn(TURN::player), ball(nullptr), player(nullptr), enemy(nullptr)
	, player_sx(RESOLUTION_W * 0.3), player_sy(GROUND_Y - 30), enemy_sx(RESOLUTION_W * 0.7), enemy_sy(GROUND_Y - 30)
	, ball_player_sx(player_sx), ball_player_sy(player_sy - 200), ball_enemy_sx(enemy_sx), ball_enemy_sy(enemy_sy - 200) {
}

void sceneGame::on_create() {
	ball = instance_create<oVolleyBall>(ball_player_sx, ball_player_sy);
	player = instance_create<oPlayerPoke>(player_sx, player_sy);
	enemy = instance_create<oEnemyPoke>(enemy_sx, enemy_sy);

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

	auto yspeed = km_per_hr(vspeed);

	if (vspeed < 0) {
		y += yspeed;
	} else {
		checky = y + box.bottom + yspeed + 1;

		if (checky < GROUND_Y) {
			y += yspeed;
		} else {
			vspeed = 0;

			y = GROUND_Y - box.bottom - 1;
		}
	}
}

oVolleyBall::oVolleyBall(GameScene* nclan, double nx, double ny)
	: parent(nclan, nx, ny) {
	auto sprite = game_framework.find_sprite(SPRITE_BALL);
	sprite_set(sprite);

	hbounce = 0.5;
	vbounce = 0.5;
}

oPokemon::oPokemon(GameScene* nclan, double nx, double ny)
	: parent(nclan, nx, ny) {}

void oPokemon::on_update(double frame_advance) {
	parent::on_update(frame_advance);
}

void oPokemon::jump(double power) {}

// 플레이어
oPlayerPoke::oPlayerPoke(GameScene* nclan, double nx, double ny)
	: parent(nclan, nx, ny) {
	auto sprite = game_framework.find_sprite(SPRITE_PIKA);
	sprite_set(sprite);
}

// 플레이어 동작
void oPlayerPoke::on_update(double frame_advance) {
	parent::on_update(frame_advance);

	int check_left = game_framework.input_check(VK_LEFT);
	int check_right = game_framework.input_check(VK_RIGHT);
	int check_up = game_framework.input_check(VK_UP);
	int check_down = game_framework.input_check(VK_DOWN);

	int check_hor = (check_right - check_left);
	if (check_hor != 0) {
		//
		x += check_hor * frame_advance * 120;
	}


}

oEnemyPoke::oEnemyPoke(GameScene* nclan, double nx, double ny)
	: parent(nclan, nx, ny) {
	auto sprite = game_framework.find_sprite(SPRITE_PIKA);
	sprite_set(sprite);
}

void oEnemyPoke::on_update(double frame_advance) {
	parent::on_update(frame_advance);
}
