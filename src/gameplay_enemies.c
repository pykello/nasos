#include <nasos.h>
#include <stdlib.h>

void update_waiting_enemies(struct game_data *game)
{
	int i = 0;
	for (i = 0; i < game->enemy_count; i++)
		game->enemies[i].waiting_center.x += game->enemy_dx;

	for (i = 0; i < game->enemy_count; i++) {
		struct spaceship_data *enemy = &game->enemies[i];
		if (enemy->state != STATE_WAITING)
			continue;

		if (rand() % 800 == 0) {
			enemy->state = STATE_JUMPING;
			enemy->jump_degree = 1.5 * PI;
			enemy->jump_x = enemy->center.x;
			enemy->jump_y = enemy->center.y;
			enemy->jump_speed = JUMPING_ENEMY_SPEED;
			enemy->jump_steps = rand() % 8 + 37;

			if (enemy->center.x < game->spaceship.center.x)
				enemy->jump_degree_delta = -JUMPING_ENEMY_ROT_SPEED;
			else
				enemy->jump_degree_delta = JUMPING_ENEMY_ROT_SPEED;	

			continue;
		}

		enemy->frame++;
		if (enemy_sprite_rect[enemy->animation][enemy->frame].w == 0)
			enemy->frame = 0;

		enemy->center = enemy->waiting_center;
	}

	game->enemy_minx += game->enemy_dx;
	game->enemy_maxx += game->enemy_dx;

	if (game->enemy_minx <= ENEMY_W) {
		game->enemy_dx = WAITING_ENEMY_SPEED;
	}

	if (game->enemy_maxx >= game->width - ENEMY_W) {
		game->enemy_dx = -WAITING_ENEMY_SPEED;
	}
}

void update_jumping_enemies(struct game_data *game)
{
	int i = 0;
	for (i = 0; i < game->enemy_count; i++) {
		double dx = 0.0;
		double dy = 0.0;

		struct spaceship_data *enemy = &game->enemies[i];
		if (enemy->state != STATE_JUMPING)
			continue;

		if (rand() % 10 == 0)
			enemy_fire(game, enemy->center);

		dx = cos(enemy->jump_degree);
		dy = sin(enemy->jump_degree);

		enemy->rotation = atan2(-dx, -dy);

		enemy->jump_x += enemy->jump_speed * dx;
		enemy->jump_y += enemy->jump_speed * dy;

		enemy->center.x = (int) enemy->jump_x;
		enemy->center.y = (int) enemy->jump_y;

		if (enemy->jump_steps > 0) {
			enemy->jump_degree += enemy->jump_degree_delta;
			enemy->jump_steps--;
		}

		if (enemy->center.y > game->height + 100) {
			enemy->center.y = -100;
			enemy->jump_x = enemy->center.x;
			enemy->jump_y = enemy->center.y;
			enemy->state = STATE_RESTORING;
		}
	}
}

void update_restoring_enemies(struct game_data *game)
{
	int i = 0;
	for (i = 0; i < game->enemy_count; i++) {
		struct spaceship_data *enemy = &game->enemies[i];
		double dx = 0.0;
		double dy = 0.0;
		double dist = 0.0;

		if (enemy->state != STATE_RESTORING)
			continue;

		if (fabs(enemy->jump_degree - 1.5 * PI) > EPS)
			enemy->jump_degree -= enemy->jump_degree_delta;

		dx = enemy->waiting_center.x - enemy->jump_x;
		dy = enemy->waiting_center.y - enemy->jump_y;
		dist = sqrt(dx * dx + dy * dy);

		if (dist < enemy->jump_speed) {
			enemy->center = enemy->waiting_center;
			enemy->state = STATE_WAITING;
			enemy->rotation = 0.0;
		}
		else {
			enemy->jump_x += enemy->jump_speed * dx / dist;
			enemy->jump_y += enemy->jump_speed * dy / dist;
			enemy->center.x = (int) enemy->jump_x;
			enemy->center.y = (int) enemy->jump_y;
			enemy->rotation = atan2(-cos(enemy->jump_degree),
						-sin(enemy->jump_degree));
		}
	}
}

void update_dying_enemies(struct game_data *game)
{
	int i = 0;
	for (i = 0; i < game->enemy_count; i++) {
		struct spaceship_data *enemy = &game->enemies[i];
		if (enemy->state != STATE_DYING)
			continue;

		enemy->frame++;
		if (enemy_sprite_rect[enemy->animation][enemy->frame].w == 0)
			enemy->state = STATE_DEAD;
	}
}

void enemy_fire(struct game_data *game, SDL_Point center)
{
	int i = 0;
	for (i = 0; i < FIRES_MAX; i++) {
		if (game->enemy_fires[i].active)
			continue;

		game->enemy_fires[i] = (struct fire_data) {
			.center = {.x = center.x, .y = center.y + ENEMY_H / 2},
			.y = center.y + ENEMY_H / 2,
			.dy = 1,
			.speed = ENEMY_FIRE_SPEED,
			.active = 1,
			.image = IMAGE_ENEMY_FIRE
		};
	}
}

void kill_enemies(struct game_data *game)
{
	int i = 0;

	if (!game->spaceship_fire.active)
		return;

	for (i = 0; i < game->enemy_count; i++) {
		struct spaceship_data *enemy = &game->enemies[i];
		SDL_Rect enemy_rect = create_rect(enemy->center, ENEMY_W, ENEMY_H);
		SDL_Rect fire_rect = create_rect(game->spaceship_fire.center,
						 FIRE_W, FIRE_H);

		if (enemy->state == STATE_DEAD || enemy->state == STATE_DYING)
			continue;

		if (SDL_HasIntersection(&enemy_rect, &fire_rect)) {
			enemy->state = STATE_DYING;
			enemy->frame = 0;
			enemy->image = IMAGE_ENEMY_DYING;
			enemy->animation = ANIM_ENEMY_DYING;
			game->spaceship_fire.active = 0;
		}
	}
}
