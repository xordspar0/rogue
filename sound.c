#include <portaudio.h>
#include <opusfile.h>

#include "sound.h"

static OggOpusFile *sound_file;
static PaStream *stream;

static int fill_buffer(const void *input, void *output,
		       unsigned long frameCount,
		       const PaStreamCallbackTimeInfo * timeInfo,
		       PaStreamCallbackFlags statusFlags, void *userData)
{
	// Based on my reading of the documentation, the 3rd argument here should be
	// frameCount, not frameCount * 2. However, the sound is awful when I do that.
	// It sounds like I'm playing sound from an uninitialized buffer. When I
	// multiply by 2 it still sounds pretty bad. It sounds like the waveform is
	// stretched out by 10% or so. But it does sound vaguely like the sound file I
	// want it to play. We need to figure this out later.
	int result =
	    op_read_float_stereo(sound_file, (float *)output, frameCount * 2);
	if (result < 0) {
		fprintf(stderr,
			"Failed to read audio: opusfile error code %d\n",
			result);
		return paAbort;
	}
	return paContinue;
}

int sound_init()
{
	PaError err = Pa_Initialize();
	if (err != paNoError) {
		fprintf(stderr, "Failed to initialize audio: %s\n",
			Pa_GetErrorText(err));
		return 1;
	}

	int file_error;
	char *file_path = "sound/song.opus";
	sound_file = op_open_file(file_path, &file_error);
	if (file_error != 0) {
		switch (file_error) {
		case OP_EFAULT:
			fprintf(stderr,
				"Failed to open audio file: file not found %s\n",
				file_path);
			break;
		default:
			fprintf(stderr,
				"Failed to open audio file: opusfile error code %d\n",
				file_error);
		}
		return 1;
	}

	err = Pa_OpenDefaultStream(&stream,
				   0,
				   2,
				   paFloat32, 48000,
				   paFramesPerBufferUnspecified, fill_buffer,
				   NULL);

	if (err != paNoError) {
		fprintf(stderr, "Failed to open audio stream: %s\n",
			Pa_GetErrorText(err));
		return 1;
	}

	return 0;
}

void sound_close()
{
	PaError err = Pa_Terminate();
	if (err != paNoError)
		fprintf(stderr, "Failed to close audio stream: %s\n",
			Pa_GetErrorText(err));
}

void sound_play(int song)
{
	PaError err = Pa_StartStream(stream);
	if (err != paNoError)
		fprintf(stderr, "Failed to play audio: %s\n",
			Pa_GetErrorText(err));
}
