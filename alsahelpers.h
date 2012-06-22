#ifndef __ALSA_HELPERS__
#define __ALSA_HELPERS__

#include <alsa/asoundlib.h>

int setup_hardware(snd_pcm_t *handle, snd_pcm_hw_params_t *params, int * dir);

#endif
