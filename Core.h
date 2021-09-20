#pragma once
#include "resource.h"
#include "pch.h"
#include "Sprite.h"


// 추상 게임 컴포넌트
class GameBehavior {
public:
	virtual ~GameBehavior() = 0 {};

	virtual void on_create() = 0;
	virtual void on_destroy() = 0;
	virtual void on_update(double) = 0;
	virtual void on_update_later(double) = 0;
	virtual void on_render(HDC) = 0;
};


// 추상 개체 컴포넌트
class GameInstance : public GameBehavior {
public:
	const long my_serial = 0L;
	static constexpr long serial = 0L;
	using parent = GameBehavior;

	GameInstance(GameScene*, double = 0.0, double = 0.0);
	virtual ~GameInstance();

	void sprite_set(shared_ptr<GameSprite>& sprite);
	void collision_update();
	bool collide_with(shared_ptr<GameInstance>& other);

	virtual void on_create() override;
	virtual void on_destroy() override;
	virtual void on_update(double) override;
	virtual void on_update_later(double) override; // animation
	virtual void on_render(HDC) override;

	GameScene* room; // 소속 장면

	RECT box; // 충돌체
	shared_ptr<GameSprite> sprite_index;
	double x, y, image_index, image_speed;

};


// 추상 장면 컴포넌트
class GameScene : public GameBehavior {
public:
	using parent = GameBehavior;

	GameScene() : parent(), done(false), instance_garages{} {}

	virtual ~GameScene() {
		//instances.clear();
		instance_garages.clear();
	}

	template<class _GObj>
	auto instance_create(const double x, const double y) {
		auto lptr = new _GObj(this, x, y);
		instance_install<_GObj>(lptr);
		lptr->room = this;
		
		return lptr;
	}

	// 모든 인스턴스에서 참조
	auto instance_id(const size_t index) {
		auto it = instance_garages.begin();
		for (int i = 0; i < index; ++i) {
			++it;
		}

		return (it->second);
	}

	// 특정한 종류의 인스턴스만 참조
	template<class _GObj>
	auto instance_seek(const size_t index) -> GameInstance* {
		var ranges = instance_garages.equal_range(_GObj::serial);
		var first = ranges.first;
		var second = ranges.second;

		if (first != instance_garages.end()) {
			if (0 < index) {
				for (int i = 0; i < index; ++i) {
					++first;
				}
			}
			return ((first->second).get());
		} else {
			return nullptr;
		}
		//*/
	}

	void on_create() override {
		if (!instance_garages.empty()) {
			for (const auto& instance : instance_garages)
				instance.second->on_create();
		}
	}

	void on_destroy() override {
		if (!instance_garages.empty()) {
			for (const auto& instance : instance_garages)
				instance.second->on_destroy();
		}
	}

	void on_update(double frame_advance) override {
		if (!instance_garages.empty()) {
			for (const auto& instance : instance_garages)
				instance.second->on_update(frame_advance);
		}
	}

	void on_update_later(double frame_advance) override {
		if (!instance_garages.empty()) {
			for (const auto& instance : instance_garages)
				instance.second->on_update_later(frame_advance);
		}
	}

	void on_render(HDC canvas) override {
		if (!instance_garages.empty()) {
			for (const auto& instance : instance_garages)
				instance.second->on_render(canvas);
		}
	}

	friend class GameInstance;

	bool done;

private:
	template<class _GObj>
	void instance_install(_GObj* target) {
		var loc = target->my_serial;
		instance_garages.emplace(loc, shared_ptr<GameInstance>(target));
	}

	template<class _GObj>
	void instance_uninstall(_GObj* target) {
		var loc = target->my_serial;
		var erloc = instance_garages.find(loc);

		instance_garages.erase(erloc);
	}

	//vector<shared_ptr<GameInstance>> instances;
	multimap<long, shared_ptr<GameInstance>> instance_garages;
	using instances_iterator = multimap<long, shared_ptr<GameInstance>>::iterator;
};
