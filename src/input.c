#include <nasos.h>
#include <stdio.h>

static void check_arrow_key(struct input_data *input, struct game_data *game,
			    int, int, int index);

struct input_data * input_init(void)
{
	struct input_data *input = malloc(sizeof(struct input_data));
	input->source = KEYBOARD;
	input->last_arrow_report[0] = 0;
	input->last_arrow_report[1] = 0;

	return input;
}

void input_destroy(struct input_data *input)
{
	free(input);
}

void input_dispatch_events(struct input_data *input, struct game_data *game)
{
	SDL_Event sdl_event;

	while (SDL_PollEvent(&sdl_event)) {
		if (sdl_event.type != SDL_KEYDOWN)
			continue;

		if (sdl_event.key.keysym.sym != SDLK_LEFT &&
		    sdl_event.key.keysym.sym != SDLK_RIGHT)
			game_handle_keypress(game, sdl_event.key.keysym.sym);
	}

	check_arrow_key(input, game, SDL_SCANCODE_LEFT, SDLK_LEFT, 0);
	check_arrow_key(input, game, SDL_SCANCODE_RIGHT, SDLK_RIGHT, 1);
}

static void check_arrow_key(struct input_data *input, struct game_data *game,
			    int scancode, int keycode, int index)
{
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[scancode] &&
	    SDL_GetTicks() > input->last_arrow_report[index] + 50) {
		game_handle_keypress(game, keycode);
		input->last_arrow_report[index] = SDL_GetTicks();
	}	
}