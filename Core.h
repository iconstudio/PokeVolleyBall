#pragma once
#include "resource.h"
#include "pch.h"
#include "Sprite.h"


// 기본 컴포넌트
class GameBehavior {
public:
	virtual ~GameBehavior() {};
	virtual void on_create() {};
	virtual void on_destroy() {};
	virtual void on_update(double) {};
	virtual void on_update_later(double) {};
	virtual void on_render(HDC) {};
};

// 개체 컴포넌트
class GameInstance : public GameBehavior {
public:
	GameInstance(GameScene*, double = 0.0, double = 0.0);
	virtual ~GameInstance();
	virtual void on_update(double frame_advance) override;
	virtual void on_update_later(double) override;
	virtual void on_render(HDC) override;

	void sprite_init(shared_ptr<GameSprite>& sprite);
	void sprite_set(shared_ptr<GameSprite>& sprite);
	bool collide_with(GameInstance*& other);

	void set_xborder(const double min, const double max);
	virtual void move(const double mx, const double my);
	virtual void jump(double power);
	virtual void contact_x_left();
	virtual void contact_x_right();
	virtual void contact_y();
	virtual void thud_left();
	virtual void thud_right();
	virtual void thud();

	shared_ptr<GameSprite> sprite_index; // 스프라이트
	RECT box; // 충돌체
	double image_index, image_speed; // 애니메이션
	double image_xscale, image_yscale, image_angle, image_alpha;
	double x, y; // 좌표
	double x_min, x_max;
	double hspeed, vspeed, gravity;

	GameScene* room; // 소속 장면
};

// 장면 컴포넌트
class GameScene : public GameBehavior {
public:
	GameScene()
		: GameBehavior(), done(false), awaken (false), persistent(false), instances{} {}

	virtual ~GameScene() {
		instances.clear();
	}

	void on_create() override {
		if (!awaken) {
			if (!instances.empty()) {
				for (auto& instance : instances)
					instance->on_create();
			}
			awaken = true;
		}
	}

	void on_destroy() override {
		if (!instances.empty()) {
			for (auto& instance : instances) {
				instance.reset();
			}
		}
		instances.clear();
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

	virtual void reset() {
		done = false;
		awaken = false;
		on_destroy();
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
			target->on_destroy();
			instances.erase(loc);
			delete target;
		}
	}

	friend class GameInstance;
	bool done, awaken;
	bool persistent;

private:

	vector<shared_ptr<GameInstance>> instances;
	//multimap<long, shared_ptr<GameInstance>> instance_garages;
};
