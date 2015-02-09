#include <nasos.h>
#include <stdlib.h>

static void player_fire(struct game_data *game);
static void update_waiting_enemies(struct game_data *game);
static void update_jumping_enemies(struct game_data *game);
static void update_restoring_enemies(struct game_data *game);
static void enemy_fire(struct game_data *game, SDL_Point center);
static void update_dying_enemies(struct game_data *game);
static void update_dying_player(struct game_data *game);
static void update_fires(struct game_data *game);
static void update_fire(struct fire_data *fire, int height);
static void kill_enemies(struct game_data *game);
static void kill_player(struct game_data *game);
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
		.animation = SPACESHIP,
		.rotation = 0.0
	};

	game->spaceship_fire = (struct fire_data) {
		.active = 0
	};

	init_enemies(game);
	memset(game->enemy_fires, 0, sizeof(game->enemy_fires));

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
	struct spaceship_data *ship = &game->spaceship;

	switch (keycode) {
	case 'q':
		game->done = 1;
		break;
	case ' ':
		player_fire(game);
		break;
	case SDLK_LEFT:
		if (ship->center.x > 30)
			ship->center.x -= 8;
		break;
	case SDLK_RIGHT:
		if (ship->center.x < game->width - 30)
			ship->center.x += 8;
		break;
	}
}

void game_handle_timer(struct game_data *game, int timer_id)
{
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

static void player_fire(struct game_data *game)
{
	SDL_Point player_center = game->spaceship.center;
	int x = player_center.x;
	int y = player_center.y - 23;

	if (game->spaceship_fire.active)
		return;

	game->spaceship.frame = 1;
	game->spaceship_fire = (struct fire_data) {
		.center = {.x = x, .y = y},
		.y = (double) y,
		.dy = -1,
		.speed = 24,
		.active = 1,
		.image = IMAGE_PLAYER_FIRE
	};
}

static void update_waiting_enemies(struct game_data *game)
{
	int i = 0;
	for (i = 0; i < game->enemy_count; i++)
		game->enemies[i].waiting_center.x += game->enemy_dx;

	for (i = 0; i < game->enemy_count; i++) {
		struct spaceship_data *enemy = &game->enemies[i];
		if (enemy->state != WAITING)
			continue;

		if (rand() % 800 == 0) {
			enemy->state = JUMPING;
			enemy->jump_degree = 1.5 * PI;
			enemy->jump_x = enemy->center.x;
			enemy->jump_y = enemy->center.y;
			enemy->jump_speed = 10;
			enemy->jump_steps = rand() % 8 + 37;

			if (enemy->center.x < game->spaceship.center.x)
				enemy->jump_degree_delta = -PI / 35;
			else
				enemy->jump_degree_delta = PI / 35;	

			continue;
		}

		enemy->frame++;
		if (enemy_sprite_rect[enemy->animation][enemy->frame].w == 0)
			enemy->frame = 0;

		enemy->center = enemy->waiting_center;
	}

	game->enemy_minx += game->enemy_dx;
	game->enemy_maxx += game->enemy_dx;

	if (game->enemy_minx <= 30) {
		game->enemy_dx = ENEMY_DX_DEFAULT;
	}

	if (game->enemy_maxx >= game->width - 30) {
		game->enemy_dx = -ENEMY_DX_DEFAULT;
	}
}

static void update_jumping_enemies(struct game_data *game)
{
	int i = 0;
	for (i = 0; i < game->enemy_count; i++) {
		double dx = 0.0;
		double dy = 0.0;

		struct spaceship_data *enemy = &game->enemies[i];
		if (enemy->state != JUMPING)
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
			enemy->state = RESTORING;
		}
	}
}

static void update_restoring_enemies(struct game_data *game)
{
	int i = 0;
	for (i = 0; i < game->enemy_count; i++) {
		struct spaceship_data *enemy = &game->enemies[i];
		double dx = 0.0;
		double dy = 0.0;
		double dist = 0.0;

		if (enemy->state != RESTORING)
			continue;

		if (fabs(enemy->jump_degree - 1.5 * PI) > EPS)
			enemy->jump_degree -= enemy->jump_degree_delta;

		dx = enemy->waiting_center.x - enemy->jump_x;
		dy = enemy->waiting_center.y - enemy->jump_y;
		dist = sqrt(dx * dx + dy * dy);

		if (dist < enemy->jump_speed) {
			enemy->center = enemy->waiting_center;
			enemy->state = WAITING;
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

static void enemy_fire(struct game_data *game, SDL_Point center)
{
	int i = 0;
	for (i = 0; i < FIRES_MAX; i++) {
		if (game->enemy_fires[i].active)
			continue;

		game->enemy_fires[i] = (struct fire_data) {
			.center = {.x = center.x, .y = center.y + 10},
			.y = center.y + 10,
			.dy = 1,
			.speed = 16,
			.active = 1,
			.image = IMAGE_ENEMY_FIRE
		};
	}
}

static void update_dying_enemies(struct game_data *game)
{
	int i = 0;
	for (i = 0; i < game->enemy_count; i++) {
		struct spaceship_data *enemy = &game->enemies[i];
		if (enemy->state != DYING)
			continue;

		enemy->frame++;
		if (enemy_sprite_rect[enemy->animation][enemy->frame].w == 0)
			enemy->state = DEAD;
	}
}

static void update_dying_player(struct game_data *game)
{
	struct spaceship_data *player = &game->spaceship;
	if (player->state != DYING)
		return;

	player->frame++;
	if (enemy_sprite_rect[player->animation][player->frame].w == 0)
		player->state = DEAD;
}

static void update_fires(struct game_data *game)
{
	int i = 0;
	struct spaceship_data *player = &game->spaceship;

	update_fire(&game->spaceship_fire, game->height);

	for (i = 0; i < FIRES_MAX; i++)
		update_fire(&game->enemy_fires[i], game->height);

	if (player->state != DEAD && player->state != DYING) {
		if (game->spaceship_fire.active)
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

static void kill_enemies(struct game_data *game)
{
	int i = 0;

	if (!game->spaceship_fire.active)
		return;

	for (i = 0; i < game->enemy_count; i++) {
		struct spaceship_data *enemy = &game->enemies[i];
		SDL_Rect enemy_rect = create_spaceship_rect(enemy);
		SDL_Rect fire_rect = create_rect(game->spaceship_fire.center, 4, 13);

		if (enemy->state == DEAD || enemy->state == DYING)
			continue;

		if (SDL_HasIntersection(&enemy_rect, &fire_rect)) {
			enemy->state = DYING;
			enemy->frame = 0;
			enemy->image = IMAGE_ENEMY_DYING;
			enemy->animation = ENEMY_DYING;
			game->spaceship_fire.active = 0;
		}
	}
}

static void kill_player(struct game_data *game)
{
	int i = 0;
	int should_die = 0;
	struct spaceship_data *player = &game->spaceship;
	SDL_Rect player_rect = create_spaceship_rect(player);

	if (player->state == DEAD || player->state == DYING)
		return;

	for (i = 0; i < game->enemy_count && !should_die; i++) {
		SDL_Rect enemy_rect = create_spaceship_rect(&game->enemies[i]);

		if (game->enemies[i].state == DEAD ||
		    game->enemies[i].state == DYING)
			continue;

		if (SDL_HasIntersection(&enemy_rect, &player_rect))
			should_die = 1;
	}

	for (i = 0; i < FIRES_MAX && !should_die; i++) {
		struct fire_data *fire = &game->enemy_fires[i];

		if (fire->active) {
			SDL_Rect fire_rect = create_rect(fire->center, 4, 13);
			if (SDL_HasIntersection(&fire_rect, &player_rect))
				should_die = 1;
		}
	}

	if (should_die) {
		player->state = DYING;
		player->frame = 0;
		player->image = IMAGE_PLAYER_DYING;
		player->animation = PLAYER_DYING;
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
			.frame = 0,
			.state = WAITING,
			.rotation = 0.0
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
			.frame = rand() % 3,
			.state = WAITING,
			.rotation = 0.0
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
			.frame = rand() % 3,
			.state = WAITING,
			.rotation = 0.0
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
				.frame = rand() % 3,
				.state = WAITING,
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

	game->enemy_dx = -ENEMY_DX_DEFAULT;
}
