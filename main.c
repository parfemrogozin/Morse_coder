#include <stdio.h>
#include "morsecode.h"
#include "mor_print.h"

static inline int search_array(const char needle, const char * haystack)
{
  int index = -1;
  for (int i = 0; i < N_LETTERS; i++)
  {
    if ( needle == haystack[i] )
    {
      index = i;
      break;
    }
  }
  return index;
}

void get_ditdah(char * input, char * output)
{
  int common_index = -1;
  int output_index = 0;
  while (*input != '\n') /* reklace with 'K' */
  {
    common_index = search_array(*input, letter);
    if ( common_index > -1)
    {
      output[output_index] = ditdah[common_index];
      output_index++;
    }
    else if (*input == ' ')
    {
      output[output_index] = 0b10;
      output_index++;
    }
    input++;
  }
  output[output_index] = 0b10;
  output[output_index+1] = ditdah[10]; /* 'K' */
  output[output_index+2] = '\0';
}


int main(int argc, char **argv)
{
  char mes_input[80];
  char mes_encoded[80];
  char mes_output[80*7];


  fgets(mes_input, 80, stdin);

  get_ditdah(mes_input, mes_encoded);
  get_dot_dash_string(mes_encoded, mes_output);
  printf("%s\n", mes_output);

  return 0;
}

