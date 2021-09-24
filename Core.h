#pragma once
#include "resource.h"
#include "pch.h"
#include "Sprite.h"


// 추상 컴포넌트
class GameBehavior {
public:
	virtual ~GameBehavior() {};
	virtual void on_create() {};
	virtual void on_destroy() {};
	virtual void on_update(double) {};
	virtual void on_update_later(double) {};
	virtual void on_render(HDC) {};
};

// 추상 개체 컴포넌트
class GameInstance : public GameBehavior {
public:
	using parent = GameBehavior;

	GameInstance(GameScene*, double = 0.0, double = 0.0);
	virtual ~GameInstance();
	virtual void on_update_later(double) override; // animation
	virtual void on_render(HDC) override;

	void sprite_init(shared_ptr<GameSprite>& sprite);
	void sprite_set(shared_ptr<GameSprite>& sprite);
	bool collide_with(GameInstance*& other);

	double x, y; // 좌표

	shared_ptr<GameSprite> sprite_index; // 스프라이트
	RECT box; // 충돌체
	double image_index, image_speed; // 애니메이션
	double image_xscale, image_yscale, image_angle;

	GameScene* room; // 소속 장면
};


// 추상 장면 컴포넌트
class GameScene : public GameBehavior {
public:
	GameScene() : GameBehavior(), done(false), instances{} {}

	virtual ~GameScene() {
		instances.clear();
	}

	void on_create() override {
		if (!instances.empty()) {
			for (auto& instance : instances)
				instance->on_create();
		}
	}

	void on_destroy() override {
		if (!instances.empty()) {
			for (auto& instance : instances)
				instance->on_destroy();
		}
	}

	void on_update(double frame_advance) override {
		if (!instances.empty()) {
			for (auto& instance : instances)
				instance->on_update(frame_advance);
		}
	}

	void on_update_later(double frame_advance) override {
		if (!instances.empty()) {
			for (auto& instance : instances)
				instance->on_update_later(frame_advance);
		}
	}

	void on_render(HDC canvas) override {
		if (!instances.empty()) {
			for (auto& instance : instances)
				instance->on_render(canvas);
		}
	}

	template<class _GObj>
	auto instance_create(const double x, const double y) {
		auto lptr = new _GObj(this, x, y);
		lptr->room = this;

		instances.push_back(shared_ptr<GameInstance>(lptr));

		return lptr;
	}

	template<class _GObj>
	inline void instance_kill(_GObj* target) {
		auto loc = find_if(instances.begin(), instances.end(), [target](shared_ptr<GameInstance>& lhs) {
			return (lhs.get() == target);
		});

		if (loc != instances.end()) {
			instances.erase(loc);
		}
	}

	friend class GameInstance;
	bool done;

private:

	vector<shared_ptr<GameInstance>> instances;
	//multimap<long, shared_ptr<GameInstance>> instance_garages;
};
