#include "util.h"
#include <boost/scoped_ptr.hpp>
#include <png.h>
#include "glStuff.h"

#include <GL/glew.h>
#include <cstdio>

void checkGLError()
{
   while (true)
   {
      switch (glGetError())
      {
      case GL_NO_ERROR:
         return;

      case GL_INVALID_ENUM:
         printf("GL Invalid enum error\n\n");
         printBacktrace();
         exit(1);
         break;

      case GL_INVALID_VALUE:
         printf("GL Invalid value error\n\n");
         printBacktrace();
         exit(1);
         break;
      case GL_INVALID_OPERATION:
         printf("GL Invalid operation error\n\n");
         printBacktrace();
         exit(1);
         break;
      case GL_STACK_OVERFLOW:
         printf("GL Stack overflow error\n\n");
         printBacktrace();
         exit(1);
         break;
      case GL_STACK_UNDERFLOW:
         printf("GL Stack underflow error\n\n");
         printBacktrace();
         exit(1);
         break;
      case GL_OUT_OF_MEMORY:
         printf("GL Out of memory error\n\n");
         printBacktrace();
         exit(1);
         break;

      case GL_TABLE_TOO_LARGE:
         printf("GL Table too large error\n\n");
         printBacktrace();
         exit(1);
         break;
      }
   }
}

unsigned int createProgram(unsigned int vertShader, unsigned int fragShader)
{
   unsigned int program = glCreateProgram();

   if (!program)
   {
      printf("Create program error ");
      checkGLError();
   }

   glAttachShader(program,vertShader);
   checkGLError();

   glAttachShader(program,fragShader);
   checkGLError();

   return program;
}

void activateProgram(unsigned int program)
{

   glLinkProgram(program);
   checkGLError();

   int param;
   glGetProgramiv(program,GL_LINK_STATUS,&param);
   checkGLError();

   if (param == GL_FALSE)
   {
      printf("Problem linking the program\n");

      glGetProgramiv(program,GL_INFO_LOG_LENGTH,&param);
      checkGLError();

      char *logBuffer = new char[param];

      glGetProgramInfoLog(program,param,NULL,logBuffer);
      checkGLError();
      printf("\n%s\n",logBuffer);


      exit(1);
   }

   glValidateProgram(program);
   checkGLError();

   glGetProgramiv(program,GL_VALIDATE_STATUS,&param);
   checkGLError();

   if (param == GL_FALSE)
   {
      glGetProgramiv(program,GL_INFO_LOG_LENGTH,&param);
      checkGLError();

      char *logBuffer = new char[param];

      glGetProgramInfoLog(program,param,NULL,logBuffer);
      checkGLError();
      printf("\n%s\n",logBuffer);
      printf("Problem validating the program\n");
      exit(1);
   }

   glUseProgram(program);
   checkGLError();

   glGetProgramiv(program,GL_INFO_LOG_LENGTH,&param);
   checkGLError();

   char *logBuffer = new char[param];

   glGetProgramInfoLog(program,param,NULL,logBuffer);
   checkGLError();
   printf("\n%s\n",logBuffer);

}
unsigned int createShader(const std::string &filename, int shaderType)
{
   std::string theFile = loadFile(filename);
   printf("I am loading the shader %s\n",filename.c_str());

   unsigned int shader = glCreateShader(shaderType);
   checkGLError();

   const char *data = theFile.c_str();
   glShaderSource(shader,1,&(data),NULL);
   checkGLError();

   glCompileShader(shader);
   checkGLError();

   int param;
   glGetShaderiv(shader,GL_COMPILE_STATUS,&param);
   checkGLError();

   if (param == GL_FALSE)
   {
      //printf("Your shader failed to compile for some reason\n");
      glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&param);
      checkGLError();

      char *logBuffer = new char[param];

      glGetShaderInfoLog(shader,param,NULL,logBuffer);
      checkGLError();
      //printf("Printing out the log of information...\n\n%s\n",logBuffer);
      printf("\n%s\n",logBuffer);
   }

   return shader;
}

void loadTexture(int target, const std::string &filename)
{
   FILE *theFile = fopen(filename.c_str(),"rb");

   if (!theFile)
   {
      perror("Fopen failed");
      exit(1);
   }

   char header[8];
   fread(header,1,8,theFile);

   if (ferror(theFile))
   {
      perror("Fread failed");
      exit(1);
   }

   if (png_sig_cmp((png_byte *) header,0,8))
   {
      printf("The file is not a png\n");
      exit(1);
   }

   png_structp png_ptr = png_create_read_struct(
                            PNG_LIBPNG_VER_STRING,
                            NULL,NULL,NULL
                         );

   if (!png_ptr)
   {
      printf("Failed initialization of png_ptr\n");
      exit(1);
   }

   png_infop info_ptr = png_create_info_struct(png_ptr);

   if (!info_ptr)
   {
      png_destroy_read_struct(&png_ptr,
                              (png_infopp)NULL, (png_infopp)NULL);
      printf("Info ptr creation failed\n");
      exit(1);
   }

   png_infop end_info = png_create_info_struct(png_ptr);

   if (!end_info)
   {
      png_destroy_read_struct(&png_ptr, &info_ptr,
                              (png_infopp)NULL);
      printf("Info ptr creation failed\n");
      exit(1);
   }

   png_init_io(png_ptr,theFile);
   png_set_sig_bytes(png_ptr,8);

   png_read_info(png_ptr,info_ptr);
   int width = png_get_image_width(png_ptr,info_ptr);
   int height = png_get_image_width(png_ptr,info_ptr);
   int depth = png_get_bit_depth(png_ptr,info_ptr);
   int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

   boost::scoped_ptr<png_byte> image_data(new png_byte[rowbytes * height]);

   png_bytep *row_pointers = new png_bytep [height];
   for (int i = 0; i < height; ++i)
           row_pointers[i] = image_data.get() + i * rowbytes;


   printf("I have read the image with width %d, height %d, depth %d, rowbytes %d\n",width,height,depth,rowbytes);

   png_read_image(png_ptr,row_pointers);

   png_read_end(png_ptr,NULL);
   png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);

   
   /*
   for (int i = 0;i < height;i++)
   {
      for (int b = 0; b< width * 4;b++)
      {
         printf("%3d ",(int) image_data.get()[i * rowbytes + b]);
         if (b%4 == 3)
            putchar('\t');
      }
      putchar('\n');
   }
   */
   
  

   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,image_data.get());
   checkGLError();

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   checkGLError();

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   checkGLError();

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
   checkGLError();

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
   checkGLError();
   
}
