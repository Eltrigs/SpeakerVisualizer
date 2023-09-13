#include <chrono>
#include <thread>

#include "audio_displayer.h"

constexpr int sleep_time_us = 100;

int main()
{
	AudioDisplayer displayer = AudioDisplayer();

	bool continue_app = displayer.init_successful();

	while (continue_app)
	{
		continue_app = displayer.update();
		std::this_thread::sleep_for(std::chrono::microseconds(sleep_time_us));
	}

	return 0;
}