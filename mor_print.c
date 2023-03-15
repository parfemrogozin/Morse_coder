#include <stddef.h>
#include <string.h>
#include "mor_print.h"

void get_dot_dash_char(unsigned int ditdah, char * letter_buffer)
{
  unsigned int tone;
  const unsigned int mask = 0b00000011;
  unsigned int shifted_mask;
  unsigned int shift = 0;
  size_t i = 0;
  int signal = 1;
  if ( ditdah == 0b10 )
  {
    signal = 0;
    letter_buffer[i] = ' ';
    i++;
  }

  while (signal)
  {
    shifted_mask = mask << shift;
    tone = shifted_mask & ditdah;
    tone = tone >> shift;

    switch(tone)
    {
      case 1:
        letter_buffer[i] = '.';
        break;
      case 3:
        letter_buffer[i] = '-';
        break;
      default:
        letter_buffer[i] = ' ';
        signal = 0;
        break;
    }
    i++;
    shift += 2;
  }
  letter_buffer[i] = '\0';

}

void get_dot_dash_string(unsigned int * input, char * output)
{
  char buffer[6];
  size_t i = 0;
  while (*input != '\0')
  {
    get_dot_dash_char(*input, buffer);
    strncat(output, buffer, 5);
    input++;
  }
  buffer[i+1] = '\0';
}

