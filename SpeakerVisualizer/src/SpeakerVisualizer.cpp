#include <chrono>
#include <thread>

#include "audio_displayer.h"

constexpr int sleep_time = 20;

int main()
{
	AudioDisplayer printer;

	bool continue_app = printer.init_successful();

	while (continue_app)
	{
		continue_app = printer.update();
		std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
	}

	return 0;
}