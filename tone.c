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

int16_t get_sample(const double frequency, const double sample_rate, const unsigned long sample_index)
{
  double func_output = sin(sample_index * frequency * M_PI * 2 / sample_rate);
  return (int16_t) (func_output * 32567);
}

int16_t * create_sound(const double sample_rate, const double frequency, float lenght)
{
  unsigned long sample_no = (unsigned long) sample_rate * lenght;
  int16_t * sound = (int16_t *)calloc(sample_no, sizeof(int16_t));

  for (int i = 0; i < sample_no; i++)
  {
    sound[i] = get_sample(frequency, sample_rate, i);
  }

  return sound;
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
  Uint32 sound_lenght = 4800 * sizeof(int16_t);

  SDL_Init(SDL_INIT_AUDIO);
  SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);


  int16_t * dit = create_sound(48000, 800.0f, 1);
  int16_t * dah = create_sound(48000, 800.0f, 3);
  int16_t * silence = (int16_t *)calloc(48000, sizeof(int16_t));

  SDL_PauseAudioDevice(deviceId, 0); /* unpause FIRST */
  SDL_QueueAudio(deviceId, dit, sound_lenght);
  SDL_QueueAudio(deviceId, silence, sound_lenght);
  SDL_QueueAudio(deviceId, dah, sound_lenght * 3);
  SDL_QueueAudio(deviceId, silence, sound_lenght);
  SDL_QueueAudio(deviceId, dit, sound_lenght);
  SDL_QueueAudio(deviceId, silence, sound_lenght);
  SDL_QueueAudio(deviceId, silence, sound_lenght);
  SDL_QueueAudio(deviceId, silence, sound_lenght);

  free(dit);
  free(dah);
  SDL_Delay(900);
  SDL_CloseAudioDevice(deviceId);
  SDL_Quit();


  return 0;
}
