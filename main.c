#include <stdio.h>
#include <readline/readline.h>
#include "morsecode.h"
#include "mor_print.h"
#include "tone.h"

int main(int argc, char **argv)
{
  char * mes_input = NULL;
  unsigned int mes_encoded[80] = {0};
  char mes_output[80*6] = {'\0'};
  Uint32 queue = 0;
  int run = 1;



  SDL_AudioDeviceID device_id = tone_init(48000, 800, 0.1f);



  while ( run )
  {
    mes_input = readline ("message> ");

    if ( strcmp(mes_input, "SK") == 0 )
    {
      run = 0;
      tone_encode_char(device_id, 0b110111010101);
      puts("+");
    }
    else
    {
      get_ditdah(mes_input, mes_encoded);
      free(mes_input);
      get_dot_dash_string(mes_encoded, mes_output);
      printf("%s\n", mes_output);
      memset(mes_output, 0, sizeof(mes_output));
      tone_encode_string(device_id, mes_encoded);
    }
  }

  do
  {
    SDL_Delay(1000);
    queue = SDL_GetQueuedAudioSize(device_id);
  }
  while (queue > 0);

  tone_destroy(device_id);

  return 0;
}

