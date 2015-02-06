#include <nasos.h>
#include <stdlib.h>

static void init_enemies(struct game_data *game);

struct game_data * game_init(void)
{
	struct game_data *game = malloc(sizeof(struct game_data));
	game->done = 0;
	game->width = 800;
	game->height = 600;
	game->spaceship = (struct spaceship_data) {
		.center = {.x = 315, .y = 550},
		.image = IMAGE_SHIP,
		.frame = 0,
		.animation = SPACESHIP
	};
	init_enemies(game);

	return game;
}

void game_destroy(struct game_data *game)
{
	free(game);
}

int game_done(struct game_data *game)
{
	return game->done;
}

void game_handle_keypress(struct game_data *game, int keycode)
{
	struct spaceship_data *ship = &game->spaceship;

	switch (keycode) {
	case 'q':
		game->done = 1;
		break;
	case SDLK_LEFT:
		if (ship->center.x > 30)
			ship->center.x -= 5;
		break;
	case SDLK_RIGHT:
		if (ship->center.x < game->width - 30)
			ship->center.x += 5;
		break;
	}
}

void game_handle_timer(struct game_data *game, int timer_id)
{
	switch (timer_id)
	{
	case TIMER_ENEMY_ANIMATION:
	{
		int i = 0;
		for (i = 0; i < game->enemy_count; i++) {
			struct spaceship_data *enemy = &game->enemies[i];

			enemy->frame++;
			if (enemy_sprite_rect[enemy->animation][enemy->frame].w == 0)
				enemy->frame = 0;

			enemy->center.x += game->enemy_dx;
		}

		game->enemy_minx += game->enemy_dx;
		game->enemy_maxx += game->enemy_dx;

		if (game->enemy_minx <= 30) {
			game->enemy_dx = ENEMY_DX_DEFAULT;
		}

		if (game->enemy_maxx >= game->width - 30) {
			game->enemy_dx = -ENEMY_DX_DEFAULT;
		}

		break;
	}
	}
}

static void init_enemies(struct game_data *game)
{
	int i = 0;
	int j = 0;
	int enemy_count = 0;

	/* 1st row */
	for (i = 0; i < 2; i++) {
		game->enemies[enemy_count++] = (struct spaceship_data) {
			.center = {
				.x = 162 + 132 * i, .y = 55
			},
			.image = IMAGE_ENEMY4A,
			.animation = NON_ATTACKING_2,
			.frame = 0
		};
	}

	/* 2nd row */
	for (i = 0; i < 6; i++) {
		game->enemies[enemy_count++] = (struct spaceship_data) {
			.center = {
				.x = 118 + 44 * i, .y = 90
			},
			.image = IMAGE_ENEMY3A,
			.animation = NON_ATTACKING_1,
			.frame = rand() % 3
		};
	}

	/* 3rd row */
	for (i = 0; i < 8; i++) {
		game->enemies[enemy_count++] = (struct spaceship_data) {
			.center = {
				.x = 74 + 44 * i, .y = 125
			},
			.image = IMAGE_ENEMY2A,
			.animation = NON_ATTACKING_1,
			.frame = rand() % 3
		};
	}

	/* last 3 rows */
	for (j = 0; j < 3; j++) {
		for (i = 0; i < 10; i++) {
			game->enemies[enemy_count++] = (struct spaceship_data) {
				.center = {
					.x = 30 + 44 * i, .y = 160 + 35 * j
				},
				.image = IMAGE_ENEMY1A,
				.animation = NON_ATTACKING_1,
				.frame = rand() % 3
			};
		}
	}

	game->enemy_count = enemy_count;

	game->enemy_minx = game->enemies[0].center.x;
	game->enemy_maxx = game->enemies[0].center.x;

	for (i = 0; i < game->enemy_count; i++) {
		SDL_Point center = game->enemies[i].center;

		if (center.x < game->enemy_minx)
			game->enemy_minx = center.x;

		if (center.x > game->enemy_maxx)
			game->enemy_maxx = center.x;
	}

	game->enemy_dx = -ENEMY_DX_DEFAULT;
}
