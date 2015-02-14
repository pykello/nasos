#include <nasos.h>
#include <stdlib.h>

static void draw_lifes(struct display_data *display, int lifes);
static void draw_spaceship(struct display_data *display,
			   struct spaceship_data *spaceship);
static void draw_fire(struct display_data *display,
		      struct fire_data *fire);
static void draw_star(struct display_data *display, struct star_data *star,
		      int ticks, int height);
static SDL_Rect create_spaceship_rect(struct spaceship_data *ship);

void display_render_game(struct display_data *display, struct game_data *game)
{
	int i = 0;
	int ticks = SDL_GetTicks();

	SDL_Surface *surface = SDL_GetWindowSurface(display->window);
	int black = SDL_MapRGB(surface->format, 0, 0, 0);
	SDL_FillRect(surface, NULL, black);

	draw_lifes(display, game->lifes);
	draw_spaceship(display, &game->player);
	draw_fire(display, &game->player_fire);

	for (i = 0; i < game->enemy_count; i++)
		draw_spaceship(display, &game->enemies[i]);

	for (i = 0; i < FIRES_MAX; i++)
		draw_fire(display, &game->enemy_fires[i]);

	for (i = 0; i < STAR_COUNT; i++)
		draw_star(display, &display->stars[i], ticks, game->height);

	SDL_UpdateWindowSurface(display->window);
}

static void draw_lifes(struct display_data *display, int lifes)
{
	int i = 0;

	SDL_Surface *screen_surface = SDL_GetWindowSurface(display->window);
	SDL_Surface *life_surface = display->images[IMAGE_LIFE];

	for (i = 0; i < lifes; i++) {
		SDL_Rect rect = {
			.x = LIFE_LEFT + i * (LIFE_DIST + life_surface->w),
			.y = screen_surface->h - LIFE_BOTTOM - life_surface->h,
			.w = life_surface->w,
			.h = life_surface->h
		};

		SDL_BlitSurface(life_surface, NULL, screen_surface, &rect);
	}
}

static void draw_spaceship(struct display_data *display,
			   struct spaceship_data *spaceship)
{
	SDL_Surface *screen_surface = NULL;
	SDL_Surface *image_surface = NULL;
	SDL_Rect image_rect;
	SDL_Rect screen_rect;

	double rotation_delta = (2 * PI) / ROTATION_COUNT;
	int rotation_idx = 0;
	double rotation = spaceship->rotation;

	if (rotation < 0)
		rotation += 2 * PI;
	if (rotation > 2 * PI)
		rotation -= 2 * PI;

	rotation_idx = (int) ((rotation + EPS) / rotation_delta);

	screen_surface = SDL_GetWindowSurface(display->window);

	if (rotation_idx > 0 && rotation_idx < ROTATION_COUNT &&
	    display->rotations[spaceship->image][rotation_idx] != NULL)
	{
		image_surface = display->rotations[spaceship->image][rotation_idx];
		image_rect = (SDL_Rect) {
			.x = 0, .y = 0,
			.w = image_surface->w,
			.h = image_surface->h
		};
		screen_rect = create_rect(spaceship->center, image_surface->w,
					  image_surface->h);
	}
	else {
		image_surface = display->images[spaceship->image];
		image_rect = enemy_sprite_rect[spaceship->animation][spaceship->frame];
		screen_rect = create_spaceship_rect(spaceship);
	}

	if (spaceship->state != STATE_DEAD)
		SDL_BlitSurface(image_surface, &image_rect,
				screen_surface, &screen_rect);
}

static void draw_fire(struct display_data *display,
		      struct fire_data *fire)
{
	SDL_Surface *screen_surface = SDL_GetWindowSurface(display->window);
	SDL_Surface *fire_surface = display->images[fire->image];
	SDL_Point center = fire->center;
	SDL_Rect target_rect = create_rect(center, fire_surface->w,
					   fire_surface->h);

	if (fire->active)
		SDL_BlitSurface(fire_surface, NULL,
				screen_surface, &target_rect);
}

static void draw_star(struct display_data *display, struct star_data *star,
		      int ticks, int height)
{
	SDL_Surface *screen_surface = SDL_GetWindowSurface(display->window);
	int white = SDL_MapRGB(screen_surface->format, 200, 200, 200);
	SDL_Rect rect = star->rect;
	
	int step = (star->step + ticks) % STAR_STEPS_MAX;
	if (step < STAR_STEPS_MAX / 2)
		return;

	rect.y = (rect.y + ticks / 20) % height;

	SDL_FillRect(screen_surface, &rect, white);
}

static SDL_Rect create_spaceship_rect(struct spaceship_data *ship)
{
	SDL_Rect image_rect = enemy_sprite_rect[ship->animation][ship->frame];
	SDL_Rect spaceship_rect = create_rect(ship->center, image_rect.w, image_rect.h);

	return spaceship_rect;
}

SDL_Rect create_rect(SDL_Point center, int w, int h)
{
	SDL_Rect rect = {
		.x = center.x - w / 2,
		.y = center.y - h / 2,
		.w = w,
		.h = h
	};

	return rect;
}
