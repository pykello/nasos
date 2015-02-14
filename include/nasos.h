#ifndef NASOS_H
#define NASOS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <nasos_data.h>

/* Math constants */
#define PI 3.1415
#define EPS 1e-5

/* Size constants */
#define PATH_MAX 4096 /* file path, ... */
#define ENEMIES_MAX 100
#define FIRES_MAX 20

/*
 * We precalculate different rotations of enemy images so we don't have to do
 * this in realtime.
 */
#define ROTATION_COUNT 48

/* Background stars */
#define STAR_COUNT 128
#define STAR_STEPS_MAX 1024
#define STAR_SIZE_MAX 3

/* Speed constants */
#define WAITING_ENEMY_SPEED 5
#define JUMPING_ENEMY_SPEED 10
#define JUMPING_ENEMY_ROT_SPEED (PI / 35)
#define PLAYER_SPEED 8
#define ENEMY_FIRE_SPEED 16
#define PLAYER_FIRE_SPEED 24

/* Game geometry */
#define GAME_W 800
#define GAME_H 800
#define FIRE_W 4
#define FIRE_H 13
#define PLAYER_W 36
#define PLAYER_H 56
#define PLAYER_X 700
#define ENEMY_W 30
#define ENEMY_H 25
#define ENEMY_Y_MIN 100
#define ENEMY_ROW_DIST 35
#define ENEMY_X_MIN 40
#define ENEMY_COL_DIST 44
#define LIFE_LEFT 80
#define LIFE_DIST 5
#define LIFE_BOTTOM 20


/* Game structures, enums, ... */

enum spaceship_state {
	STATE_WAITING,
	STATE_JUMPING,
	STATE_DYING,
	STATE_DEAD,
	STATE_RESTORING
};

/* player or enemy spaceship */
struct spaceship_data {
	SDL_Point center; /* current position of spaceship */

	/*
	 * Enemy spaceships can jump. they should know where they should go back
	 * when their attack is finished. waiting_center represents that place.
	 */
	SDL_Point waiting_center;
	
	enum spaceship_state state; /* STATE_WAITING, etc. */
	enum image_id image; /* IMAGE_SHIP, etc. */
	int frame; /* which frame in image */
	enum animation_id animation; /* how to divide images into frames */
	
	/* jump variables for enemies */
	double jump_degree;  /* current orientation */
	double jump_degree_delta; /* how much should degree change on each update */
	double jump_x;
	double jump_y;
	double jump_speed;
	double rotation;
	int jump_steps;
};

/* player or enemy fire */
struct fire_data {
	int active;
	SDL_Point center;
	double dy;
	double y;
	double speed;
	int image;
};

/* game state */
struct game_data {
	struct spaceship_data player;
	struct spaceship_data enemies[ENEMIES_MAX];
	struct fire_data player_fire;
	struct fire_data enemy_fires[FIRES_MAX];
	int lifes;
	int enemy_minx;
	int enemy_maxx;
	int enemy_dx;
	int enemy_count;
	int done;
	int width;
	int height;
};


/* Display structures */

struct star_data {
	SDL_Rect rect;
	int step;
};

struct display_data {
	SDL_Window *window;
	SDL_Surface *images[IMAGE_COUNT];
	SDL_Surface *rotations[IMAGE_COUNT][ROTATION_COUNT];
	struct star_data stars[STAR_COUNT];
};


/* Input structures */

enum input_source {
	KEYBOARD
};

struct input_data {
	enum input_source source;
	unsigned int last_arrow_report[2];
	void (*handle_input_func)(void *, int);
	void *private;
};


/* Timer structures */

struct timer_data {
	int id;
	int duration;
	int last_tick;
	void (*timer_handle_func)(void *, int);
	void *private;
};


/* Mixer structures */

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
void game_handle_keypress(void *, int);
void game_handle_timer(void *, int);

/* gameplay_init.c */
void reset_game(struct game_data *game);
void reset_player(struct spaceship_data *player);
void reset_enemies(struct game_data *game);

/* gameplay_enemies.c */
void update_waiting_enemies(struct game_data *game);
void update_jumping_enemies(struct game_data *game);
void update_restoring_enemies(struct game_data *game);
void update_dying_enemies(struct game_data *game);
void enemy_fire(struct game_data *game, SDL_Point center);
void kill_enemies(struct game_data *game);

/* gameplay_player.c */
void update_dying_player(struct game_data *game);
void player_fire(struct game_data *game);
void kill_player(struct game_data *game);

/* gameplay_fire.c */
void update_fires(struct game_data *game);

/* display.c */
struct display_data * display_init(struct game_data *);
void display_destroy(struct display_data *);
void display_render(struct display_data *, struct game_data *);
SDL_Rect create_rect(SDL_Point center, int w, int h);

/* input.c */
struct input_data * input_init(void (*)(void *, int), void *);
void input_destroy(struct input_data *);
void input_dispatch_events(struct input_data *);

/* timer.c */
struct timer_data * timer_init(int, int, void (*)(void *, int), void *);
void timer_destroy(struct timer_data *);
void timer_dispatch_events(struct timer_data *);

/* mixer.c */
struct mixer_data * mixer_init(void);
void mixer_destroy(struct mixer_data *mixer);
void mixer_update(struct mixer_data *mixer, struct game_data *game);

#endif