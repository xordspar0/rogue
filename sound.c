#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SDL.h"
#include "SDL_sound.h"

static const char *filename = "sound/beat.mp3";

typedef struct {
	Sound_Sample *sample;
	SDL_AudioSpec devformat;
	Uint8 *decoded_ptr;
	Uint32 decoded_bytes;
} callback_data;

static void SDLCALL audio_callback(void *userdata, Uint8 * stream, int len)
{
	callback_data *data = (callback_data *) userdata;
	Sound_Sample *sample = data->sample;
	int bw = 0;		/* bytes written to stream this time through the callback */

	while (bw < len) {
		int cpysize;	/* bytes to copy on this iteration of the loop. */

		if (data->decoded_bytes == 0) {	/* need more data! */
			if (((sample->flags & SOUND_SAMPLEFLAG_ERROR) == 0) &&
			    ((sample->flags & SOUND_SAMPLEFLAG_EOF) == 0)) {
				data->decoded_bytes = Sound_Decode(sample);
				data->decoded_ptr = sample->buffer;
			}

			if (data->decoded_bytes == 0) {
				SDL_memset(stream + bw, '\0', len - bw);	/* write silence. */
				return;	/* we're done playback, one way or another. */
			}
		}

		cpysize = len - bw;	/* len - bw == amount device still wants. */
		if (cpysize > data->decoded_bytes)
			cpysize = data->decoded_bytes;	/* clamp to what we have left. */

		if (cpysize > 0) {
			SDL_memcpy(stream + bw, (Uint8 *) data->decoded_ptr,
				   cpysize);

			bw += cpysize;
			data->decoded_ptr += cpysize;
			data->decoded_bytes -= cpysize;
		}
	}
}

int sound_init()
{
	if (!Sound_Init()) {
		fprintf(stderr, "Sound_Init failed: %s.\n", Sound_GetError());
		SDL_Quit();
		return 1;
	}

	return 0;
}

void sound_quit()
{
	Sound_Quit();
	SDL_Quit();
}

void sound_play(int song)
{
	callback_data data;
	SDL_zero(data);
	data.sample = Sound_NewSampleFromFile(filename, NULL, 65536);
	if (data.sample == NULL) {
		fprintf(stderr, "Sound_NewSampleFromFile failed: %s.\n",
			Sound_GetError());
		return;
	}

	data.devformat.freq = data.sample->actual.rate;
	data.devformat.format = data.sample->actual.format;
	data.devformat.channels = data.sample->actual.channels;
	data.devformat.samples = 4096;
	data.devformat.callback = audio_callback;
	data.devformat.userdata = &data;
	if (SDL_OpenAudio(&data.devformat, NULL) < 0) {
		fprintf(stderr, "Couldn't open audio device: %s.\n",
			SDL_GetError());
		Sound_FreeSample(data.sample);
		return;
	}

	SDL_PauseAudio(0);
}
