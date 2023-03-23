#include "morsecode.h"

const char letter[N_LETTERS] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
                        'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
                        'U', 'V', 'W', 'X', 'Y', 'Z'};

const unsigned int ditdah[N_LETTERS] = {0b1101, 0b01010111, 0b01110111, 0b010111, 0b01,
                        0b01110101, 0b011111, 0b01010101, 0b0101,
                        0b11111101, 0b110111, 0b01011101, 0b1111, 0b0111,
                        0b111111, 0b01111101, 0b11011111, 0b011101,
                        0b010101, 0b11, 0b110101, 0b11010101, 0b111101,
                        0b11010111, 0b11110111, 0b01011111};


int search_array(const char needle, const char * haystack)
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

void get_ditdah(char * input, unsigned int * output)
{
  int common_index = -1;
  int output_index = 0;
  while (*input)
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
  output[output_index] = '\0';
}

