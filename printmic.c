#include "alsahelpers.h"
#include <stdio.h>
#include <stdlib.h>

void die(char * s, int rc){
	fprintf(stderr, "%s: %s\n", s, snd_strerror(rc));
	exit(1);
}

int main(int argc, char *argv[]){
	int rc;
	snd_pcm_t *handle;
	snd_pcm_hw_params_t *params;
	int dir;
	snd_pcm_uframes_t frames;
	short *buffer;
	int i;

	if(argc < 2)
		rc = snd_pcm_open(&handle, "default", SND_PCM_STREAM_CAPTURE, 0);
	else rc = snd_pcm_open(&handle, argv[1], SND_PCM_STREAM_CAPTURE, 0);

	if(rc < 0)
		die("Could not open pcm device", rc);

	snd_pcm_hw_params_alloca(&params);

	rc = setup_hardware(handle, params, &dir);
	
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
