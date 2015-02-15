#include <nasos.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <unistd.h>

static void menu_handle_keypress(void *private, int keycode);
static void render_menu(struct display_data *display, struct menu_data *menu);
static void draw_logo(struct display_data * display);
static void draw_menu_items(struct display_data *display, struct menu_data *menu);
static TTF_Font * load_font(int font_id, int size);

int run_menu_loop(struct display_data *display)
{
	struct menu_data menu = {
		.current = 0,
		.selected = -1,
		.font = load_font(FONT_AVERIA, MENU_FONT_SIZE)
	};

	struct input_data *input = input_init(menu_handle_keypress, &menu);

	while (menu.selected == -1) {
		input_dispatch_events(input);
		render_menu(display, &menu);

		/* sleep 10ms to avoid 100% cpu */
		usleep(10000);
	}

	input_destroy(input);
	TTF_CloseFont(menu.font);

	return menu.selected;
}

static void menu_handle_keypress(void *private, int keycode)
{
	struct menu_data *menu = private;

	switch (keycode) {
	case SDLK_UP:
		if (menu->current == 0)
			menu->current = MENU_COUNT - 1;
		else
			menu->current -= 1;
		break;
	case SDLK_DOWN:
		if (menu->current == MENU_COUNT - 1)
			menu->current = 0;
		else
			menu->current += 1;
		break;
	case SDLK_RETURN:
		menu->selected = menu->current;
		break;
	}
}

static void render_menu(struct display_data *display, struct menu_data *menu)
{
	SDL_Surface *surface = SDL_GetWindowSurface(display->window);
	int black = SDL_MapRGB(surface->format, 0, 0, 0);
	SDL_FillRect(surface, NULL, black);

	draw_stars(display);
	draw_logo(display);
	draw_menu_items(display, menu);

	SDL_UpdateWindowSurface(display->window);
}

static void draw_logo(struct display_data * display)
{
	SDL_Surface *screen_surface = SDL_GetWindowSurface(display->window);
	SDL_Surface *logo = display->images[IMAGE_LOGO];

	SDL_Rect rect = {
		.x = (screen_surface->w - logo->w) / 2,
		.y = LOGO_TOP,
		.w = logo->w,
		.h = logo->h
	};

	SDL_BlitSurface(logo, NULL, screen_surface, &rect);
}

static void draw_menu_items(struct display_data *display, struct menu_data *menu)
{
	int i = 0;

	SDL_Surface *screen_surface = SDL_GetWindowSurface(display->window);
	int w = screen_surface->w;

	for (i = 0; i < MENU_COUNT; i++) {
		SDL_Surface *menu_item_surface = NULL;
		SDL_Color color;
		SDL_Rect rect;

		if (i == menu->current)
			color = (SDL_Color) {10, 200, 10, 255};
		else
			color = (SDL_Color) {200, 200, 200, 255};
		
		menu_item_surface = TTF_RenderText_Solid(menu->font, menu_text[i],
							 color);

		rect = (SDL_Rect) {
			.x = (w - menu_item_surface->w) / 2,
			.y = MENU_TOP + (MENU_ROW_DIST * i),
			.w = menu_item_surface->w,
			.h = menu_item_surface->h
		};

		SDL_BlitSurface(menu_item_surface, NULL, screen_surface, &rect);

		SDL_FreeSurface(menu_item_surface);
	}
}

static TTF_Font * load_font(int font_id, int size)
{
	TTF_Font *font = NULL;
	char path[PATH_MAX];
	char current_dir[PATH_MAX];

	if (!TTF_WasInit())
		TTF_Init();

	getcwd(current_dir, PATH_MAX);

	/* try data sub-directory */
	sprintf(path, "%s/data/%s", current_dir, font_filename[font_id]);
	font = TTF_OpenFont(path, size);

	/* if still not found, try installation directory */
	if (font == NULL) {
		sprintf(path, "%s/%s", DATADIR, font_filename[font_id]);
		font = TTF_OpenFont(path, size);
	}

	return font;
}
