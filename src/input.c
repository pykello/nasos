#include <nasos.h>
#include <stdio.h>

static void check_arrow_key(struct input_data *input, int, int, int index);

struct input_data * input_init(void (*handle_input_func)(void *, int), void *private)
{
	struct input_data *input = malloc(sizeof(struct input_data));
	input->source = KEYBOARD;
	input->last_arrow_report[0] = 0;
	input->last_arrow_report[1] = 0;
	input->handle_input_func = handle_input_func;
	input->private = private;

	return input;
}

void input_destroy(struct input_data *input)
{
	free(input);
}

void input_dispatch_events(struct input_data *input)
{
	SDL_Event sdl_event;

	while (SDL_PollEvent(&sdl_event)) {
		if (sdl_event.type == SDL_KEYDOWN &&
			sdl_event.key.keysym.sym != SDLK_LEFT &&
		    sdl_event.key.keysym.sym != SDLK_RIGHT)
		{
			input->handle_input_func(input->private,
									 sdl_event.key.keysym.sym);
		}
	}

	check_arrow_key(input, SDL_SCANCODE_LEFT, SDLK_LEFT, 0);
	check_arrow_key(input, SDL_SCANCODE_RIGHT, SDLK_RIGHT, 1);
}

static void check_arrow_key(struct input_data *input, int scancode,
			    int keycode, int index)
{
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[scancode] &&
	    SDL_GetTicks() > input->last_arrow_report[index] + 50) {
		input->handle_input_func(input->private, keycode);
		input->last_arrow_report[index] = SDL_GetTicks();
	}	
}