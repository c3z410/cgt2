#ifndef _MYGL_H_
#define _MYGL_H_

#include "definitions.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <chrono>

//*****************************************************************************
// Defina aqui as suas funções gráficas
//*****************************************************************************

inline Color lerp(Color c1, Color c2, float t)
{
  return {
    c1.red + (c2.red - c1.red) * t,
    c1.green + (c2.green - c1.green) * t,
    c1.blue + (c2.blue - c1.blue) * t,
    c1.alpha + (c2.alpha - c1.alpha) * t,
  };
}

void PutPixel(Point point, Color color)
{
  int offset = point.x * 4 + point.y * IMAGE_WIDTH * 4;

  FBptr[offset] = round(color.red);
  FBptr[offset + 1] = round(color.green);
  FBptr[offset + 2] = round(color.blue);
  FBptr[offset + 3] = round(color.alpha);
}

void DrawLine(Point initial, Point final, Color initial_color, Color final_color)
{

  int dx = final.x - initial.x;
  int dy = final.y - initial.y;
  double m = (double) dy / (double) dx;
  int d, incr_e, incr_ne, incr_xa, incr_xb, incr_ya, incr_yb;

  if (!dx) { // linha vertical
    if (dy < 0) {
      DrawLine(final, initial, final_color, initial_color);
      return;
    }

    PutPixel(initial, initial_color);

    while (initial.y < final.y) {
      PutPixel(initial, lerp(initial_color, final_color, 1 - (double)(final.y - initial.y)/dy));
      initial.y++;
    }
  }

  if (m >= 0 && m <= 1) { // 1º e 5º octantes
    if (dx < 0) { // 5º octante
        DrawLine(final, initial, final_color, initial_color);
        return;
    }

    d = 2 * dy - dx;
    incr_e = 2 * dy;
    incr_ne = 2 * (dy - dx);

    incr_xa = incr_xb = incr_yb = 1;
    incr_ya = 0;

  } else if (m > 1) { // 2º e 6° octantes
    if (dx < 0) { // 6º octante
      DrawLine(final, initial, final_color, initial_color);
      return;
    }

    d = dy -2 * dx;
    incr_e = 2 * (dy - dx);
    incr_ne =  -2 * dx;

    incr_xa = incr_ya = incr_yb = 1;
    incr_xb = 0;

  } else if (m <= -1) { // 3º e 7º octantes
    if (dx < 0) { // 7º octante
      DrawLine(final, initial, final_color, initial_color);
      return;
    }

    d = dy + 2 * dx;
    incr_e = 2 * dx;
    incr_ne =  2 * (dy + dx);

    incr_xa = 0;
    incr_ya = incr_yb = -1;
    incr_xb = 1;

  } else if (m > -1 && m < 0) { // 4º e 8º octantes
    if (dx < 0) {
      DrawLine(final, initial, final_color, initial_color);
      return;
    }

    d = 2 * dy + dx;
    incr_e = 2 * (dy + dx);
    incr_ne =  2 * dy;

    incr_xa = incr_xb = 1;
    incr_ya = -1;
    incr_yb = 0;

  }

  PutPixel(initial, initial_color);

  while (initial.x < final.x) {
    if (d <= 0) {
      d += incr_e;
      initial.x +=  incr_xa;
      initial.y += incr_ya;
    } else {
      d += incr_ne;
      initial.x += incr_xb;
      initial.y += incr_yb;
    }

    PutPixel(initial, lerp(initial_color, final_color, 1 - (double)(final.x - initial.x)/dx));
    
  }

}

void DrawTriangle(Point p1, Point p2, Point p3, Color c1, Color c2, Color c3)
{
  DrawLine(p1, p2, c1, c2);
  DrawLine(p1, p3, c1, c3);
  DrawLine(p2, p3, c2, c3);
}

#endif // _MYGL_H_
