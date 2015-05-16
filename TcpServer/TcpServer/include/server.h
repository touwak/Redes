#ifndef __SERVER_H__
#define __SERVER_H__

#include "stdio.h"
#include "winsock2.h"
#include <string.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>


namespace Server
{
  int init(int port, const char* file_path);
  char* readFile(const char* file_path, int * out_put_size);
  char* readImage(const char* file_path, int * out_put_size);
};
#endif // !__SERVER_H__