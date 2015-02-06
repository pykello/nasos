#ifndef NASOS_H
#define NASOS_H

#include <SDL2/SDL.h>

#define PATH_MAX 4096
#define ENEMIES_MAX 100
#define ENEMY_DX_DEFAULT 5

struct shot_data {
	SDL_Point position;
	SDL_Point direction;
};

enum spaceship_animation {
	NON_ATTACKING_1 = 0,
	NON_ATTACKING_2 = 1,
	SPACESHIP = 2,
	ATTACKING
};

static SDL_Rect const enemy_sprite_rect[][10] = {
	[SPACESHIP] = {
		{.x = 0, .y = 0, .w = 36, .h = 44},
		{.x = 0, .y = 0, .w = 0, .h = 0}
	},
	[NON_ATTACKING_1] = {
		{.x = 0, .y = 0, .w = 30, .h = 22},
		{.x = 34, .y = 0, .w = 30, .h = 22},
		{.x = 70, .y = 0, .w = 30, .h = 22},
		{.x = 0, .y = 0, .w = 0, .h = 0}
	},
	[NON_ATTACKING_2] = {
		{.x = 0, .y = 0, .w = 29, .h = 32},
		{.x = 0, .y = 0, .w = 0, .h = 0}
	},
	[ATTACKING] = {
		{.x = 0, .y = 0, .w = 0, .h = 0}
	}
};

struct spaceship_data {
	SDL_Point center;
	struct shot_data shot;
	int image;
	int frame;
	int animation;
};

struct game_data {
	struct spaceship_data spaceship;
	struct spaceship_data enemies[ENEMIES_MAX];
	int enemy_minx;
	int enemy_maxx;
	int enemy_dx;
	int enemy_count;
	int done;
	int width;
	int height;
};

enum {
	IMAGE_SHIP = 0,
	IMAGE_ENEMY1A,
	IMAGE_ENEMY2A,
	IMAGE_ENEMY3A,
	IMAGE_ENEMY4A,
	IMAGE_COUNT
};

enum {
	TIMER_ENEMY_ANIMATION = 0,
	TIMER_COUNT
};

static int const timer_duration[] = {
	[TIMER_ENEMY_ANIMATION] = 250
};

static char * const image_filename[] = {
	[IMAGE_SHIP] = "ship.bmp",
	[IMAGE_ENEMY1A] = "enemy1a.bmp",
	[IMAGE_ENEMY2A] = "enemy2a.bmp",
	[IMAGE_ENEMY3A] = "enemy3a.bmp",
	[IMAGE_ENEMY4A] = "enemy4a.bmp"
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