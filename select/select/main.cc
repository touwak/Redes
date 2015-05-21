#include "stdio.h"
#include "winsock2.h"

int main(){
  WSADATA wsa;
  SOCKET sock;
  fd_set sock_in;
  struct timeval time;
  struct sockaddr_in ip;
  int size = sizeof(ip);
  WSAStartup(MAKEWORD(2, 0), &wsa);
  sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  ip.sin_addr.s_addr = inet_addr("127.0.0.1");
  ip.sin_port = htons(9999);
  ip.sin_family = AF_INET;
  bind(sock, (SOCKADDR*)&ip, sizeof(ip));
  listen(sock, SOMAXCONN);
  time.tv_sec = 4;
  time.tv_usec = 0;
  while (true)
  {
    FD_ZERO(&sock_in);
    FD_SET(sock, &sock_in);
    select(0, &sock_in, NULL, NULL, &time);
    if (FD_ISSET(sock, &sock_in)){
      printf("\nNueva Conexion\n");
    }
    else{
      printf("\nTimeOut\n");
    }
  }
  system("pause");
  return 0;
}