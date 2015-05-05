#include "stdio.h"
#include "winsock2.h"


int mainc()
{
  WSADATA wsa;
  SOCKET sock;
  struct sockaddr_in ip;

  WSAStartup(MAKEWORD(2, 0), &wsa);
  sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  //IP
  ip.sin_family = AF_INET;
  ip.sin_port = htons(27000);
  ip.sin_addr.s_addr = inet_addr("127.0.0.1");

  if (!connect(sock, (SOCKADDR*)&ip, sizeof(ip))){
    printf("\n Conectado correctamente\n");
  }
  system("PAUSE");
  return 0;
}