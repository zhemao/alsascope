#include <alsa/asoundlib.h>
#include <stdio.h>
#include <stdlib.h>

void die(char * s, int rc){
	fprintf(stderr, "%s: %s\n", s, snd_strerror(rc));
	exit(1);
}

int main(void){
	int rc;
	snd_pcm_t *handle;
	snd_pcm_hw_params_t *params;
	unsigned int val;
	int dir;
	snd_pcm_uframes_t frames;
	short *buffer;
	int i;

	rc = snd_pcm_open(&handle, "default", SND_PCM_STREAM_CAPTURE, 0);

	if(rc < 0)
		die("Could not open pcm device", rc);

	snd_pcm_hw_params_any(handle, params);

	snd_pcm_hw_params_set_access(handle, params, 
								 SND_PCM_ACCESS_RW_INTERLEAVED);

	snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);

	snd_pcm_hw_params_set_channels(handle, params, 1);

	val = 44100;
	snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir);

	frames = 32;
	snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);

	rc = snd_pcm_hw_params(handle, params);
	
	if(rc < 0)
		die("Could not set hardware parameters", rc);

	snd_pcm_hw_params_get_period_size(params, &frames, &dir);
		
	buffer = (short*) calloc(frames, sizeof(short));

	while(1){
		rc = snd_pcm_readi(handle, buffer, frames);
		if( rc == -EPIPE )
			fprintf(stderr, "buffer overrun\n");
		else if( rc < 0 )
			fprintf(stderr, "read error: %s\n", snd_strerror(rc));
		else {
			for(i=0; i < rc; i++){
				printf("%d\n", buffer[i]);
			}
		}
	}

	snd_pcm_drain(handle);
	snd_pcm_close(handle);

	free(buffer);

	return 0;
}
