#pragma once
#include "resource.h"
#include "Framework.h"
#include "Phaser.h"
#include "Sprite.h"
#include "Core.h"


constexpr double GRAVITY = px_per_sec(0.098);

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

class GCamera {
public:
	GCamera();
	~GCamera();

	void on_update_later(HWND);

	bool canmove = true;

	static long width, height;		// 뷰의 크기
	RECT* rect;						// 뷰의 사각형
	double x, y;					// 뷰의 좌표
	double destx, desty;			// 뷰의 목표 좌표
	double speed = 10;				// 뷰의 속도 (-1 이면 바로 이동)

	double hspeed, vspeed;			// 뷰의 속도 (종속 변수)
};


// 로고 도입부
class sceneIntro : public GameScene {
public:
	// 재사용 타입
	using type = sceneIntro;
	using parent = GameScene;

	sceneIntro();
	sceneIntro(type&) = default;
	virtual ~sceneIntro();

private:
	Phaser state{ 0.4f, 3.0f, 0.6f };
	double alpha = 0.0;
};

// 타이틀
class sceneTitle : public GameScene {
public:
	// 재사용 타입
	using type = sceneTitle;
	using parent = GameScene;

	sceneTitle();
	sceneTitle(type&) = default;
	virtual ~sceneTitle();

private:

};

// 주 메뉴
class sceneMainMenu : public GameScene {
public:
	// 재사용 타입
	using type = sceneMainMenu;
	using parent = GameScene;

	sceneMainMenu();
	sceneMainMenu(type&) = default;
	virtual ~sceneMainMenu();

private:

};

// 게임 준비
class sceneGameReady : public GameScene {
public:
	// 재사용 타입
	using type = sceneGameReady;
	using parent = GameScene;

	sceneGameReady();
	sceneGameReady(type&) = default;
	virtual ~sceneGameReady();

private:

};

// 게임 중
class sceneGame : public GameScene {
public:
	// 재사용 타입
	using type = sceneGame;
	using parent = GameScene;

	sceneGame();
	sceneGame(type&) = default;
	virtual ~sceneGame();

	virtual void on_create() override;
	virtual void on_destroy() override;
	virtual void on_update(double frame_advance) override;
	virtual void on_update_later(double frame_advance) override;
	virtual void on_render(HDC canvas) override;

private:

};

// 게임 일시 정지
class sceneGamePaused : public GameScene {
public:
	// 재사용 타입
	using type = sceneGamePaused;
	using parent = GameScene;

	sceneGamePaused();
	sceneGamePaused(type&) = default;
	virtual ~sceneGamePaused();

private:

};

// 게임 완수
class sceneGameComplete : public GameScene {
public:
	// 재사용 타입
	using type = sceneGameComplete;
	using parent = GameScene;

	sceneGameComplete();
	sceneGameComplete(type&) = default;
	virtual ~sceneGameComplete();

private:

};

// 점수 정산
class sceneScoring : public GameScene {
public:
	// 재사용 타입
	using type = sceneScoring;
	using parent = GameScene;

	sceneScoring();
	sceneScoring(type&) = default;
	virtual ~sceneScoring();

private:

};

// 설정
class sceneSetting : public GameScene {
public:
	// 재사용 타입
	using type = sceneSetting;
	using parent = GameScene;

	sceneSetting();
	sceneSetting(type&) = default;
	virtual ~sceneSetting();

private:

};

class oSolid : public GameInstance {
public:
	const long my_serial = 2L;
	static constexpr long serial = 2L;

	// 재사용 타입
	using type = oSolid;
	using parent = GameInstance;
};

// 중력 개체
class oGraviton : public GameInstance {
	public:
		const long my_serial = 1L;
		static constexpr long serial = 1L;

		// 재사용 타입
		using type = oGraviton;
		using parent = GameInstance;

		oGraviton(GameScene* nclan, double nx, double ny);
		oGraviton(type&) = default;

		virtual void on_create() override;
		virtual void on_destroy() override;
		virtual void on_update(double frame_advance) override;
		virtual void on_render(HDC canvas) override;

		double hspeed, vspeed, gravity;

};

class oValleyBall : public oGraviton {
public:
	const long my_serial = 3L;
	static constexpr long serial = 3L;

	// 재사용 타입
	using type = oValleyBall;
	using parent = oGraviton;

	oValleyBall(GameScene* nclan, double nx, double ny);
	oValleyBall(type&) = default;


};

class oPokemon;
class oPlayerPoke;
class oEnemyPoke;
