#ifndef NASOS_H
#define NASOS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#define PATH_MAX 4096
#define ENEMIES_MAX 100
#define FIRES_MAX 20
#define ENEMY_DX_DEFAULT 5
#define ROTATION_COUNT 48
#define STAR_COUNT 128
#define STAR_STEPS_MAX 1024
#define STAR_SIZE_MAX 3

#define PI 3.1415
#define EPS 1e-5

struct fire_data {
	int active;
	SDL_Point center;
	double dy;
	double y;
	double speed;
	int image;
};

struct star_data {
	SDL_Rect rect;
	int step;
};

enum spaceship_state {
	WAITING,
	JUMPING,
	DYING,
	DEAD,
	RESTORING
};

enum spaceship_animation {
	NON_ATTACKING_1 = 0,
	NON_ATTACKING_2 = 1,
	SPACESHIP = 2,
	ENEMY_DYING = 3,
	PLAYER_DYING = 4,
	ATTACKING
};

static SDL_Rect const enemy_sprite_rect[][10] = {
	[SPACESHIP] = {
		{.x = 0, .y = 0, .w = 36, .h = 56},
		{.x = 40, .y = 0, .w = 36, .h = 56},
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
	},
	[ENEMY_DYING] = {
		{.x = 0, .y = 0, .w = 42, .h = 44},
		{.x = 50, .y = 0, .w = 24, .h = 44},
		{.x = 80, .y = 0, .w = 35, .h = 44},
		{.x = 120, .y = 0, .w = 42, .h = 44},
		{.x = 0, .y = 0, .w = 0, .h = 0}
	},
	[PLAYER_DYING] = {
		{.x = 0, .y = 0, .w = 80, .h = 85},
		{.x = 85, .y = 0, .w = 80, .h = 85},
		{.x = 170, .y = 0, .w = 85, .h = 85},
		{.x = 258, .y = 0, .w = 80, .h = 85},
		{.x = 0, .y = 0, .w = 0, .h = 0}
	}
};

struct spaceship_data {
	SDL_Point center;
	SDL_Point waiting_center;
	int image; /* IMAGE_SHIP, etc. */
	int frame; /* which frame in image */
	int animation; /* how to divide images into frames */
	int state;
	double jump_degree;
	double jump_degree_delta;
	double jump_x;
	double jump_y;
	double jump_speed;
	double rotation;
	int jump_steps;
};

struct game_data {
	struct spaceship_data spaceship;
	struct spaceship_data enemies[ENEMIES_MAX];
	struct fire_data spaceship_fire;
	struct fire_data enemy_fires[FIRES_MAX];
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
	IMAGE_PLAYER_FIRE,
	IMAGE_ENEMY_FIRE,
	IMAGE_ENEMY_DYING,
	IMAGE_PLAYER_DYING,
	IMAGE_COUNT
};

enum {
	SOUND_BACKGROUND = 0,
	SOUND_PLAYER_FIRE,
	SOUND_ENEMY_FIRE,
	SOUND_PLAYER_EXPLOSION,
	SOUND_COUNT
};

enum {
	TIMER_ENEMY_ANIMATION = 0,
	TIMER_ENEMY_DYING,
	TIMER_ENEMY_JUMP,
	TIMER_FIRE,
	TIMER_COUNT
};

static int const timer_duration[] = {
	[TIMER_ENEMY_ANIMATION] = 250,
	[TIMER_ENEMY_DYING] = 70,
	[TIMER_ENEMY_JUMP] = 40,
	[TIMER_FIRE] = 40
};

static char * const image_filename[] = {
	[IMAGE_SHIP] = "ship.bmp",
	[IMAGE_ENEMY1A] = "enemy1a.bmp",
	[IMAGE_ENEMY2A] = "enemy2a.bmp",
	[IMAGE_ENEMY3A] = "enemy3a.bmp",
	[IMAGE_ENEMY4A] = "enemy4a.bmp",
	[IMAGE_PLAYER_FIRE] = "ship_fire.bmp",
	[IMAGE_ENEMY_FIRE] = "enemy_fire.bmp",
	[IMAGE_ENEMY_DYING] = "enemy_dying.bmp",
	[IMAGE_PLAYER_DYING] = "ship_dying.bmp"
};

static char * const sound_filename[] = {
	[SOUND_BACKGROUND] = "background.ogg",
	[SOUND_PLAYER_FIRE] = "player_fire.ogg",
	[SOUND_ENEMY_FIRE] = "enemy_fire.ogg",
	[SOUND_PLAYER_EXPLOSION] = "player_explosion.ogg"
};

struct display_data {
	SDL_Window *window;
	SDL_Surface *images[IMAGE_COUNT];
	SDL_Surface *rotations[IMAGE_COUNT][ROTATION_COUNT];
	struct star_data stars[STAR_COUNT];
};

enum input_source {
	KEYBOARD
};

struct input_data {
	enum input_source source;
	unsigned int last_arrow_report[2];
};

struct timer_data {
	int id;
	int duration;
	int last_tick;
};

struct mixer_data {
	int enabled;
	int background_started;
	Mix_Chunk *sounds[SOUND_COUNT];

	int player_fire_active;
	int enemy_fire_active[FIRES_MAX];

	int player_state;
	int enemy_state[ENEMIES_MAX];
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
SDL_Rect create_spaceship_rect(struct spaceship_data *ship);
SDL_Rect create_rect(SDL_Point center, int w, int h);

/* input.c */
struct input_data * input_init(void);
void input_destroy(struct input_data *);
void input_dispatch_events(struct input_data *, struct game_data *);

/* timer.c */
struct timer_data * timer_init(int, int);
void timer_destroy(struct timer_data *);
void timer_dispatch_events(struct timer_data *, struct game_data *);

/* mixer.c */
struct mixer_data * mixer_init(void);
void mixer_destroy(struct mixer_data *mixer);
void mixer_update(struct mixer_data *mixer, struct game_data *game);

#endif