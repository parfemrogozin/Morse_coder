/*
*  A 0b1101
* B 0b01010111
* C 0b01110111
* D 0b010111
* E 0b01
* F 0b01110101
* G 0b011111
* H 0b01010101
* I 0b0101
* J 0b11111101 !
* K 0b110111
* L 0b01011101
* M 0b1111
* N 0b0111
* O 0b111111
* P 0b01111101
* Q 0b11011111 !
* R 0b011101
* S 0b010101
* T 0b11
* U 0b110101
* V 0b11010101
* W 0b111101
* X 0b11010111 !
* Y 0b11110111 !
* Z 0b01011111
*
* PROSIGN_AR 0b0111011101
*/

#ifndef MORSECODE_H_INCLUDED
#define MORSECODE_H_INCLUDED
  #define N_LETTERS 26

  extern const char letter[];
  extern const unsigned int ditdah[];


  int search_array(const char needle, const char * haystack);
  void get_ditdah(char * input, unsigned int * output);
#endif
