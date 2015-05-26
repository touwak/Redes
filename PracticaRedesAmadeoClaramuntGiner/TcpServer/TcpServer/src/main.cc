#include "server.h"


int main(int argc, char** argv){
  int port = 0;


  if (argv[1] != NULL){
    port = atoi(argv[1]);
  }
  else{
    port = 8080;
  }

  Server::init(port);
 
  
  return 0;
}