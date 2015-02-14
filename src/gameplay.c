#include <nasos.h>
#include <stdlib.h>

struct game_data * game_init(void)
{
	struct game_data *game = malloc(sizeof(struct game_data));
	reset_game(game);

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

void game_handle_keypress(void *private, int keycode)
{
	struct game_data *game = private;
	struct spaceship_data *player = &game->player;

	switch (keycode) {
	case 'q':
		game->done = 1;
		break;
	case 'r':
		reset_game(game);
		break;
	case ' ':
		player_fire(game);
		break;
	case SDLK_LEFT:
		if (player->center.x > 30)
			player->center.x -= PLAYER_SPEED;
		break;
	case SDLK_RIGHT:
		if (player->center.x < game->width - 30)
			player->center.x += PLAYER_SPEED;
		break;
	}
}

void game_handle_timer(void *private, int timer_id)
{
	struct game_data *game = private;

	switch (timer_id)
	{
	case TIMER_ENEMY_ANIMATION:
		update_waiting_enemies(game);
		break;

	case TIMER_ENEMY_DYING:
		update_dying_enemies(game);
		update_dying_player(game);
		break;

	case TIMER_ENEMY_JUMP:
		update_jumping_enemies(game);
		update_restoring_enemies(game);
		kill_player(game);
		break;

	case TIMER_FIRE:
		update_fires(game);
		kill_enemies(game);
		break;
	}
}
