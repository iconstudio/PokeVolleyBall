#include "pch.h"
#include "Phaser.h"


Phaser::Phaser(const float&& duration)
	: phase(0), ratio(0.0f), time(0.0f), my_size(1), my_durations(duration) {}

Phaser::Phaser(const std::initializer_list<float>&& durations)
	: phase(0), ratio(0.0f), time(0.0f), my_size(durations.size()), my_durations(durations) {}

void Phaser::update(float frame_advance) {
	auto limit = my_durations[phase];
	ratio = (time / limit);

	if (time < limit) { // stuck one time
		time += frame_advance;
		if (limit < time)
			time = limit;
	} else if (phase < my_size - 1) {
		ratio = 0.0f;
		time = 0.0f;
		phase++;
	} else {
		ratio = 1.0f;
		done = true;
	}
}

float Phaser::get() const {
	return ratio;
}

unsigned int Phaser::get_phase() const {
	return phase;
}

bool Phaser::get_done() const {
	return done;
}
