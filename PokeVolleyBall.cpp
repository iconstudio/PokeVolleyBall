// PokeValleyBall.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include "pch.h"
#include "stdafx.h"
#include "PokeVolleyBall.h"
#include "Client.h"


#define MAX_LOADSTRING 100


// 전역 변수:
WCHAR szTitle[MAX_LOADSTRING];								// 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];						// 기본 창 클래스 이름입니다.
WindowsClient game_client{ RESOLUTION_W, RESOLUTION_H };	// 클라이언트 객체입니다.
GameFramework game_framework;


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


	// 게임 진행:
	game_framework.init();

	game_framework.input_register(VK_LEFT);
	game_framework.input_register(VK_RIGHT);
	game_framework.input_register(VK_UP);
	game_framework.input_register(VK_DOWN);

	auto room_0 = game_framework.state_push<sceneGame>();
	game_framework.state_jump(0);

	//auto room_1 = make_scene<sceneTitle>();
	//auto room_2 = make_scene<sceneMainMenu>();
	//auto room_3 = make_scene<sceneSetting>();
	//auto room_4 = make_scene<sceneGameReady>();
	//auto room_5 = make_scene<sceneGame>();
	//auto room_6 = make_scene<sceneGamePaused>();
	//auto room_7 = make_scene<sceneGameComplete>();
	//auto room_8 = make_scene<sceneScoring>();


	// 기본 메시지 루프입니다:
	while (true) {
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}

			if (!::TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
	}

	game_framework.state_clear();

	return (int)msg.wParam;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
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
			auto vk_status = game_framework.key_checkers[VB_LEFT];
			vk_status->pressing = true;

			game_framework.mouse_x = LOWORD(lParam);
			game_framework.mouse_y = HIWORD(lParam);
		}
		break;

		// 마우스 왼쪽 뗌
		case WM_LBUTTONUP:
		{
			auto vk_status = game_framework.key_checkers[VB_LEFT];
			vk_status->pressing = false;
		}
		break;

		// 마우스 오른쪽 누름
		case WM_RBUTTONDOWN:
		{
			auto vk_status = game_framework.key_checkers[VB_RIGHT];
			vk_status->pressing = true;

			game_framework.mouse_x = LOWORD(lParam);
			game_framework.mouse_y = HIWORD(lParam);
		}
		break;

		// 마우스 오른쪽 뗌
		case WM_RBUTTONUP:
		{
			auto vk_status = game_framework.key_checkers[VB_RIGHT];
			vk_status->pressing = false;
		}
		break;

		// 마우스 휠 누름
		case WM_MBUTTONDOWN:
		{
			auto vk_status = game_framework.key_checkers[VB_MIDDLE];
			vk_status->pressing = true;

			game_framework.mouse_x = LOWORD(lParam);
			game_framework.mouse_y = HIWORD(lParam);
		}
		break;

		// 마우스 휠 뗌
		case WM_MBUTTONUP:
		{
			auto vk_status = game_framework.key_checkers[VB_MIDDLE];
			vk_status->pressing = false;
		}
		break;

		// 키보드 누름
		case WM_KEYDOWN:
		{
			if (wParam == VK_F1) {
				DialogBox(game_client.instance, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
				break;
			}
			
			auto vk_status = game_framework.key_checkers.find(wParam);
			if (vk_status != game_framework.key_checkers.end()) {
				vk_status->second->pressing = true;
			}
		}
		break;

		// 키보드 뗌
		case WM_KEYUP:
		{
			auto vk_status = game_framework.key_checkers.find(wParam);
			if (vk_status != game_framework.key_checkers.end()) {
				vk_status->second->pressing = false;
			}
		}
		break;

		// 렌더링 타이머
		case WM_TIMER:
		{
			Render::refresh(hwnd);
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

sceneIntro::sceneIntro() : parent() {}

sceneIntro::~sceneIntro() {}

sceneTitle::sceneTitle() : parent() {}

sceneTitle::~sceneTitle() {}

sceneGame::sceneGame() {}

sceneGame::~sceneGame() {}

void sceneGame::on_create() {}

void sceneGame::on_destroy() {}

void sceneGame::on_update(double frame_advance) {}

void sceneGame::on_update_later(double frame_advance) {}

void sceneGame::on_render(HDC canvas) {
}

sceneMainMenu::sceneMainMenu() {}

sceneMainMenu::~sceneMainMenu() {}
