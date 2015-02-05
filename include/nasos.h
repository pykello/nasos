#ifndef NASOS_H
#define NASOS_H

#include <SDL2/SDL.h>

#define PATH_MAX 4096
#define ENEMIES_MAX 40

struct shot_data {
	SDL_Point position;
	SDL_Point direction;
};

struct spaceship_data {
	SDL_Rect rect;
	struct shot_data shot;
	int frame;
};

enum enemy_state {
	NON_ATTACKING = 0,
	ATTACKING
};

static SDL_Rect const enemy_sprite_rect[][10] = {
	[NON_ATTACKING] = {
		{.x = 0, .y = 0, .w = 45, .h = 33},
		{.x = 51, .y = 0, .w = 46, .h = 33},
		{.x = 104, .y = 0, .w = 46, .h = 33},
		{.x = 0, .y = 0, .w = 0, .h = 0}
	},
	[ATTACKING] = {
		{.x = 0, .y = 0, .w = 0, .h = 0}
	}
};

struct game_data {
	struct spaceship_data spaceship;
	struct spaceship_data enemies[ENEMIES_MAX];
	int enemy_count;
	int done;
	int width;
	int height;
};

enum {
	IMAGE_SHIP = 0,
	IMAGE_ENEMY1A,
	IMAGE_COUNT
};

enum {
	TIMER_ENEMY_ANIMATION = 0,
	TIMER_COUNT
};

static int const timer_duration[] = {
	[TIMER_ENEMY_ANIMATION] = 400
};

static char * const image_filename[] = {
	[IMAGE_SHIP] = "ship.bmp",
	[IMAGE_ENEMY1A] = "enemy1a.bmp"
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
	int id;
	int duration;
	int last_tick;
};

/* gameplay.c */
struct game_data * game_init(void);
void game_destroy(struct game_data *);
int game_done(struct game_data *);
void game_handle_keypress(struct game_data *, int);
void game_handle_timer(struct game_data *, int);

/* display.c */
struct display_data * display_init(struct game_data *);
void display_destroy(struct display_data *);
void display_render(struct display_data *, struct game_data *);

/* input.c */
struct input_data * input_init(void);
void input_destroy(struct input_data *);
void input_dispatch_events(struct input_data *, struct game_data *);

/* timer.c */
struct timer_data * timer_init(int, int);
void timer_destroy(struct timer_data *);
void timer_dispatch_events(struct timer_data *, struct game_data *);

#endif