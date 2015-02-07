#include <nasos.h>
#include <unistd.h>
#include <SDL2/SDL2_rotozoom.h>

static void draw_spaceship(struct display_data *display,
			   struct spaceship_data *spaceship);
static void draw_fire(struct display_data *display,
		      struct fire_data *fire);
static void load_images(struct display_data *display);
static void free_images(struct display_data *display);
static void create_rotations(struct display_data *display, int image,
			     SDL_Rect clip_rect);
static void free_rotations(struct display_data *display);

struct display_data * display_init(struct game_data *game)
{
	struct display_data *display = NULL;
	SDL_Window *window = NULL;

	const char *title = "Nasos";

	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
				  SDL_WINDOWPOS_UNDEFINED, game->width,
				  game->height, SDL_WINDOW_OPENGL);

	display = malloc(sizeof(struct display_data));
	display->window = window;
	load_images(display);

	memset(display->rotations, 0, sizeof(display->rotations));
	create_rotations(display, IMAGE_ENEMY1A, enemy_sprite_rect[NON_ATTACKING_1][0]);
	create_rotations(display, IMAGE_ENEMY2A, enemy_sprite_rect[NON_ATTACKING_1][0]);
	create_rotations(display, IMAGE_ENEMY3A, enemy_sprite_rect[NON_ATTACKING_1][0]);
	create_rotations(display, IMAGE_ENEMY4A, enemy_sprite_rect[NON_ATTACKING_2][0]);

	return display;
}

void display_destroy(struct display_data *display)
{
	free_images(display);
	free_rotations(display);	

	SDL_DestroyWindow(display->window);
	SDL_Quit();

	free(display);
}

void display_render(struct display_data *display, struct game_data *game)
{
	int i = 0;

	SDL_Surface *surface = SDL_GetWindowSurface(display->window);
	int black = SDL_MapRGB(surface->format, 0, 0, 0);
	SDL_FillRect(surface, NULL, black);

	draw_spaceship(display, &game->spaceship);
	draw_fire(display, &game->spaceship_fire);

	for (i = 0; i < game->enemy_count; i++)
		draw_spaceship(display, &game->enemies[i]);

	for (i = 0; i < FIRES_MAX; i++)
		draw_fire(display, &game->enemy_fires[i]);

	SDL_UpdateWindowSurface(display->window);
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

	if (spaceship->state != DEAD)
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

static void load_images(struct display_data *display)
{
	int i = 0;

	char current_dir[PATH_MAX];
	getcwd(current_dir, PATH_MAX);

	for (i = 0; i < IMAGE_COUNT; i++) {
		char path[PATH_MAX];
		SDL_Surface *image = NULL;

		/* try data sub-directory */
		sprintf(path, "%s/data/%s", current_dir, image_filename[i]);
		image = SDL_LoadBMP(path);
		
		/* if still not found, try installation directory */
		if (image == NULL) {
			sprintf(path, "%s/%s", DATADIR, image_filename[i]);
			image = SDL_LoadBMP(path);
		}

		/* if still not found, use a small square */
		if (image == NULL) {
			image = SDL_CreateRGBSurface(0, 32, 32, 32,
						     0xff000000,
						     0x00ff0000,
						     0x0000ff00,
						     0x000000ff);
			SDL_FillRect(image, NULL,
				     SDL_MapRGB(image->format, 200, 200, 200));
		}

		SDL_SetColorKey(image, SDL_TRUE, 0x000000);
		display->images[i] = image;
	}
}

static void free_images(struct display_data *display)
{
	int i = 0;
	for (i = 0; i < IMAGE_COUNT; i++)
		SDL_FreeSurface(display->images[i]);
}

SDL_Rect create_spaceship_rect(struct spaceship_data *ship)
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

static void create_rotations(struct display_data *display, int image,
			     SDL_Rect clip_rect)
{
	int i = 0;
	SDL_Surface *clipped_surface = NULL;

	clipped_surface = SDL_CreateRGBSurface(0, clip_rect.w, clip_rect.h,
					       32, 0xff000000, 0x00ff0000,
					       0x0000ff00, 0x000000ff);
	SDL_BlitSurface(display->images[image], &clip_rect,
			clipped_surface, NULL);

	for (i = 0; i < ROTATION_COUNT; i++) {
		SDL_Surface *rotated_surface = NULL;
		double angle = (360.0 * i) / ROTATION_COUNT;

		rotated_surface = rotozoomSurface(clipped_surface, angle, 1.0, 1);
		display->rotations[image][i] = rotated_surface;
	}

	SDL_FreeSurface(clipped_surface);
}

static void free_rotations(struct display_data *display)
{
	int i = 0, j = 0;

	for (i = 0; i < IMAGE_COUNT; i++)
		for (j = 0; j < ROTATION_COUNT; j++)
			if (display->rotations[i][j] != NULL)
				SDL_FreeSurface(display->rotations[i][j]);
}
