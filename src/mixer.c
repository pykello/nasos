#include <nasos.h>
#include <unistd.h>
#include <SDL2/SDL_mixer.h>

static void play_fire(struct mixer_data *mixer, int sound,
		      struct fire_data *fire, int *fire_was_active);
static void play_explosion(struct mixer_data *mixer, int sound,
			   struct spaceship_data *spaceship, int ex_state);
static void play_jump(struct mixer_data *mixer, int sound,
		      struct spaceship_data *spaceship, int ex_state);
static void load_sounds(struct mixer_data *mixer);
static void free_sounds(struct mixer_data *mixer);

struct mixer_data * mixer_init(void)
{
	struct mixer_data *mixer = NULL;
	int enabled = 0;

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,
			  MIX_DEFAULT_FORMAT, 2, 1024) != -1)
		enabled = 1;

	Mix_AllocateChannels(128);

	mixer = malloc(sizeof(struct mixer_data));
	mixer->enabled = enabled;
	mixer->background_started = 0;
	mixer->player_fire_active = 0;
	memset(mixer->enemy_fire_active, 0, sizeof(mixer->enemy_fire_active));
	mixer->player_state = 0;
	memset(mixer->enemy_state, 0, sizeof(mixer->enemy_state));
	load_sounds(mixer);

	return mixer;
}

void mixer_destroy(struct mixer_data *mixer)
{
	free_sounds(mixer);
	free(mixer);

	Mix_CloseAudio();
}

void mixer_update(struct mixer_data *mixer, struct game_data *game)
{
	int i = 0;

	if (!mixer->enabled)
		return;

	if (!mixer->background_started &&
	    mixer->sounds[SOUND_BACKGROUND] != NULL) {
		Mix_PlayChannel(-1, mixer->sounds[SOUND_BACKGROUND], -1);
		mixer->background_started = 1;
	}

	play_fire(mixer, SOUND_PLAYER_FIRE, &game->player_fire,
		  &mixer->player_fire_active);

	for (i = 0; i < FIRES_MAX; i++)
		play_fire(mixer, SOUND_ENEMY_FIRE, &game->enemy_fires[i],
			  &mixer->enemy_fire_active[i]);

	play_explosion(mixer, SOUND_PLAYER_EXPLOSION, &game->player,
		       mixer->player_state);

	for (i = 0; i < game->enemy_count; i++)
		play_explosion(mixer, SOUND_ENEMY_EXPLOSION, &game->enemies[i],
			       mixer->enemy_state[i]);

	for (i = 0; i < game->enemy_count; i++)
		play_jump(mixer, SOUND_ENEMY_JUMPING, &game->enemies[i],
			  mixer->enemy_state[i]);

	mixer->player_state = game->player.state;

	for (i = 0; i < game->enemy_count; i++)
		mixer->enemy_state[i] = game->enemies[i].state;
}

static void play_fire(struct mixer_data *mixer, int sound,
		      struct fire_data *fire, int *fire_was_active)
{
	if (mixer->sounds[sound] == NULL)
		return;

	if (fire->active && !*fire_was_active)
		Mix_PlayChannel(-1, mixer->sounds[sound], 0);

	*fire_was_active = fire->active;
}

static void play_explosion(struct mixer_data *mixer, int sound,
			   struct spaceship_data *spaceship, int ex_state)
{
	if (mixer->sounds[sound] == NULL)
		return;

	if (spaceship->state == STATE_DYING && ex_state != STATE_DYING)
		Mix_PlayChannel(-1, mixer->sounds[sound], 0);
}

static void play_jump(struct mixer_data *mixer, int sound,
		      struct spaceship_data *spaceship, int ex_state)
{
	if (mixer->sounds[sound] == NULL)
		return;

	if (spaceship->state == STATE_JUMPING && ex_state != STATE_JUMPING)
		Mix_PlayChannel(-1, mixer->sounds[sound], 0);
}

static void load_sounds(struct mixer_data *mixer)
{
	int i = 0;

	char current_dir[PATH_MAX];
	getcwd(current_dir, PATH_MAX);

	for (i = 0; i < SOUND_COUNT; i++) {
		char path[PATH_MAX];
		Mix_Chunk *sound = NULL;

		/* try data sub-directory */
		sprintf(path, "%s/data/%s", current_dir, sound_filename[i]);
		sound = Mix_LoadWAV(path);

		/* if still not found, try installation directory */
		if (sound == NULL) {
			sprintf(path, "%s/%s", DATADIR, sound_filename[i]);
			sound = Mix_LoadWAV(path);
		}

		mixer->sounds[i] = sound;
	}
}

static void free_sounds(struct mixer_data *mixer)
{
	int i = 0;

	for (i = 0; i < SOUND_COUNT; i++)
		if (mixer->sounds[i] != NULL)
			Mix_FreeChunk(mixer->sounds[i]);
}
