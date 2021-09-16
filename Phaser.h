#pragma once


class Phaser {
public:
	Phaser(const float&& duration)
		: phase(0), ratio(0.0f), time(0.0f), my_size(1), my_durations(duration) {}

	Phaser(const std::initializer_list<float>&& durations)
		: phase(0), ratio(0.0f), time(0.0f), my_size(durations.size()), my_durations(durations) {}

	void update(float frame_advance) {
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

	float get() const {
		return ratio;
	}

	unsigned int get_phase() const {
		return phase;
	}

	bool get_done() const {
		return done;
	}

private:
	bool done = false;
	unsigned int phase;
	float ratio, time;

	const unsigned int my_size;
	const vector<float> my_durations;
};
