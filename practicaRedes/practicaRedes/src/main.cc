#include "stdio.h"
#include "winsock2.h"


int main(int argc, char** argv){
  int i;
  WSADATA wsa;
  SOCKET sock;
  struct sockaddr_in ip;
  if (WSAStartup(MAKEWORD(2, 0), &wsa)){
    printf("Error!");
    return 1;
  }

  sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (sock == INVALID_SOCKET){
    printf("ERROR en socket(): %ld\n", WSAGetLastError());
    WSACleanup();
    return 1;
  }

  ip.sin_family = AF_INET;
  ip.sin_addr.s_addr = inet_addr("127.0.0.1");
  ip.sin_port = htons(8080);
  printf("IP y puerto asignado: %s: %d", inet_ntoa(ip.sin_addr), ntohs(ip.sin_port));

  if (bind(sock, (SOCKADDR*)&ip, sizeof(ip)) == SOCKET_ERROR){
    printf("\nERROR en bind: %ld\n", WSAGetLastError());
    return 1;
  }
  if (listen(sock, SOMAXCONN)){
    printf("ERROR en apertura de puerto");
    return 1;
  }

  system("pause");
  closesocket(sock);
  WSACleanup();
  return 0;
}