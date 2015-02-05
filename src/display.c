#include <nasos.h>
#include <unistd.h>

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
	SDL_Surface *spaceship_surface = display->images[IMAGE_SHIP];

	int black = SDL_MapRGB(surface->format, 0, 0, 0);

	SDL_FillRect(surface, NULL, black);
	SDL_BlitSurface(spaceship_surface, NULL, surface, &game->spaceship.rect);

	for (i = 0; i < game->enemy_count; i++) {
		struct spaceship_data *enemy = &game->enemies[i];
		SDL_Surface *enemy_surface =  display->images[enemy->image];

		SDL_BlitSurface(enemy_surface,
				&enemy_sprite_rect[enemy->animation][enemy->frame],
				surface,
				&enemy->rect);
	}

	SDL_UpdateWindowSurface(display->window);
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
