#ifndef TONE_H_INCLUDED
#define TONE_H_INCLUDED

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

  int16_t get_sample(struct wave_settings settings, const unsigned int sample_index);
  int16_t * create_sound(struct wave_settings settings, float lenght);
  int create_sound_table(struct wave_settings settings, int16_t * table[3]);
  void destroy_sound_table(int16_t * table[3]);
  void insert_silence(SDL_AudioDeviceID deviceId, int16_t * sound, struct wave_settings settings, const unsigned int times);
  void snd_encode_char(SDL_AudioDeviceID deviceId, struct wave_settings settings, int16_t * table[3], char ditdah);

#endif