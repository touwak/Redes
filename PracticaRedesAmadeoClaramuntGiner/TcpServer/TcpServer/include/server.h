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
  //initialize the server and charge the web
  int init(int port);
  //read the web files
  char* readFile(const char* file_path, int * out_put_size);
  //read the image files
  char* readImage(const char* file_path, int * out_put_size);
};
#endif // !__SERVER_H__