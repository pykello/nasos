#include <stdlib.h>
#include <unistd.h>
#include <nasos.h>

int main() {
	struct game_data *game = game_init();
	struct display_data *display = display_init();
	struct input_data *input = input_init();
	struct timer_data *timer = timer_init();

	while (!game_done(game)) {
		display_render(display, game);
		input_dispatch_events(input, game);
		timer_dispatch_events(timer, game);

		/* sleep 5ms to avoid 100% cpu */
		usleep(5000);
	}

	timer_destroy(timer);
	input_destroy(input);
	display_destroy(display);
	game_destroy(game);

	return 0;
}
