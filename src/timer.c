#include <nasos.h>

struct timer_data * timer_init(int id, int duration)
{
	struct timer_data *timer = NULL;

	timer = malloc(sizeof(struct timer_data));
	(*timer) = (struct timer_data) {
		.id = id,
		.duration = duration,
		.last_tick = SDL_GetTicks()
	};

	return timer;
}

void timer_destroy(struct timer_data *timer)
{
	free(timer);
}

void timer_dispatch_events(struct timer_data *timer, struct game_data *game)
{
	int current_tick = SDL_GetTicks();
	if (current_tick - timer->last_tick >= timer->duration) {
		game_handle_timer(game, timer->id);
		timer->last_tick = current_tick;
	}
}
