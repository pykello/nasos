#include <nasos.h>
#include <stdio.h>

struct input_data * input_init(void)
{
	struct input_data *input = malloc(sizeof(struct input_data));
	input->source = KEYBOARD;

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

		game_handle_keypress(game, sdl_event.key.keysym.sym);
	}
}
