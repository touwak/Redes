#include "stdio.h"
#include "winsock2.h"

struct clientes{
  int id;
  char nick[50];
  struct sockaddr_in ip;
};


int main(){

  WSADATA wsa;
  SOCKET sock, sock_c;
  struct sockaddr_in ip, ip_c;
  int size, bytes = 0;
  char buff[512];
  memset(buff, '\0', 512);
  WSAStartup(MAKEWORD(2, 0), &wsa);
  sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  ip.sin_family = AF_INET;
  ip.sin_addr.s_addr = inet_addr("0.0.0.0");
  ip.sin_port = htons(9999);
  bind(sock, (SOCKADDR*)&ip, sizeof(ip));
  size = sizeof(ip);

  while (1){
    bytes = recvfrom(sock, buff, 512, 0, (SOCKADDR *)&ip_c, &size);
    if (bytes > 0){
      printf("\nConectado: %s", inet_ntoa(ip_c.sin_addr));
      printf("msn: %s", buff);
      memset(buff, '\0', 512);
    }
  }

  return 0;
}