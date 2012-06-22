#include "alsahelpers.h"

int setup_hardware(snd_pcm_t *handle, snd_pcm_hw_params_t *params, int * dir){
	snd_pcm_uframes_t frames;
	unsigned int val;

	snd_pcm_hw_params_any(handle, params);

	snd_pcm_hw_params_set_access(handle, params, 
								 SND_PCM_ACCESS_RW_INTERLEAVED);

	snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);

	snd_pcm_hw_params_set_channels(handle, params, 1);


	val = 44100;
	snd_pcm_hw_params_set_rate_near(handle, params, &val, dir);

	frames = 32;
	snd_pcm_hw_params_set_period_size_near(handle, params, &frames, dir);

	return snd_pcm_hw_params(handle, params);
}
