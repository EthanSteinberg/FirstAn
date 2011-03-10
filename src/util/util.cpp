#include <ctime>
#include "util.h"

#include <cstdio>
#include <execinfo.h>

std::string loadFile(const std::string &filename)
{
   FILE *theFile = fopen(filename.c_str(),"r");

   if (!theFile)
   {
      perror("Fopenin loadFile: ");
      exit(1);
   }

   if (fseek(theFile,0,SEEK_END))
   {
      perror("Fseek loadFile: ");
      exit(1);
   }

   long size = ftell(theFile);

   if (fseek(theFile,0,SEEK_SET))
   {
      perror("Fseek loadFile: ");
      exit(1);
   }

   char *data = new char [size];

   fread(data,sizeof(char),size,theFile);

   data[size -1] = 0;
   std::string output = data;

   fclose(theFile);
   return output;
}

void printBacktrace()
{
   const int size = 100;
   void *buffer[size];

   int nptrs = backtrace(buffer, size);
   printf("backtrace() returned %d addresses\n",nptrs);

   int blah = 1/0;
   //backtrace_symbols_fd(buffer,nptrs, STDOUT_FILENO);
}


unsigned int getNano()
{
   timespec res;

   if (clock_gettime(CLOCK_MONOTONIC,&res))
   {
      perror("Clock");
      exit(1);
   }

   return res.tv_nsec;
}

