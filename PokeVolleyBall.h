#pragma once
#include "resource.h"
#include "Framework.h"
#include "Phaser.h"
#include "Sprite.h"
#include "Core.h"


constexpr double GRAVITY = km_per_hr(0.0098);


// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

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

	double hspeed, vspeed;			// 뷰의 속도 (종속 변수)
};


// 로고 도입부
class sceneIntro : public GameScene {
public:
	using parent = GameScene;

private:
	Phaser state{ 0.4f, 3.0f, 0.6f };
	double alpha = 0.0;
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

	virtual void on_create() override;
	virtual void on_destroy() override;
	virtual void on_update(double frame_advance) override;
	virtual void on_update_later(double frame_advance) override;
	virtual void on_render(HDC canvas) override;

private:
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

		virtual void on_update(double frame_advance) override;

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

};

class oPlayerPoke : public oGraviton {

};

class oEnemyPoke : public oGraviton {

};
