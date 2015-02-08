#include <nasos.h>
#include <unistd.h>
#include <SDL2/SDL_mixer.h>

static void load_sounds(struct mixer_data *mixer);
static void free_sounds(struct mixer_data *mixer);

struct mixer_data * mixer_init(void)
{
	struct mixer_data *mixer = NULL;
	int enabled = 0;

	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) != -1 )
		enabled = 1;

	mixer = malloc(sizeof(struct mixer_data));
	mixer->enabled = enabled;
	mixer->background_started = 0;
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
	if (!mixer->enabled)
		return;

	if (!mixer->background_started &&
	    mixer->sounds[SOUND_BACKGROUND] != NULL) {
		Mix_PlayChannel(-1, mixer->sounds[SOUND_BACKGROUND], -1);
		mixer->background_started = 1;
	}
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
