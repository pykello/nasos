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
		.w = 10,
		.h = 20
	};

	SDL_FillRect(surface, NULL, black);
	SDL_FillRect(surface, &ship_rect, white);

	SDL_UpdateWindowSurface(display->window);
}
