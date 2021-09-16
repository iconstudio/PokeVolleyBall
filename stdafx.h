#pragma once

#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "Ws2_32.lib")

#include "targetver.h"

/// C 런타임 헤더 파일입니다.
//#define _CRT_SECURE_NO_WARNINGS

#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>
#include <cstdlib>


/// Windows 헤더 파일:
#define WIN32_LEAN_AND_MEAN // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#include <windows.h>
#include <winperf.h>


/// ATL / MFC 헤더 파일:
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS // 일부 CString 생성자는 명시적으로 선언됩니다.
#include <atlbase.h>
#include <atlimage.h>
#include <atlstr.h>


// 수학 상수 선언
#define _USE_MATH_DEFINES
#include <math.h>


// 표준 라이브러리
#include <memory>
#include <vector>
#include <list>
#include <stack>
#include <set>
#include <map>
#include <queue>
#include <algorithm>
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>

using std::vector;
using std::list;
using std::stack;
using std::set;
using std::map;
using std::priority_queue;
using std::deque;
using std::string;
using std::shared_ptr;
using std::unique_ptr;
using std::weak_ptr;
using std::make_shared;
using std::make_unique;
using std::cout;
using std::endl;


class GameInput;
class GameFramework;
class GameSprite;

class GameBehavior;
class GameScene;
class sceneTitle;
class sceneMainMenu;
class sceneGameReady;
class sceneGame;
class sceneGamePaused;
class sceneGameComplete;
class sceneScoring;
class sceneSetting;

class GameInstance;
class oGraviton;
class oValleyBall;
class oPokemon;
class oPlayerPoke;
class oEnemyPoke;


namespace Render {
	void refresh(HWND hwnd);

	void transform_set(HDC, XFORM&);
	void transform_set_identity(HDC);
	void transform_set_rotation(HDC);

	BOOL draw_rectangle(HDC, int, int, int, int);
	void draw_end(HDC, HGDIOBJ, HGDIOBJ);

	static XFORM transform_identity{ 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };
}


#define sign(_x) ((_x > 0) ? 1 : ((_x == 0) ? 0 : -1))

inline constexpr auto radtodeg(double value) {
	return value / M_PI * 180;
}

inline constexpr auto degtorad(double value) {
	return value * M_PI / 180;
}

inline double lengthdir_x(double length, double direction) {
	return cos(degtorad(direction)) * length;
}

inline double lengthdir_y(double length, double direction) {
	return -sin(degtorad(direction)) * length;
}
