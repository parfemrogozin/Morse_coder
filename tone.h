#ifndef TONE_H_INCLUDED
#define TONE_H_INCLUDED

  #include <SDL2/SDL.h>

  enum SOUNDS
  {
    SILENCE = 0,
    DIT = 1,
    DAH = 2
  };

  SDL_AudioDeviceID tone_init(const double sample_rate, const double frequency, const float lenght_unit);
  void tone_destroy(SDL_AudioDeviceID device_id);
  void tone_encode_char(SDL_AudioDeviceID device_id, unsigned int ditdah);
  void tone_encode_string(SDL_AudioDeviceID device_id, unsigned int mes_encoded[80]);


#endif