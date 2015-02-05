#ifndef NASOS_H
#define NASOS_H

#include <SDL2/SDL.h>

#define PATH_MAX 4096

struct shot_data {
	SDL_Point position;
	SDL_Point direction;
};

struct spaceship_data {
	SDL_Point position;
	struct shot_data shot;
};

struct game_data {
	struct spaceship_data spaceship;
	int done;
};

enum {
	IMAGE_SHIP = 0,
	IMAGE_ENEMY,
	IMAGE_COUNT
};

static char * const image_filename[] = {
	[IMAGE_SHIP] = "ship.bmp",
	[IMAGE_ENEMY] = "enemy.bmp"
};

struct display_data {
	SDL_Window *window;
	SDL_Surface *images[IMAGE_COUNT];
};

enum input_source {
	KEYBOARD
};

struct input_data {
	enum input_source source;
};

struct timer_data {
	/* TODO */
};

/* gameplay.c */
struct game_data * game_init(void);
void game_destroy(struct game_data *);
int game_done(struct game_data *);
void game_handle_keypress(struct game_data *, int);

/* display.c */
struct display_data * display_init(void);
void display_destroy(struct display_data *);
void display_render(struct display_data *, struct game_data *);

/* input.c */
struct input_data * input_init(void);
void input_destroy(struct input_data *);
void input_dispatch_events(struct input_data *, struct game_data *);

/* timer.c */
struct timer_data * timer_init(void);
void timer_destroy(struct timer_data *);
void timer_dispatch_events(struct timer_data *, struct game_data *);

#endif