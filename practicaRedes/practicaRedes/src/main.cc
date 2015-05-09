#include "server.h"


int main(int argc, char** argv){
  
  Server::init(8080, "assets/index.htm");
  
  return 0;
}