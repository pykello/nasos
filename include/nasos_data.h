#ifndef NASOS_DATA_H
#define NASOS_DATA_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

/* Images */

enum image_id {
	IMAGE_SHIP = 0,
	IMAGE_ENEMY1A,
	IMAGE_ENEMY2A,
	IMAGE_ENEMY3A,
	IMAGE_ENEMY4A,
	IMAGE_PLAYER_FIRE,
	IMAGE_ENEMY_FIRE,
	IMAGE_ENEMY_DYING,
	IMAGE_PLAYER_DYING,
	IMAGE_LIFE,
	IMAGE_COUNT
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
	[IMAGE_PLAYER_DYING] = "ship_dying.bmp",
	[IMAGE_LIFE] = "life.bmp"
};


/* Animations */

enum animation_id {
	ANIM_NON_ATTACKING_1 = 0,
	ANIM_NON_ATTACKING_2 = 1,
	ANIM_SPACESHIP = 2,
	ANIM_ENEMY_DYING = 3,
	ANIM_PLAYER_DYING = 4
};

static SDL_Rect const enemy_sprite_rect[][10] = {
	[ANIM_SPACESHIP] = {
		{.x = 0, .y = 0, .w = 36, .h = 56},
		{.x = 40, .y = 0, .w = 36, .h = 56},
		{.x = 0, .y = 0, .w = 0, .h = 0}
	},
	[ANIM_NON_ATTACKING_1] = {
		{.x = 0, .y = 0, .w = 30, .h = 22},
		{.x = 34, .y = 0, .w = 30, .h = 22},
		{.x = 70, .y = 0, .w = 30, .h = 22},
		{.x = 0, .y = 0, .w = 0, .h = 0}
	},
	[ANIM_NON_ATTACKING_2] = {
		{.x = 0, .y = 0, .w = 29, .h = 32},
		{.x = 0, .y = 0, .w = 0, .h = 0}
	},
	[ANIM_ENEMY_DYING] = {
		{.x = 0, .y = 0, .w = 42, .h = 44},
		{.x = 50, .y = 0, .w = 24, .h = 44},
		{.x = 80, .y = 0, .w = 35, .h = 44},
		{.x = 120, .y = 0, .w = 42, .h = 44},
		{.x = 0, .y = 0, .w = 0, .h = 0}
	},
	[ANIM_PLAYER_DYING] = {
		{.x = 0, .y = 0, .w = 80, .h = 85},
		{.x = 85, .y = 0, .w = 80, .h = 85},
		{.x = 170, .y = 0, .w = 85, .h = 85},
		{.x = 258, .y = 0, .w = 80, .h = 85},
		{.x = 0, .y = 0, .w = 0, .h = 0}
	}
};


/* Sounds */

enum {
	SOUND_BACKGROUND = 0,
	SOUND_PLAYER_FIRE,
	SOUND_ENEMY_FIRE,
	SOUND_PLAYER_EXPLOSION,
	SOUND_ENEMY_EXPLOSION,
	SOUND_ENEMY_JUMPING,
	SOUND_COUNT
};

static char * const sound_filename[] = {
	[SOUND_BACKGROUND] = "background.ogg",
	[SOUND_PLAYER_FIRE] = "player_fire.ogg",
	[SOUND_ENEMY_FIRE] = "enemy_fire.ogg",
	[SOUND_PLAYER_EXPLOSION] = "player_explosion.ogg",
	[SOUND_ENEMY_EXPLOSION] = "enemy_explosion.ogg",
	[SOUND_ENEMY_JUMPING] = "enemy_jumping.ogg"
};


/* Timers */

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

/* Menu Items */

enum {
	MENU_START = 0,
	MENU_ABOUT,
	MENU_QUIT,
	MENU_COUNT
};

static char * const menu_text[] = {
	[MENU_START] = "Start Game",
	[MENU_ABOUT] = "About",
	[MENU_QUIT] = "Quit"
};


/* Fonts */

enum {
	FONT_AVERIA = 0,
	FONT_COUNT
};

static char * const font_filename[] = {
	[FONT_AVERIA] = "averia-regular.ttf"
};

#endif
