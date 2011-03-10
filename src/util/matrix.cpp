#include "matrix.h"
#include <cstring>

void makeOrtho(float left, float right, float top, float bottom, float near, float far, float matrix[16])
{
   float tx = (right + left)/(right - left);
   float ty = (top + bottom)/(top - bottom);
   float tz = (far + near)/(far - near);

   memset(matrix, 0,16);

   matrix[0] = 2.0/(right - left);
   matrix[5] = 2.0/(top - bottom);
   matrix[10] = -2.0/(far - near);
   matrix[12] = -tx;
   matrix[13] = -ty;
   matrix[14] = -tz;
   matrix[15] = 1;
}


