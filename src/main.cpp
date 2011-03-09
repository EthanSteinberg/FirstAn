#include <GL/glew.h>
#include <iostream>
#include <SFML/Window.hpp>
#include <string>
#include "util/util.h"

using std::cout;
using std::endl;

int main(int argv, char **argc)
{

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
   
   glUseProgram(program);
   checkGLError();

   unsigned int buffers[1];
   
   glGenBuffers(1,buffers);
   checkGLError();

   glBindBuffer(GL_ARRAY_BUFFER,buffers[0]);
   checkGLError();

   glBufferData(GL_ARRAY_BUFFER,40 * 3 * sizeof(float),NULL,GL_STREAM_DRAW);
   checkGLError();

   glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof (GLfloat) * 2,0);
   checkGLError();

   glEnableVertexAttribArray(0);
   checkGLError();

   glBindAttribLocation(program,0,"in_Position");
   checkGLError();

   float vertexes[40 * 3];

   //THEIR CODE BELOW______

   glClearDepth(1.f);
   glClearColor(1.f, 1.f, 1.f, 1.f);

   // Enable Z-buffer read and write
   //glEnable(GL_DEPTH_TEST);
   //glDepthMask(GL_TRUE);

   // Setup a perspective projection
   //glMatrixMode(GL_PROJECTION);
   //glLoadIdentity();
   //gluPerspective(90.f, 1.f, 1.f, 500.f);

   long theTime = getNano();
   int frames = 0;


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

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


      int i = 0;

      vertexes[i++] = .4;
      vertexes[i++] = 0;
      vertexes[i++] = 0;

      vertexes[i++] = .8;
      vertexes[i++] = 0;
      vertexes[i++] = 0;

      vertexes[i++] = .2;
      vertexes[i++] = 0;
      vertexes[i++] = 0;

      glBufferSubData(GL_ARRAY_BUFFER,0, 40 * 3 * sizeof(float),vertexes);
      checkGLError();

      glDrawArrays(GL_TRIANGLES,0,i);
      checkGLError();

      App.Display();
   }






   cout<<"Hello world"<<endl;
}
