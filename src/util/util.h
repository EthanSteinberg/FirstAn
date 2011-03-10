#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <string>

std::string loadFile(const std::string &filename);
void printBacktrace();



unsigned int getNano();

#endif 
