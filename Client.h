#pragma once
#include "resource.h"


// 클라이언트 메서드 집합
class WindowsClient {
public:
	WindowsClient(LONG width, LONG height);
	~WindowsClient();

	BOOL initialize(HINSTANCE handle, WNDPROC procedure, LPCWSTR title, LPCWSTR id, INT cmd_show);

	HINSTANCE instance;						// 프로세스 인스턴스
	HWND hwindow;							// 창 인스턴스
	WindowProcedure procedure;				// 창 처리기
	WNDCLASSEX properties;					// 창 등록정보
	LPCWSTR title_caption, class_id;		// 창 식별자
	LONG width, height;						// 창 크기
};
