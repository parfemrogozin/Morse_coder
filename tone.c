#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "tone.h"

/*
  * short mark, dot or dit ( . ): 1
  * long mark, dash or dah ( - ): 3
  * inter-element gap between the dits and dahs within a character: 1
  * short gap (between letters): 3
  * medium gap (between words): 7
  *
*/

struct wave_settings
{
  double sample_rate;
  double frequency;
  float lenght_unit;
};

static struct wave_settings SETTINGS = {48000, 800, 0.3f};
static int16_t * TABLE[3];

static int16_t get_sample(const unsigned int sample_index, double sample_rate, double frequency)
{
  double func_output = sin(sample_index * frequency * M_PI * 2 / sample_rate);
  return (int16_t) (func_output * 32567);
}

static int16_t * create_sound(float lenght)
{
  unsigned int sample_no = (unsigned int) SETTINGS.sample_rate * lenght * SETTINGS.lenght_unit;
  int16_t * sound = (int16_t *)calloc(sample_no, sizeof(int16_t));

  for (int i = 0; i < sample_no; i++)
  {
    sound[i] = get_sample(i, SETTINGS.sample_rate, SETTINGS.frequency);
  }

  return sound;
}

static int create_sound_table()
{
  TABLE[SILENCE] = (int16_t *)calloc(SETTINGS.sample_rate * SETTINGS.lenght_unit, sizeof(int16_t));
  TABLE[DIT] = create_sound(1);
  TABLE[DAH] = create_sound(3);
  return 0;
}

static void destroy_sound_table()
{
  for (int i = 0; i < 3; i++)
  {
    free(TABLE[i]);
  }
}

SDL_AudioDeviceID tone_init(const double sample_rate, const double frequency, const float lenght_unit)
{
  SETTINGS.sample_rate = sample_rate;
  SETTINGS.frequency = frequency;
  SETTINGS.lenght_unit = lenght_unit;

  create_sound_table();

  SDL_AudioSpec spec =
  {
  .freq = SETTINGS.sample_rate,
  .format = AUDIO_S16SYS,
  .channels = 1,
  .samples = 4096,
  .callback = NULL,
  .userdata = NULL
  };

  SDL_Init(SDL_INIT_AUDIO);
  SDL_AudioDeviceID device_id = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);

  SDL_PauseAudioDevice(device_id, 0);
  return device_id;
}

void tone_destroy(SDL_AudioDeviceID device_id)
{
  /*SDL_PauseAudioDevice(device_id, 1);*/
  destroy_sound_table();
  SDL_CloseAudioDevice(device_id);
  SDL_Quit();
}

void snd_encode_char(SDL_AudioDeviceID deviceId, char ditdah)
{
  int signal = 1;
  const char mask = 0b00000011;
  char shifted_mask;
  char shift = 0;
  char flag;

  Uint32 len = SETTINGS.sample_rate * SETTINGS.lenght_unit * sizeof(int16_t);

  if ( ditdah == 0b10 )
  {
    for (int i = 0; i < 7; i++)
    {
      SDL_QueueAudio(deviceId, TABLE[SILENCE], len);
    }
    signal = 0;
  }

  while (signal)
  {
    shifted_mask = mask << shift;
    flag = shifted_mask & ditdah;
    flag = flag >> shift;

    switch(flag)
    {
      case 1:
        SDL_QueueAudio(deviceId, TABLE[DIT], len);
        SDL_QueueAudio(deviceId, TABLE[SILENCE], len);
        break;
      case 3:
        SDL_QueueAudio(deviceId, TABLE[DAH], len * 3);
        SDL_QueueAudio(deviceId, TABLE[SILENCE], len);
        break;
      default:
        signal = 0;
        break;
    }
    shift += 2;
  }

}


int main(int argc, char **argv)
{
  SDL_LogSetPriority(SDL_LOG_CATEGORY_AUDIO, SDL_LOG_PRIORITY_VERBOSE);

  SDL_AudioDeviceID device_id = tone_init(48000, 800, 0.1f);

  snd_encode_char(device_id, 0b01010111);
  Uint32 queue = 0;
  SDL_LogDebug(SDL_LOG_CATEGORY_AUDIO, "queued: %d", queue);
  do
  {
    SDL_Delay(1);
    queue = SDL_GetQueuedAudioSize(device_id);
  }
  while (queue > 0);


  tone_destroy(device_id);

  return 0;
}
