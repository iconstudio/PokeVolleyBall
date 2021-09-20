#include "pch.h"
#include "Core.h"


GameInstance::GameInstance(GameScene* nclan, double nx, double ny)
	: parent(), room(nclan), sprite_index(nullptr), box{ 0, 0, 0, 0 }
	, x(nx), y(ny), image_index(0.0), image_speed(0.0)
	, image_xscale(1.0), image_yscale(1.0), image_angle(0.0) {}

GameInstance::~GameInstance() {
	if (room) {
		room->instance_uninstall(this);
		room = nullptr;
	}
	if (sprite_index)
		sprite_index.reset();
}

void GameInstance::sprite_set(shared_ptr<GameSprite>& sprite) {
	sprite_index = sprite;
	CopyRect(&box, &(sprite->bbox));
}

void GameInstance::collision_update() {
	if (sprite_index) {
		//RectInRegion
	}
}

bool GameInstance::collide_with(shared_ptr<GameInstance>& other) {
	if (sprite_index && other->sprite_index) {
		auto& otherbox = other->box;
		OffsetRect(&otherbox, other->x, other->y);
		OffsetRect(&box, x, y);

		RECT temp;
		bool result = (bool)IntersectRect(&temp, &box, &otherbox);

		OffsetRect(&otherbox, -(other->x), -(other->y));
		OffsetRect(&box, -x, -y);

		return result;
	}

	return false;
}

void GameInstance::on_create() {}

void GameInstance::on_destroy() {}

void GameInstance::on_update(double frame_advance) {}

void GameInstance::on_update_later(double frame_advance) {
	if (sprite_index) {
		double animation_speed;
		auto image_number = sprite_index->number;

		if (1 < image_number && 0.0 != (animation_speed = image_speed * frame_advance)) {
			image_index += animation_speed;

			// clipping
			while (image_index < 0) image_index += image_number;
			while (image_number <= image_index) image_index -= image_number;
		}
	}
}

void GameInstance::on_render(HDC canvas) {
	if (sprite_index) {
		sprite_index->draw(canvas, x, y, image_index, image_angle, image_xscale, image_yscale);
	}
}
