#include <stdlib.h>
#include <unistd.h>
#include <nasos.h>

static void run_game_loop(struct display_data *display);

int main()
{
	int done = 0;
	struct display_data *display = display_init(GAME_W, GAME_H);

	while (!done) {
		int selected = run_menu_loop(display);
		switch (selected)
		{
		case MENU_START:
			run_game_loop(display);
			break;
		case MENU_QUIT:
			done = 1;
			break;
		}
	}

	display_destroy(display);

	return 0;
}

static void run_game_loop(struct display_data *display)
{
	struct game_data *game = game_init();
	struct input_data *input = input_init(game_handle_keypress, game);
	struct mixer_data *mixer = mixer_init();
	struct timer_data *timers[TIMER_COUNT];
	int timer_index = 0;

	for (timer_index = 0; timer_index < TIMER_COUNT; timer_index++)
		timers[timer_index] = timer_init(timer_index,
						 timer_duration[timer_index],
						 game_handle_timer, game);

	while (!game_done(game)) {
		display_render_game(display, game);
		input_dispatch_events(input);

		for (timer_index = 0; timer_index < TIMER_COUNT; timer_index++)
			timer_dispatch_events(timers[timer_index]);

		mixer_update(mixer, game);

		/* sleep 10ms to avoid 100% cpu */
		usleep(10000);
	}

	for (timer_index = 0; timer_index < TIMER_COUNT; timer_index++)
		timer_destroy(timers[timer_index]);

	mixer_destroy(mixer);
	input_destroy(input);
	game_destroy(game);
}
