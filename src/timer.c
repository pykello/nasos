#include <nasos.h>

struct timer_data * timer_init(int id, int duration,
			       void (*timer_handle_func)(void *, int),
			       void *private)
{
	struct timer_data *timer = NULL;

	timer = malloc(sizeof(struct timer_data));
	(*timer) = (struct timer_data) {
		.id = id,
		.duration = duration,
		.last_tick = SDL_GetTicks(),
		.timer_handle_func = timer_handle_func,
		.private = private
	};

	return timer;
}

void timer_destroy(struct timer_data *timer)
{
	free(timer);
}

void timer_dispatch_events(struct timer_data *timer)
{
	int current_tick = SDL_GetTicks();
	if (current_tick - timer->last_tick >= timer->duration) {
		timer->timer_handle_func(timer->private, timer->id);
		timer->last_tick = current_tick;
	}
}
