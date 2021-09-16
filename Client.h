#pragma once
#include "resource.h"


// 클라이언트 메서드 집합
class WindowsClient {
public:
	/// <summary>
	///		GameClient(width, height)
	/// </summary>
	/// <param name="width">창의 너비</param>
	/// <param name="height">창의 높이</param>
	WindowsClient(const LONG, const LONG);
	~WindowsClient();

	/// <summary>
	///		initialize(handle)
	/// 
	///		주 창 클래스를 등록하고 생성합니다.
	/// </summary>
	/// <param name="handle">창 인스턴스</param>
	/// <param name="procedure">메시지 처리 메서드</param>
	/// <param name="title">클라이언트 제목</param>
	/// <param name="id">클라이언트 식별자</param>
	/// <param name="cmd_show">창 보기 명령</param>
	BOOL initialize(HINSTANCE, WNDPROC, LPCWSTR, LPCWSTR, INT);

	HINSTANCE instance;						// 프로세스 인스턴스
	HWND hwindow;							// 창 인스턴스
	WindowProcedure procedure;				// 창 처리기
	WNDCLASSEX properties;					// 창 등록정보
	LPCWSTR title_caption, class_id;		// 창 식별자
	LONG width, height;						// 창 크기
};
