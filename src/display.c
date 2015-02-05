#include <nasos.h>

struct display_data * display_init(void)
{
	struct display_data *display = NULL;
	SDL_Window *window = NULL;

	const char *title = "Nasos";
	const int width = 640;
	const int height = 480;

	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
				  SDL_WINDOWPOS_UNDEFINED, width, height,
				  SDL_WINDOW_OPENGL);

	display = malloc(sizeof(struct display_data));
	display->window = window;

	return display;
}

void display_destroy(struct display_data *display)
{
	SDL_DestroyWindow(display->window);
	SDL_Quit();

	free(display);
}

void display_render(struct display_data *display, struct game_data *game)
{
	struct spaceship_data *spaceship = &game->spaceship;
	SDL_Surface *surface = SDL_GetWindowSurface(display->window);

	int black = SDL_MapRGB(surface->format, 0, 0, 0);
	int white = SDL_MapRGB(surface->format, 200, 200, 200);

	SDL_Rect ship_rect = {
		.x = spaceship->position.x,
		.y = spaceship->position.y,
		.w = 45,
		.h = 31
	};

	SDL_FillRect(surface, NULL, black);
	
	SDL_Surface *spaceship_surface = SDL_LoadBMP("data/ship.bmp");
	if (spaceship_surface == NULL)
	{
		SDL_FillRect(surface, &ship_rect, white);
	}
	else
	{
		SDL_SetColorKey(spaceship_surface, SDL_TRUE, 0x008800);
		SDL_BlitSurface(spaceship_surface, NULL, surface, &ship_rect);
		SDL_FreeSurface(spaceship_surface);
	}

	SDL_UpdateWindowSurface(display->window);
}
