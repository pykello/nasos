#include <nasos.h>

struct game_data * game_init(void)
{
	struct game_data *game = malloc(sizeof(struct game_data));
	game->done = 0;
	game->spaceship = (struct spaceship_data) {
		.position = {.x = 200, .y = 440}
	};

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
		ship->position.x -= 5;
		break;
	case SDLK_RIGHT:
		ship->position.x += 5;
		break;
	}
}
