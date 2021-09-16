#pragma once
#include "resource.h"
#include "Framework.h"
#include "Phaser.h"
#include "Sprite.h"
#include "Core.h"


#define RENDER_TIMER_ID 0


// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);


template<class GInstance>
auto instance_create(double x, double y);


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


class oGraviton : public GameInstance {
	public:
		// 재사용 타입
		using type = oGraviton;
		using parent = GameInstance;

		double hspeed, vspeed, gravity;


};

class oValleyBall;
class oPokemon;
class oPlayerPoke;
class oEnemyPoke;

