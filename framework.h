#pragma once
#include "resource.h"


struct ALL {};

class GameInput {
public:
	bool pressing = false;
};


// 게임 구동부
class GameFramework {
public:
	GameFramework();

	void input_register(int virtual_button);
	bool input_check(int virtual_button);
	//bool input_check_pressed(int button);
	//bool input_check_released(int button);

	void init();
	void build(); // 장면을 불러옵니다.
	bool update();
	void quit();
	double get_elapsed_second() const;

	size_t make_sprite(HINSTANCE instance, const UINT resource, const UINT number, const int xoff, const int yoff);
	size_t make_sprite(const LPCTSTR path, const UINT number, const int xoff, const int yoff);
	shared_ptr<GameSprite>& find_sprite(const size_t index);

	void on_create();
	void on_destroy();
	void on_update(const double frame_advance);
	void on_update_later(const double frame_advance);
	void on_mousedown(const WPARAM button, const LPARAM cursor);
	void on_mouseup(const WPARAM button, const LPARAM cursor);
	void on_keydown(const WPARAM key);
	void on_keyup(const WPARAM key);
	void on_render(HWND window);

	template<class GScene>
	GScene* state_push() {
		auto room = new GScene();
		states.push_back(room);

		return room;
	}

	bool state_is_done() const;
	void state_clear();
	bool state_remains();
	void state_jump(const INT);
	void state_jump_next();

	friend LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	GameScene* state_id;
	INT state_handle = 0;

	LONG mouse_x, mouse_y; // 마우스 좌표
private:
	using tick_type = std::chrono::microseconds;
	using clock_type = std::chrono::system_clock::time_point;

	clock_type clock_previos, clock_now;
	LONGLONG elapsed;
	double delta_time; // 실제 시간

	PAINTSTRUCT painter; // 렌더링 정보

	// 게임 장면 모음
	deque<GameScene*> states;

	vector<shared_ptr<GameSprite>> sprites;

	map<WPARAM, shared_ptr<GameInput>> key_checkers;
};

using garage_iterator = multimap<unsigned long, GameBehavior*>::iterator;

// 마우스 버튼
const WPARAM VB_LEFT = 0, VB_MIDDLE = 1, VB_RIGHT = 2;
