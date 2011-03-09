#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <string>

std::string loadFile(const std::string &filename);
void printBacktrace();
void checkGLError();

unsigned int createShader(const std::string &filename, int type);
unsigned int createProgram(unsigned int vertShader, unsigned int fragShader);

unsigned int getNano();

#endif 
