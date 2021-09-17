#pragma once
#include "resource.h"
#include "pch.h"
#include "Sprite.h"


// 추상 게임 컴포넌트
class GameBehavior {
public:
	// 재사용 타입
	using type = GameBehavior;

	GameBehavior();
	GameBehavior(type&) = default;
	virtual ~GameBehavior() = 0;

	virtual void on_create() = 0;
	virtual void on_destroy() = 0;
	virtual void on_update(double) = 0;
	virtual void on_update_later(double) = 0;
	virtual void on_render(HDC) = 0;

	int data;
	bool done;
};

// 추상 개체 컴포넌트
class GameInstance : public GameBehavior {
public:
	const long my_serial = 0L;
	static constexpr long serial = 0L;

	// 재사용 타입
	using type = GameInstance;
	using parent = GameBehavior;

	GameInstance(GameScene*, double = 0.0, double = 0.0);
	GameInstance(type&) = default;
	virtual ~GameInstance();

	virtual void signout();
	virtual void signin(GameScene*);

	virtual void on_create() override;
	virtual void on_destroy() override;
	virtual void on_update(double) override;
	virtual void on_update_later(double) override; // animation
	virtual void on_render(HDC) override;

	UINT sprite_index, image_number;
	double x, y, image_index, image_speed;

	RECT box; // 충돌체
	GameScene* room; // 소속 장면
};

// 추상 장면 컴포넌트
class GameScene : public GameBehavior {
private:
	using GameInstancePtr = shared_ptr<GameInstance>;

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

public:
	// 재사용 타입
	using type = GameScene;
	using parent = GameBehavior;

	GameScene() : parent(), instances{}, instance_garages{} {}

	GameScene(type&) = default;

	virtual ~GameScene() {
		instances.clear();
		instance_garages.clear();
	}

	template<class _GObj>
	void instance_add(_GObj* rptr) {
		rptr->signout();
		rptr->signin(this);
	}

	template<class _GObj>
	void instance_add(_GObj& rptr) {
		rptr.signout();
		rptr.signin(this);
	}

	template<class _GObj>
	auto instance_create(const double x, const double y) -> GameInstance* {
		auto lptr = new _GObj(this, x, y);
		instance_install(lptr);
		constexpr long temp_serial2 = _GObj::serial;
		
		return lptr;
	}

	auto instance_id(const size_t index) -> GameInstance* {
		auto it = instance_garages.begin();
		for (int i = 0; i < index; ++i) {
			++it;
		}

		return (it->second).get();
	}

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
		if (!instances.empty()) {
			for (const auto& instance : instances)
				instance->on_create();
		}
	}

	void on_destroy() override {
		if (!instances.empty()) {
			for (const auto& instance : instances)
				instance->on_destroy();
		}
	}

	void on_update(double frame_advance) override {
		if (!instances.empty()) {
			for (const auto& instance : instances)
				instance->on_update(frame_advance);
		}
	}

	void on_update_later(double frame_advance) override {
		if (!instances.empty()) {
			for (const auto& instance : instances)
				instance->on_update_later(frame_advance);
		}
	}

	void on_render(HDC canvas) override {
		if (!instances.empty()) {
			for (const auto& instance : instances)
				instance->on_render(canvas);
		}
	}

	friend class GameInstance;
private:
	vector<GameInstancePtr> instances;
	multimap<long, GameInstancePtr> instance_garages;
	using instances_iterator = multimap<long, GameInstancePtr>::iterator;
};
