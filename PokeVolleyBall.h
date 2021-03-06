#pragma once
#include "resource.h"
#include "Framework.h"
#include "Phaser.h"
#include "Sprite.h"
#include "Core.h"

#define VK_CHAR_C 0x43
#define VK_CHAR_R 0x52
#define VK_CHAR_X 0x58
#define VK_CHAR_Z 0x5A
#define VK_CHAR_P 0x54

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

constexpr LONG GROUND_Y = RESOLUTION_H - 100; // 접지 좌표
constexpr double GRAVITY = km_per_hr(300.0);
constexpr double GRAVITY_BALL = km_per_hr(190.0);

constexpr double PIKA_X_START_PLAYER = RESOLUTION_W * 0.3; // 플레이어 시작 x 좌표
constexpr double PIKA_X_START_ENEMY = RESOLUTION_W * 0.7; // 상대 시작 x 좌표
constexpr double PIKA_Y_START = GROUND_Y - 30.; // 공통 시작 y 좌표

constexpr double BALL_PLAYER_X_START = PIKA_X_START_PLAYER; // 플레이어 서브의 공 시작 x 좌표
constexpr double BALL_PLAYER_Y_START = PIKA_Y_START - 200.; // 플레이어 서브의 공 시작 y 좌표
constexpr double BALL_ENEMY_X_START = PIKA_X_START_ENEMY; // 적 서브의 공 시작 x 좌표
constexpr double BALL_ENEMY_Y_START = PIKA_Y_START - 200.; // 적 서브의 공 시작 y 좌표

constexpr double FENCE_X_LEFT = RESOLUTION_W * 0.5 - 8.0; // 가운데 네트의 좌측 경계
constexpr double FENCE_X_RIGHT = RESOLUTION_W * 0.5 + 8.0; // 가운데 네트의 우측 경계
constexpr double FENCE_HEIGHT = GROUND_Y * 0.4; // 네트 높이
constexpr double FENCE_Y = GROUND_Y - FENCE_HEIGHT;
constexpr double PLAYER_X_MIN = 30.0;
constexpr double PLAYER_X_MAX = FENCE_X_LEFT;
constexpr double ENEMY_X_MIN = FENCE_X_RIGHT;
constexpr double ENEMY_X_MAX = RESOLUTION_W - 30.0;
constexpr double FENCE_APGUREUGI_BOUNCE_MIN_XVELOCITY = km_per_hr(8); // 앞구르기 할때 벽에 닿으면 팅겨낼 속도의 최소값
constexpr double FIELD_SIZE = PLAYER_X_MAX - PLAYER_X_MIN; // 플레이어가 다닐 수 있는 땅의 너비

constexpr double PIKA_MOVE_XACCEL = km_per_hr(190); // 피카츄 좌우 가속도
constexpr double PIKA_MOVE_XVELOCITY = km_per_hr(34); // 피카츄 좌우 이동 속도
constexpr double PIKA_JUMP_VELOCITY = km_per_hr(100); // 피카츄 점프 속도

constexpr double PIKA_APGUREUGI_GRAVITY = km_per_hr(55); // 앞구르기 할때 바뀌는 중력
constexpr double PIKA_APGUREUGI_XVELOCITY = km_per_hr(47); // 앞구르기 전진 속도
constexpr double PIKA_APGUREUGI_YVELOCITY = km_per_hr(12); // 앞구르기 뛰기 속도
constexpr double PIKA_APGUREUGI_BOUNCE_STD_YVELOCITY = km_per_hr(8);
constexpr double PIKA_APGUREUGI_BOUNCE_YVELOCITY = km_per_hr(6);
constexpr double PIKA_APGUREUGI_REBOUND_DURATION = 0.25; // 앞구르기 낙법 후 회복 시간

constexpr double PIKA_BLINK_DISTANCE = FIELD_SIZE * 0.38; // 전광석화 거리
constexpr double PIKA_BLINK_DURATION = 0.1; // 전광석화 소요 시간
constexpr double PIKA_BLINK_VELOCITY = PIKA_BLINK_DISTANCE / PIKA_BLINK_DURATION;
constexpr double PIKA_BLINK_STOP_DURATION = 0.25; // 전광석화 후 회복 시간
constexpr double PIKA_BLINK_RECOVER_DURATION = 1.0; // 전광석화 후 회복 시간
constexpr double PIKA_BLINK_REBOUND_VELOCITY = km_per_hr(20);

constexpr double PIKA_STAMINA_MAX = 15.0; // 최대 체력 (시간)
constexpr double STAMINA_SPEND_APGUREUGI = 3.5; // 앞구르기의 소모 체력 (시간)
constexpr double STAMINA_SPEND_BLINK = 5.5; // 전광석화의 소모 체력 (시간)

constexpr double BALL_HEADING_YVELOCITY = km_per_hr(72); // 일반 헤딩
constexpr double BALL_HEADING_JUMP_YVELOCITY = km_per_hr(120); // 점프 헤딩
constexpr double BALL_HEADING_APGUREUGI_XVELOCITY = km_per_hr(40); // 앞구르기할 때 헤딩
constexpr double BALL_HEADING_APGUREUGI_YVELOCITY = km_per_hr(80); // 앞구르기할 때 헤딩
constexpr double BALL_HEADING_BLINK_XVELOCITY = PIKA_BLINK_VELOCITY * 0.5; // 전광석화할 때 헤딩
constexpr double BALL_HEADING_BLINK_YVELOCITY = km_per_hr(110); // 전광석화할 때 헤딩
constexpr double BALL_SMASH_HAIRPIN_XVELOCITY = km_per_hr(80); // 수평으로 날리는 헤어핀
constexpr double BALL_SMASH_HAIRPIN_YVELOCITY = km_per_hr(10); // 수평으로 날리는 헤어핀
constexpr double BALL_SMASH_UP_XVELOCITY = -km_per_hr(50); // 위로 올려치는 토스
constexpr double BALL_SMASH_UP_YVELOCITY = km_per_hr(130); // 위로 올려치는 토스
constexpr double BALL_SMASH_SPIKE_YVELOCITY = km_per_hr(80); // 위에서 아래로 내리꽂는 스매시
constexpr double BALL_SMASH_SPIKE_XVELOCITY = km_per_hr(140); // 위에서 아래로 내리꽂는 스매시

enum class LOOKDIR : int { LEFT = -1, RIGHT = 1 };
enum class TURN { player, enemy };

// 로고 도입부
class sceneIntro : public GameScene {
public:
	virtual void on_create() override;
	virtual void on_destroy() override;
	virtual void on_update(double frame_advance) override;
	virtual void on_render(HDC canvas) override;

private:
	shared_ptr<GameSprite> sprite_index;
	double x = RESOLUTION_W * 0.5;
	double y = RESOLUTION_H * 0.5;

	Phaser state{ 0.6, 3.0, 0.8, 1.0 };
	double alpha = 1.0;
};

// 타이틀
class sceneTitle : public GameScene {
public:
	virtual void on_create() override;
	virtual void on_destroy() override;
	virtual void on_update(double frame_advance) override;
	virtual void on_render(HDC canvas) override;

private:
	shared_ptr<GameSprite> sprite_index;
	double x = RESOLUTION_W * 0.5;
	double y = RESOLUTION_H * 0.5 - 50;

	bool entering = false;
};

// 주 메뉴
class sceneMainMenu : public GameScene {
public:
	virtual void on_create() override;
	virtual void on_destroy() override;
	virtual void on_update(double frame_advance) override;
	virtual void on_render(HDC canvas) override;

private:

};

// 게임 중
class sceneGame : public GameScene {
public:
	virtual void on_create() override;
	virtual void on_destroy() override;
	virtual void on_update(double frame_advance) override;
	virtual void on_update_later(double frame_advance) override;
	virtual void on_render(HDC canvas) override;

private:
	bool ready_done;
	double ready_counter;
	bool wining;
	TURN winner;

	int score_player, score_enemy;

	TURN turn; // 현재 차례

	oVolleyBall* ball;
	oPlayerPoke* player;
	oEnemyPoke* enemy;
};

// 설정
class sceneSetting : public GameScene {
public:
	using parent = GameScene;


private:

};

class oVolleyBall : public GameInstance {
public:
	oVolleyBall(GameScene* nclan, double nx, double ny);

	virtual void on_update(double frame_advance) override;

	virtual void thud_left();
	virtual void thud_right();
	virtual void thud();
};

class oPikachu : public GameInstance {
public:
	oPikachu(GameScene* nclan, double nx, double ny);

	virtual void on_update(double frame_advance) override;

	void walk(int direction, double frame_advance); // hspeed 기반의 좌우 이동
	void jump(double power); // vspeed 기반의 점프
	virtual void thud_left();
	virtual void thud_right();
	virtual void thud(); // 착지

	void set_flag_jump(bool flag);
	void set_flag_roll(bool flag);
	void set_flag_blink(bool flag);

	bool can_action() const; // 행동이 가능한지 여부
	bool can_move() const; // 이동이 가능한지 여부
	bool can_jump() const; // 점프가 가능한지 여부
	bool can_smash() const; // 스매시가 가능한지 여부
	bool can_rollingforward() const; // 앞구르기가 가능한지 여부
	bool can_blink() const; // 전광석화가 가능한지 여부

	void rollingforward(); // 앞구르기
	void blink(); // 전광석화

	void rolling_recover(); // 앞구르기 후 회복
	void blink_recover(); // 전광석화 후 회복

	bool is_acting() const;
	bool is_jumping() const;
	bool is_rolling() const;
	bool is_blinking() const;

	void look_at(LOOKDIR direction);

	GameInstance* ball;
	LOOKDIR dir;

private:
	bool jumping, sliding, blinking, carrying; // 상태 플래그

	double stamina; // 체력

	double wake_time;
	double blink_time, blink_recover_time;
};

class oPlayerPoke : public oPikachu {
public:
	using parent = oPikachu;

	oPlayerPoke(GameScene* nclan, double nx, double ny);

	virtual void on_update(double frame_advance) override;
};

class oEnemyPoke : public oPikachu {
public:
	using parent = oPikachu;

	// 적 초기화
	oEnemyPoke(GameScene* nclan, double nx, double ny);

	// 적 인공지능
	virtual void on_update(double frame_advance) override;
};
