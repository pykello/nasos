#include <nasos.h>
#include <unistd.h>

static void draw_spaceship(struct display_data *display,
			   struct spaceship_data *spaceship);
static void draw_fire(struct display_data *display,
		      struct fire_data *fire);
static void load_images(struct display_data *display);
static void free_images(struct display_data *display);

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

	return display;
}

void display_destroy(struct display_data *display)
{
	free_images(display);	

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

	SDL_UpdateWindowSurface(display->window);
}

static void draw_spaceship(struct display_data *display,
			   struct spaceship_data *spaceship)
{
	SDL_Surface *screen_surface = SDL_GetWindowSurface(display->window);

	SDL_Surface *spaceship_surface = display->images[spaceship->image];
	SDL_Rect image_rect = enemy_sprite_rect[spaceship->animation][spaceship->frame];
	SDL_Rect target_rect = create_spaceship_rect(spaceship);

	if (spaceship->state != DEAD)
		SDL_BlitSurface(spaceship_surface, &image_rect,
				screen_surface, &target_rect);
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

