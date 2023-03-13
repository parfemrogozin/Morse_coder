#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>

/*
  * short mark, dot or dit ( . ): 1
  * long mark, dash or dah ( - ): 3
  * inter-element gap between the dits and dahs within a character: 1
  * short gap (between letters): 3
  * medium gap (between words): 7
  *
*/

enum SOUNDS
{
  SILENCE = 0,
  DIT = 1,
  DAH = 2
};

struct wave_settings
{
  double frequency;
  double sample_rate;
};

int16_t get_sample(struct wave_settings settings, const unsigned int sample_index)
{
  double func_output = sin(sample_index * settings.frequency * M_PI * 2 / settings.sample_rate);
  return (int16_t) (func_output * 32567);
}

int16_t * create_sound(struct wave_settings settings, float lenght)
{
  unsigned int sample_no = (unsigned long) settings.sample_rate * lenght;
  int16_t * sound = (int16_t *)calloc(sample_no, sizeof(int16_t));

  for (int i = 0; i < sample_no; i++)
  {
    sound[i] = get_sample(settings, i);
  }

  return sound;
}

int create_sound_table(struct wave_settings settings, int16_t * table[3])
{
  table[SILENCE] = (int16_t *)calloc(settings.sample_rate, sizeof(int16_t));
  table[DIT] = create_sound(settings, 1);
  table[DAH] = create_sound(settings, 3);
  return 0;
}

void destroy_sound_table(int16_t * table[3])
{
  for (int i = 0; i < 3; i++)
  {
    free(table[i]);
  }
}

void insert_silence(SDL_AudioDeviceID deviceId, int16_t * sound, struct wave_settings settings, const unsigned int times)
{
  Uint32 len = settings.sample_rate * sizeof(int16_t);
  for (int t = 0; t < times; t++)
  {
    SDL_QueueAudio(deviceId, sound, len);
  }
}

void snd_encode_char(SDL_AudioDeviceID deviceId, struct wave_settings settings, int16_t * table[3], char ditdah)
{
  int signal = 1;
  const char mask = 0b00000011;
  char shifted_mask;
  char shift = 0;
  char flag;

  Uint32 len = settings.sample_rate * sizeof(int16_t);

  if ( ditdah == 0b10 )
  {
    insert_silence(deviceId, table[SILENCE], settings, 7);
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
        SDL_QueueAudio(deviceId, table[DIT], len);
        insert_silence(deviceId, table[SILENCE], settings, 1);
        break;
      case 3:
        SDL_QueueAudio(deviceId, table[DAH], len * 3);
        insert_silence(deviceId, table[SILENCE], settings, 1);
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
  SDL_AudioSpec spec =
  {
  .freq = 48000,
  .format = AUDIO_S16SYS,
  .channels = 1,
  .samples = 4096,
  .callback = NULL,
  .userdata = NULL
  };

  struct wave_settings settings = {800, 48000};
  int16_t * table[3];
  create_sound_table(settings, table);

  SDL_Init(SDL_INIT_AUDIO);
  SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);
  SDL_PauseAudioDevice(deviceId, 0);

  snd_encode_char(deviceId, settings, table, 0b01010111);

  SDL_Delay(9000);
  SDL_CloseAudioDevice(deviceId);
  SDL_Quit();


  return 0;
}
