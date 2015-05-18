#include "server.h"


int main(int argc, char** argv){
  int port = 8080;

  printf("introduce el puerto a escuchar:\n");
  scanf("&d", port);

  Server::init(port, "www/index.htm");
  
  return 0;
}