#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <limits.h>
#include "alsahelpers.h"

void die(char * s, int rc){
	fprintf(stderr, "%s: %s\n", s, snd_strerror(rc));
	exit(1);
}

void draw_sample(WINDOW *win, int t, int v){
	int width, height;
	int x, y;
	int i;

	getmaxyx(win, height, width);

	x = t % width;
	y = v * height / USHRT_MAX + height / 2;

	for(i=0; i<height; i++)
		mvaddch(i, x, ' ');

	mvaddch(y, x, 'V');
}

int main(int argc, char *argv[]){
	int rc, dir;
	snd_pcm_t *handle;
	snd_pcm_hw_params_t *params;
	snd_pcm_uframes_t frames;
	short *buffer;
	int t = 0, i, c;
	WINDOW *win;

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

	win = initscr();
	nodelay(win, 1);
	noecho();

	while(1){
		c = getch();

		if(c == 'q') 
			break;

		rc = snd_pcm_readi(handle, buffer, frames);

		if( rc == -EPIPE )
			fprintf(stderr, "buffer overrun\n");
		else if( rc < 0 )
			fprintf(stderr, "read error: %s\n", snd_strerror(rc));
		else {
			for(i=0; i < rc; i++){
				draw_sample(win, t, buffer[i]);
				t++;
			}
		}
	}

	endwin();

	snd_pcm_drain(handle);
	snd_pcm_close(handle);

	free(buffer);

	return 0;
}
