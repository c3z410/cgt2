#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#define IMAGE_WIDTH  512 // número de colunas da imagem.
#define IMAGE_HEIGHT 512 // número de linhas da imagem.

unsigned char * FBptr;

struct Point {
    int x;
    int y;
};

struct Color {
  int red;
  int green;
  int blue;
  int alpha;
};

#endif // _DEFINITIONS_H_
