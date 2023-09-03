#include<stdlib.h>
#include<stdio.h>

#include <portaudio.h>

static void checkErr(PaError err) {
	if (err != paNoError)
	{
		printf("PortAudio error: %s\n", Pa_GetErrorText(err));
		exit(EXIT_FAILURE);
	}
}

int main()
{
	PaError err;
	err = Pa_Initialize();
	checkErr(err);

	return EXIT_SUCCESS;
}