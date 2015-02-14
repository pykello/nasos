#include <nasos.h>
#include <stdlib.h>

void update_dying_player(struct game_data *game)
{
	struct spaceship_data *player = &game->spaceship;
	if (player->state != STATE_DYING)
		return;

	player->frame++;
	if (enemy_sprite_rect[player->animation][player->frame].w == 0) {
		player->state = STATE_DEAD;
		game->lifes -= 1;
		if (game->lifes == 0)
			reset_game(game);
		else
			reset_player(player);
	}
}

void player_fire(struct game_data *game)
{
	SDL_Point player_center = game->spaceship.center;
	int x = player_center.x;
	int y = player_center.y - PLAYER_H / 2;

	if (game->spaceship_fire.active)
		return;

	game->spaceship.frame = 1;
	game->spaceship_fire = (struct fire_data) {
		.center = {.x = x, .y = y},
		.y = (double) y,
		.dy = -1,
		.speed = PLAYER_FIRE_SPEED,
		.active = 1,
		.image = IMAGE_PLAYER_FIRE
	};
}

void kill_player(struct game_data *game)
{
	int i = 0;
	int should_die = 0;
	struct spaceship_data *player = &game->spaceship;
	SDL_Rect player_rect = create_rect(player->center, PLAYER_W, PLAYER_H);

	if (player->state == STATE_DEAD || player->state == STATE_DYING)
		return;

	for (i = 0; i < game->enemy_count && !should_die; i++) {
		SDL_Rect enemy_rect = create_rect(game->enemies[i].center,
						  ENEMY_W, ENEMY_H);

		if (game->enemies[i].state == STATE_DEAD ||
		    game->enemies[i].state == STATE_DYING)
			continue;

		if (SDL_HasIntersection(&enemy_rect, &player_rect))
			should_die = 1;
	}

	for (i = 0; i < FIRES_MAX && !should_die; i++) {
		struct fire_data *fire = &game->enemy_fires[i];

		if (fire->active) {
			SDL_Rect fire_rect = create_rect(fire->center, FIRE_W,
							 FIRE_H);
			if (SDL_HasIntersection(&fire_rect, &player_rect))
				should_die = 1;
		}
	}

	if (should_die) {
		player->state = STATE_DYING;
		player->frame = 0;
		player->image = IMAGE_PLAYER_DYING;
		player->animation = ANIM_PLAYER_DYING;
	}
}
