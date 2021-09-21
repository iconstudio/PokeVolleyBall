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

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

constexpr LONG GROUND_Y = RESOLUTION_H - 100; // 접지 좌표
constexpr double GRAVITY = 6.0; // 시간 당 미터 이동거리

constexpr double PIKA_X_START_PLAYER = RESOLUTION_W * 0.3; // 플레이어 시작 x 좌표
constexpr double PIKA_X_START_ENEMY = RESOLUTION_W * 0.7; // 상대 시작 x 좌표
constexpr double PIKA_Y_START = GROUND_Y - 30.; // 공통 시작 y 좌표

constexpr double BALL_PLAYER_X_START = PIKA_X_START_PLAYER; // 플레이어 서브의 공 시작 x 좌표
constexpr double BALL_PLAYER_Y_START = PIKA_Y_START - 200.; // 플레이어 서브의 공 시작 y 좌표
constexpr double BALL_ENEMY_X_START = PIKA_X_START_ENEMY; // 적 서브의 공 시작 x 좌표
constexpr double BALL_ENEMY_Y_START = PIKA_Y_START - 200.; // 적 서브의 공 시작 y 좌표

constexpr double PIKA_MOVE_XACCEL = 4.; // 피카츄 좌우 가속도
constexpr double PIKA_MOVE_XVELOCITY = 25.; // 피카츄 좌우 이동 속도
constexpr double PIKA_JUMP_VELOCITY = 140.; // 피카츄 점프 속도

constexpr double PIKA_APGUREUGI_GRAVITY = 1.1; // 앞구르기 할때 바뀌는 중력
constexpr double PIKA_APGUREUGI_XVELOCITY = 40.; // 앞구르기 전진 속도
constexpr double PIKA_APGUREUGI_YVELOCITY = 11.; // 앞구르기 뛰기 속도
constexpr double PIKA_APGUREUGI_BOUNCE_YVELOCITY = 6.;
constexpr double PIKA_APGUREUGI_BOUNCE_STD_YVELOCITY = 7.;
constexpr double PIKA_APGUREUGI_REBOUND_DURATION = 0.3; // 앞구르기 낙법 후 회복 시간

constexpr double PIKA_STAMINA_MAX = 15.0; // 최대 체력 (시간)
constexpr double STAMINA_SPEND_APGUREUGI = 3.5; // 앞구르기의 소모 체력 (시간)
constexpr double STAMINA_SPEND_BLINK = 5.5; // 전광석화의 소모 체력 (시간)

constexpr double FENCE_X_LEFT = RESOLUTION_W * 0.5 - 8.0; // 가운데 네트의 좌측 경계
constexpr double FENCE_X_RIGHT = RESOLUTION_W * 0.5 + 8.0; // 가운데 네트의 우측 경계
constexpr double PLAYER_X_MIN = 10.0;
constexpr double PLAYER_X_MAX = FENCE_X_LEFT;
constexpr double ENEMY_X_MIN = FENCE_X_RIGHT;
constexpr double ENEMY_X_MAX = RESOLUTION_W - 10.0;
constexpr double FENCE_APGUREUGI_BOUNCE_MIN_XVELOCITY = 8.; // 앞구르기 할때 벽에 닿으면 팅겨낼 속도의 최소값


enum class LOOKDIR : int { LEFT = -1, RIGHT = 1 };

enum class TURN { player, enemy };

class GameCamera {
public:
	GameCamera();
	~GameCamera();

	void on_update_later(HWND);

	bool canmove = true;

	static long width, height; // 뷰의 크기
	RECT* rect; // 뷰의 사각형
	double x, y; // 뷰의 좌표
	double destx, desty; // 뷰의 목표 좌표
	double speed = 100; // 뷰의 속도 (-1 이면 바로 이동)

	double hspeed, vspeed; // 뷰의 속도 (종속 변수)
};

// 로고 도입부
class sceneIntro : public GameScene {
public:
	using parent = GameScene;

	virtual void on_create() override;
	// 도입부 장면이 끝날 때 로고 스프라이트를 삭제합니다. 매우 거대한 스프라이트라서 메모리를 많이 차지합니다.
	virtual void on_destroy() override;
	virtual void on_update(double frame_advance) override;
	virtual void on_render(HDC canvas) override;

private:
	shared_ptr<GameSprite> logo;
	Phaser state{ 0.6, 3.0, 0.8, 1.0 };
	double alpha = 1.0;
};

// 타이틀
class sceneTitle : public GameScene {
public:
	using parent = GameScene;

private:

};

// 주 메뉴
class sceneMainMenu : public GameScene {
public:
	using parent = GameScene;

private:

};

// 게임 준비
class sceneGameReady : public GameScene {
public:
	using parent = GameScene;

private:

};

// 게임 중
class sceneGame : public GameScene {
public:
	using parent = GameScene;

	sceneGame();

	virtual void on_create() override;
	virtual void on_destroy() override;
	virtual void on_update(double frame_advance) override;
	virtual void on_update_later(double frame_advance) override;
	virtual void on_render(HDC canvas) override;

private:
	TURN turn; // 현재 차례

	oVolleyBall* ball;
	oPlayerPoke* player;
	oEnemyPoke* enemy;
};

// 게임 일시 정지
class sceneGamePaused : public GameScene {
public:
	using parent = GameScene;

private:

};

// 게임 완수
class sceneGameComplete : public GameScene {
public:
	using parent = GameScene;

private:

};

// 점수 정산
class sceneScoring : public GameScene {
public:
	using parent = GameScene;

private:

};

// 설정
class sceneSetting : public GameScene {
public:
	using parent = GameScene;


private:

};

class oSolid : public GameInstance {
public:
	const long my_serial = 2L;
	static constexpr long serial = 2L;

	using parent = GameInstance;


};

// 중력 개체
class oGraviton : public GameInstance {
	public:
		const long my_serial = 1L;
		static constexpr long serial = 1L;

		using parent = GameInstance;

		oGraviton(GameScene* nclan, double nx, double ny);

		// 중력 개체 동작
		virtual void on_update(double frame_advance) override;

		virtual void jump(double power);
		virtual void contact();
		virtual void thud();

		double hspeed, vspeed, gravity;
		double hbounce, vbounce;

};

class oVolleyBall : public oGraviton {
public:
	const long my_serial = 3L;
	static constexpr long serial = 3L;

	using parent = oGraviton;

	oVolleyBall(GameScene* nclan, double nx, double ny);
};

class oPokemon : public oGraviton {
public:
	const long my_serial = 4L;
	static constexpr long serial = 4L;

	using parent = oGraviton;

	oPokemon(GameScene* nclan, double nx, double ny);
	virtual ~oPokemon();

	virtual void on_create() override;
	virtual void on_update(double frame_advance) override;

	virtual void move(int direction, double frame_advance); // hspeed 기반의 좌우 이동
	virtual void jump(double power); // vspeed 기반의 점프
	virtual void thud(); // 착지

	bool can_action(); // 이동과 점프 가능한지 여부
	bool can_rollingforward();
	bool can_blink();

	void rollingforward(); // 앞구르기
	void blink(); // 전광석화

	void thud_recover(); // 앞구르기 후 회복
	void blink_recover(); // 전광석화 후 회복

	void look_at(LOOKDIR direction);

	bool jumping, sliding, blinking; // 상태 플래그

	double stamina; // 체력

	double wake_time;
	double x_min, x_max;
	LOOKDIR dir;

	shared_ptr<GameInstance> ball;

};

class oPlayerPoke : public oPokemon {
public:
	const long my_serial = 5L;
	static constexpr long serial = 5L;

	using parent = oPokemon;

	oPlayerPoke(GameScene* nclan, double nx, double ny);

	virtual void on_update(double frame_advance) override;
};

class oEnemyPoke : public oPokemon {
public:
	const long my_serial = 6L;
	static constexpr long serial = 6L;

	using parent = oPokemon;

	oEnemyPoke(GameScene* nclan, double nx, double ny);

	virtual void on_update(double frame_advance) override;
};
