#include <nasos.h>
#include <unistd.h>
#include <SDL2/SDL2_rotozoom.h>

static void load_images(struct display_data *display);
static void free_images(struct display_data *display);
static void create_rotations(struct display_data *display, int image,
			     SDL_Rect clip_rect);
static void free_rotations(struct display_data *display);
static void create_stars(struct display_data *display, int w, int h);
static void draw_star(struct display_data *display, struct star_data *star,
		      int ticks);

struct display_data * display_init(int w, int h)
{
	struct display_data *display = NULL;
	SDL_Window *window = NULL;

	const char *title = "Nasos";

	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
				  SDL_WINDOWPOS_UNDEFINED, w, h,
				  SDL_WINDOW_OPENGL);

	display = malloc(sizeof(struct display_data));
	display->window = window;
	load_images(display);

	memset(display->rotations, 0, sizeof(display->rotations));
	create_rotations(display, IMAGE_ENEMY1A, enemy_sprite_rect[ANIM_NON_ATTACKING_1][0]);
	create_rotations(display, IMAGE_ENEMY2A, enemy_sprite_rect[ANIM_NON_ATTACKING_1][0]);
	create_rotations(display, IMAGE_ENEMY3A, enemy_sprite_rect[ANIM_NON_ATTACKING_1][0]);
	create_rotations(display, IMAGE_ENEMY4A, enemy_sprite_rect[ANIM_NON_ATTACKING_2][0]);

	create_stars(display, w, h);

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

static void create_stars(struct display_data *display, int w, int h)
{
	int i = 0;
	for (i = 0; i < STAR_COUNT; i++) {
		int size = 1 + rand() % STAR_SIZE_MAX;
		int x = rand() % w;
		int y = rand() % h;
		int step = rand() % STAR_STEPS_MAX;

		display->stars[i] = (struct star_data) {
			.rect = {.x = x, .y = y, .w = size, .h = size},
			.step = step
		};
	}
}

void draw_stars(struct display_data *display)
{
	int ticks = SDL_GetTicks();
	int i = 0;
	for (i = 0; i < STAR_COUNT; i++)
		draw_star(display, &display->stars[i], ticks);
}

static void draw_star(struct display_data *display, struct star_data *star,
		      int ticks)
{
	SDL_Surface *screen_surface = SDL_GetWindowSurface(display->window);
	int height = screen_surface->h;
	int white = SDL_MapRGB(screen_surface->format, 200, 200, 200);
	SDL_Rect rect = star->rect;
	
	int step = (star->step + ticks) % STAR_STEPS_MAX;
	if (step < STAR_STEPS_MAX / 2)
		return;

	rect.y = (rect.y + ticks / 20) % height;

	SDL_FillRect(screen_surface, &rect, white);
}

