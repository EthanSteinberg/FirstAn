#include <GL/glew.h>
#include <iostream>
#include <SFML/Window.hpp>
#include <string>
#include "util/util.h"
#include "util/text.h"
#include "util/glStuff.h"
#include "util/matrix.h"

struct inputData
{
   float position[3];
   float translation[3];
   float textcord[2];
   float rotation;

   float buffer[7];
};

inline
void * offset(int floatNum)
{
   return (void *)( sizeof(float) * floatNum);
}


using std::cout;
using std::endl;

int main(int argv, char **argc)
{
   
   printf("The size of it is %d, with offset at %ld, %ld, %ld and %ld\n",(int)sizeof(inputData),(long)offset(0),(long )offset(3),(long) offset(6),(long) offset(8));


   sf::Window App(sf::VideoMode(800, 600, 32), "SFML OpenGL");
   App.SetFramerateLimit(60);

   GLenum err = glewInit();

   if (GLEW_OK != err)
   {
      printf("Glew error: %s\n", glewGetErrorString(err));
      exit(1);
   }

   printf("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
   GLuint vertShader = createShader("../res/vert", GL_VERTEX_SHADER);
   GLuint fragShader = createShader("../res/frag", GL_FRAGMENT_SHADER);
   GLuint program = createProgram(vertShader,fragShader);


   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_BLEND);

   unsigned int textures[1];
   glGenTextures(1,textures);
   checkGLError();

   glActiveTexture(GL_TEXTURE0);
   checkGLError();
   
   glBindTexture(GL_TEXTURE_2D,textures[1]);
   checkGLError();

   loadTexture(GL_TEXTURE_2D,"../res/Wow.png");
   unsigned int buffers[2];

   glGenBuffers(2,buffers);
   checkGLError();

   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,buffers[1]);
   checkGLError();

   unsigned short indexes[60] = {};

   for (int l = 0; l < 60; l++)
   {
      int b  = l % 6;

      if (b < 3)
         indexes[l] = b;

      else
         indexes[l] = b -2;
      //printf("The number at %d at %d in %d\n",l,b,indexes[l]);
   }

   
   glBufferData(GL_ELEMENT_ARRAY_BUFFER,60 * sizeof(short),indexes,GL_STATIC_DRAW);
   checkGLError();


   glBindBuffer(GL_ARRAY_BUFFER,buffers[0]);
   checkGLError();

   glBufferData(GL_ARRAY_BUFFER,40 * sizeof(inputData),NULL,GL_STREAM_DRAW);
   checkGLError();

   glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof (inputData) ,offset(0));
   checkGLError();
   glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof (inputData) ,offset(3));
   checkGLError();
   glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof (inputData) ,offset(6));
   checkGLError();
   glVertexAttribPointer(3,1,GL_FLOAT,GL_FALSE,sizeof (inputData) ,offset(8));
   checkGLError();

   glEnableVertexAttribArray(0);
   checkGLError();
   glEnableVertexAttribArray(1);
   checkGLError();
   glEnableVertexAttribArray(2);
   checkGLError();
   glEnableVertexAttribArray(3);
   checkGLError();

   glBindAttribLocation(program,0,"in_Position");
   checkGLError();

   glBindAttribLocation(program,1,"in_Translation");
   checkGLError();

   glBindAttribLocation(program,2,"in_TextCord");
   checkGLError();

   glBindAttribLocation(program,3,"in_ZRotation");
   checkGLError();

   activateProgram(program);

   int perspectivePosition = glGetUniformLocation(program,"in_ProjectionMatrix");
   checkGLError();

   float matrix[16] = {};
   makeOrtho(0,100,100,0,-5,5,matrix);

   glUniformMatrix4fv(perspectivePosition,1,false,matrix);
   checkGLError();


   int texturePosition = glGetUniformLocation(program,"in_Texture");
   checkGLError();
   glUniform1i(texturePosition,0);
   checkGLError();

   inputData vertexes[40];

   //THEIR CODE BELOW______

   glClearDepth(1.f);
   glClearColor(0.f, .5f, 0.5f, 0.f);

   // Enable Z-buffer read and write
   //glEnable(GL_DEPTH_TEST);
   //glDepthMask(GL_TRUE);

   long theTime = getNano();
   int frames = 0;
   double rotation = 0;

   loadText();


   while (App.IsOpened())
   {
      
      frames++;
      //if (frames % 100 == 0)
      {
         printf("\rThe time is %f",1.0/((getNano() -  theTime) / 1.0e9));
         theTime = getNano();
      //printf("Wow really\n");
      }
      
      sf::Event Event;

      while (App.GetEvent(Event))
      {
         switch (Event.Type)
         {
         case
               sf::Event::Resized:
            {
               glViewport(0,0,Event.Size.Width, Event.Size.Height);
            }
            break;

         case sf::Event::Closed:
            exit(0);
            break;

         default:
            ;
            break;
         }
      }

      glClear(GL_COLOR_BUFFER_BIT);


      vertexes = {};

      int i = 0;
      //rotation++;

      vertexes[i].textcord[0] = 1076/2048.0;
      vertexes[i].textcord[1] = (1946 + 61) / 2048.0;
      vertexes[i].translation[0] = 70;
      vertexes[i].translation[1] = 70;
      vertexes[i].translation[2] = 0;
      vertexes[i].position[0] = -10;
      vertexes[i].position[1] = -10;
      vertexes[i].position[2] = 0;
      vertexes[i++].rotation = rotation;

      vertexes[i].textcord[0] = 1076/2048.0;
      vertexes[i].textcord[1] =  (1946) / 2048.0;
      vertexes[i].translation[0] = 70;
      vertexes[i].translation[1] = 70;
      vertexes[i].translation[2] = 0;
      vertexes[i].position[0] = -10;
      vertexes[i].position[1] = 10;
      vertexes[i].position[2] = 0;
      vertexes[i++].rotation = rotation;

      vertexes[i].textcord[0] = (1076 + 86)/2048.0;
      vertexes[i].textcord[1] =   (1946 + 61) / 2048.0;
      vertexes[i].translation[0] = 70;
      vertexes[i].translation[1] = 70;
      vertexes[i].translation[2] = 0;
      vertexes[i].position[0] = 20;
      vertexes[i].position[1] = -10;
      vertexes[i].position[2] = 0;
      vertexes[i++].rotation = rotation;

      vertexes[i].textcord[0] = (1076 + 86)/2048.0;
      vertexes[i].textcord[1] =  (1946) / 2048.0;
      vertexes[i].translation[0] = 70;
      vertexes[i].translation[1] = 70;
      vertexes[i].translation[2] = 0;
      vertexes[i].position[0] = 20;
      vertexes[i].position[1] = 10;
      vertexes[i].position[2] = 0;
      vertexes[i++].rotation = rotation;

      glBufferSubData(GL_ARRAY_BUFFER,0, 40 * sizeof(inputData),vertexes);
      checkGLError();

      glPointSize(20);
      glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,0);
      drawText();

      checkGLError();

      App.Display();
   }

   cout<<"Hello world"<<endl;
}
