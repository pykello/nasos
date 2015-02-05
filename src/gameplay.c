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
		.rect = {.x = 305, .y = 530, .w = 53, .h = 65}
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
		if (ship->rect.x > 15)
			ship->rect.x -= 5;
		break;
	case SDLK_RIGHT:
		if (ship->rect.x + ship->rect.w < game->width - 15)
			ship->rect.x += 5;
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
			game->enemies[i].frame = (game->enemies[i].frame + 1) % 3;
		}

		break;
	}
	}
}

static void init_enemies(struct game_data *game)
{
	int i = 0;

	game->enemy_count = 10;
	for (i = 0; i < game->enemy_count; i++) {
		game->enemies[i] = (struct spaceship_data) {
			.rect = {.x = 30 + 65 * i, .y = 100, .w = 46, .h = 33},
			.frame = rand() % 3
		};
	}
}
