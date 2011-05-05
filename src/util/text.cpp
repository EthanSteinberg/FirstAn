#include "text.h"
#include <cassert>
#include <cmath>
#include "myVector2.h"

#include "algoMaxRects.h"

#include <cstdio>
#include <GL/glew.h>
#include <cstdlib>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
void checkFaceFlags(long faceFlags);

template <class T>
T nexthigher(T k)
{
   k--;

   for (unsigned int i=1; i<sizeof(T)*8; i<<=1)
   {
      k = k | k >> i;
   }

   return k+1;
}


char *buffer;
int side;

void drawText()
{
   glRasterPos2f(-1,-1);
   glDrawPixels(side, side,GL_RGBA, GL_UNSIGNED_BYTE,buffer);
}


void loadText()
{
   FT_Library lib;
   int error = FT_Init_FreeType(&lib);

   if (error)
   {
      printf("An error in loading the library\n");
      exit(0);
   }

   FT_Face face;
   error = FT_New_Face(lib,"../res/DejaVuSans.ttf",0,&face);

   if (error)
   {
      printf("An error in loading the face\n");
      exit(0);
   }


   long faceFlags = face->face_flags;
   //checkFaceFlags(faceFlags);

   int emUnits = face->units_per_EM;
   int numSizes = face->num_fixed_sizes;
   //printf("The units are %d with %d sizes\n",emUnits,numSizes);

   error = FT_Set_Char_Size(face,0,50 * 64, 120,120);

   if (error)
   {
      printf("An error in loading the face\n");
      exit(0);
   }


   const int start = 'A';

   const int end = 'B';

   FT_Glyph characters[end - start + 1];

   std::vector<t_myVector2> sizes(end - start + 1);

   int area = 0;

   for (int character = start; character <= end; character++)
   {
      int glyph = FT_Get_Char_Index(face, character);

      //printf("The piece of %c has an index %d \n",character,glyph);

      error = FT_Load_Glyph(face,glyph, FT_LOAD_RENDER  | FT_LOAD_TARGET_LCD);

      if (error)
      {
         printf("An error in loading the face\n");
         exit(0);
      }

      error = FT_Get_Glyph(face->glyph,&characters[character - start]);

      if (error)
      {
         printf("An error in loading the face\n");
         exit(0);
      }

      printf("The size was %d %d\n\n",face->glyph->bitmap.width, face->glyph->bitmap.rows);
      sizes[character - start] = t_myVector2(face->glyph->bitmap.width/3, face->glyph->bitmap.rows);

      area += face->glyph->bitmap.width/3 * face->glyph->bitmap.rows;
   }

   /*
   for (int i = 0 ;i < 26;i++)
   {
      printf("the thing %d is %d %d\n",i,sizes[i].x,sizes[i].y);
   }
   */

   side = 2 * nexthigher((int) sqrt(area));
   t_myVector2 size(side,side);

   bool done = 0;


   t_algoMaxRects algo;
   auto theMap = algo.pack(sizes,size,done);
   printf("\nThe side is %d and %d\n",side,done);


   //buffer = new char[side * side];
   buffer = (char *)calloc(side * side * 4,1);

   if (!buffer)
   {
      printf("No memory\n");
      exit(1);
   }

   for (int ch = start ; ch <= end; ch++)
   {
      t_myVector2 theSize = sizes[ch - start];
      FT_BitmapGlyph image = (FT_BitmapGlyph) characters[ch - start];
      auto iter = theMap.find(theSize);

      if (iter == theMap.end())
      {
         printf("Not an iter?\n");
         continue;
      }

      t_myVector2 theLocation = iter->second;
      theMap.erase(iter);

      //printf("The character %c at %d %d, with size %d %d\n",ch,theLocation.x,theLocation.y,theSize.x,theSize.y);



      for (int y = 0; y< theSize.y; y++)
      {

         for (int x = 0; x < theSize.x;x++)
         {
            int r  =  image->bitmap.buffer[ (y) * (image->bitmap.pitch) + x*3+0];
            int g  =  image->bitmap.buffer[ (y) * (image->bitmap.pitch) + x*3+1];
            int b  =  image->bitmap.buffer[ (y) * (image->bitmap.pitch) + x*3+2];
            buffer[theLocation.x*4 + (side -1 - (theLocation.y + y)) * side * 4 +x * 4 + 0] =  image->bitmap.buffer[ (y) * (image->bitmap.pitch) + x*3+0];
            buffer[theLocation.x*4 + (side -1 - (theLocation.y + y)) * side * 4 +x * 4 + 1] =  image->bitmap.buffer[ (y) * (image->bitmap.pitch) + x*3+1];
            buffer[theLocation.x*4 + (side -1 - (theLocation.y + y)) * side * 4 +x * 4 + 2] =  image->bitmap.buffer[ (y) * (image->bitmap.pitch) + x*3+2];
            //buffer[theLocation.x + (side -1 - (theLocation.y + y)) * side +x+ 3] =  image->bitmap.buffer[ (y) * (image->bitmap.pitch) + x+3];
            buffer[theLocation.x*4 + (side -1 - (theLocation.y + y)) * side * 4 +x * 4 + 3] =  (r+ g +b )/3 ;
         }

         // printf("Writing to %d %d\n",(y + theLocation.y),(theSize.x + theLocation.x));
         //memcpy(buffer + theLocation.x + (side - 1 - (theLocation.y + y)) * side,image->bitmap.buffer + (y) *(image->bitmap.pitch) ,theSize.x);
      }


   }

   
   /*
   
   for (int y = 0; y < side;y++)
   {
      for (int x = 0; x < side;x++)
      {
         for (int i = 0; i < 3;i++)
         printf("%2d ",(unsigned char)buffer[y * side + x * 4 + i]);

         printf("   ");
      }
      putc('\n',stdout);
   }
   */
   
   
}

void checkFaceFlags(long faceFlags)
{
   if (faceFlags & FT_FACE_FLAG_SCALABLE)
   {
      printf("It is scalable\n");
   }

   if (faceFlags & FT_FACE_FLAG_FIXED_SIZES)
   {
      printf("It is fixed sizes\n");
   }

   if (faceFlags & FT_FACE_FLAG_FIXED_WIDTH)
   {
      printf("It is fixed width\n");
   }

   if (faceFlags & FT_FACE_FLAG_SFNT)
   {
      printf("It is sfnt?\n");
   }

   if (faceFlags & FT_FACE_FLAG_HORIZONTAL)
   {
      printf("It is horizontal\n");
   }

   if (faceFlags & FT_FACE_FLAG_VERTICAL)
   {
      printf("It is vertical\n");
   }

   if (faceFlags & FT_FACE_FLAG_KERNING)
   {
      printf("It is kerning\n");
   }

   if (faceFlags & FT_FACE_FLAG_FAST_GLYPHS)
   {
      printf("It is fast glyphs\n");
   }

   if (faceFlags & FT_FACE_FLAG_MULTIPLE_MASTERS)
   {
      printf("It is multiple masters\n");
   }

   if (faceFlags & FT_FACE_FLAG_GLYPH_NAMES)
   {
      printf("It is glyph names\n");
   }

   if (faceFlags & FT_FACE_FLAG_EXTERNAL_STREAM)
   {
      printf("It is external stream\n");
   }

   if (faceFlags & FT_FACE_FLAG_HINTER)
   {
      printf("It is hinter\n");
   }

   if (faceFlags & FT_FACE_FLAG_CID_KEYED)
   {
      printf("It is cid keyed\n");
   }

   if (faceFlags & FT_FACE_FLAG_TRICKY)
   {
      printf("It is tricky\n");
   }
}
