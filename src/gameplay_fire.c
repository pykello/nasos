#include <nasos.h>
#include <stdlib.h>

static void update_fire(struct fire_data *fire, int height);

void update_fires(struct game_data *game)
{
	int i = 0;
	struct spaceship_data *player = &game->player;

	update_fire(&game->player_fire, game->height);

	for (i = 0; i < FIRES_MAX; i++)
		update_fire(&game->enemy_fires[i], game->height);

	if (player->state != STATE_DEAD && player->state != STATE_DYING) {
		if (game->player_fire.active)
			player->frame = 1;
		else
			player->frame = 0;
	}
}

static void update_fire(struct fire_data *fire, int height)
{
	if (!fire->active)
		return;

	fire->y += fire->speed * fire->dy;
	fire->center.y = (int) fire->y;

	if (fire->center.y < 5 || fire->center.y > height - 5)
		fire->active = 0;
}
