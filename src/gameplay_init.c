#include <nasos.h>
#include <stdlib.h>

void reset_game(struct game_data *game)
{
	game->lifes = 3;
	game->done = 0;
	game->width = GAME_W;
	game->height = GAME_H;
	game->player_fire = (struct fire_data) {
		.active = 0
	};

	reset_player(&game->player);
	reset_enemies(game);
	memset(game->enemy_fires, 0, sizeof(game->enemy_fires));
}

void reset_player(struct spaceship_data *player)
{
	(*player) = (struct spaceship_data) {
		.center = {
			.x = (GAME_W - PLAYER_W) / 2,
			.y = PLAYER_X
		},
		.image = IMAGE_SHIP,
		.frame = 0,
		.animation = ANIM_SPACESHIP,
		.rotation = 0.0
	};
}

void reset_enemies(struct game_data *game)
{
	int i = 0;
	int j = 0;
	int enemy_count = 0;

	/* 1st row */
	for (i = 0; i < 2; i++) {
		game->enemies[enemy_count++] = (struct spaceship_data) {
			.center = {
				.x = ENEMY_X_MIN + (3 + 3 * i) * ENEMY_COL_DIST,
				.y = ENEMY_Y_MIN
			},
			.image = IMAGE_ENEMY4A,
			.animation = ANIM_NON_ATTACKING_2,
			.frame = 0,
			.state = STATE_WAITING,
			.rotation = 0.0
		};
	}

	/* 2nd row */
	for (i = 0; i < 6; i++) {
		game->enemies[enemy_count++] = (struct spaceship_data) {
			.center = {
				.x = ENEMY_X_MIN + (2 + i) * ENEMY_COL_DIST,
				.y = ENEMY_Y_MIN + ENEMY_ROW_DIST
			},
			.image = IMAGE_ENEMY3A,
			.animation = ANIM_NON_ATTACKING_1,
			.frame = rand() % 3,
			.state = STATE_WAITING,
			.rotation = 0.0
		};
	}

	/* 3rd row */
	for (i = 0; i < 8; i++) {
		game->enemies[enemy_count++] = (struct spaceship_data) {
			.center = {
				.x = ENEMY_X_MIN + (1 + i) * ENEMY_COL_DIST,
				.y = ENEMY_Y_MIN + 2 * ENEMY_ROW_DIST
			},
			.image = IMAGE_ENEMY2A,
			.animation = ANIM_NON_ATTACKING_1,
			.frame = rand() % 3,
			.state = STATE_WAITING,
			.rotation = 0.0
		};
	}

	/* last 3 rows */
	for (j = 0; j < 3; j++) {
		for (i = 0; i < 10; i++) {
			game->enemies[enemy_count++] = (struct spaceship_data) {
				.center = {
					.x = ENEMY_X_MIN + ENEMY_COL_DIST * i,
					.y = ENEMY_Y_MIN + (3 + j) * ENEMY_ROW_DIST
				},
				.image = IMAGE_ENEMY1A,
				.animation = ANIM_NON_ATTACKING_1,
				.frame = rand() % 3,
				.state = STATE_WAITING,
				.rotation = 0.0
			};
		}
	}

	for (i = 0; i < enemy_count; i++)
		game->enemies[i].waiting_center = game->enemies[i].center;

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

	game->enemy_dx = -WAITING_ENEMY_SPEED;
}
