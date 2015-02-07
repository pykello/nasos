#include <stdlib.h>
#include <unistd.h>
#include <nasos.h>

int main() {
	struct game_data *game = game_init();
	struct display_data *display = display_init(game);
	struct input_data *input = input_init();
	struct timer_data *timers[TIMER_COUNT];
	int timer_index = 0;

	for (timer_index = 0; timer_index < TIMER_COUNT; timer_index++)
		timers[timer_index] = timer_init(timer_index,
						 timer_duration[timer_index]);

	while (!game_done(game)) {
		display_render(display, game);
		input_dispatch_events(input, game);

		for (timer_index = 0; timer_index < TIMER_COUNT; timer_index++)
			timer_dispatch_events(timers[timer_index], game);

		/* sleep 10ms to avoid 100% cpu */
		usleep(10000);
	}

	for (timer_index = 0; timer_index < TIMER_COUNT; timer_index++)
		timer_destroy(timers[timer_index]);

	input_destroy(input);
	display_destroy(display);
	game_destroy(game);

	return 0;
}
