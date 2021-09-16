#pragma once
#include "resource.h"


class GameInput {
public:
	bool pressing = false;
};


// 게임 구동부
class GameFramework {
public:
	GameFramework();
	~GameFramework();

	UINT mouse_x, mouse_y; // 마우스 좌표

	void input_register(int);
	bool input_check(int);
	//bool input_check_pressed(int button);
	//bool input_check_released(int button);

	void init();
	void build(); // 장면을 불러옵니다.
	bool update();
	void quit();

	void on_create();
	void on_destroy();
	void on_update(double);
	void on_update_later(double);
	void on_render(HWND);

	template<class GScene = GameScene>
	GameScene* state_push() {
		var room = new GScene();
		states.push_back(room);

		return (GameScene*)room;
	}

	bool state_is_done() const;
	void state_clear();
	bool state_remains();
	void state_jump(const INT);
	void state_jump_next();

	friend LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
private:
	double delta_time; // 실제 시간
	//GameChrono game_clock; // 게임 시간 재기 객체입니다.

	// 게임 장면 모음
	deque<GameScene*> states;
	GameScene* state_id;
	INT state_handle = 0;

	PAINTSTRUCT painter; // 렌더링 정보

	map<WPARAM, shared_ptr<GameInput>> key_checkers;
};

// 마우스 버튼
const WPARAM VB_LEFT = 0, VB_MIDDLE = 1, VB_RIGHT = 2;
