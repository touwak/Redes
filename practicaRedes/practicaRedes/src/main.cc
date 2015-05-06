#include "stdio.h"
#include "winsock2.h"


int main(int argc, char** argv){
  int i;
  WSADATA wsa;
  SOCKET sock;
  SOCKET sock_cte;
  struct sockaddr_in ip, ip_c;

  char msg_c[1024];

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
  

  if (bind(sock, (SOCKADDR*)&ip, sizeof(ip)) == SOCKET_ERROR){
    printf("\nERROR en bind: %ld\n", WSAGetLastError());
    return 1;
  }
  //apertura de puerto
  if (listen(sock, SOMAXCONN)){
    printf("ERROR en apertura de puerto");
    return 1;
  }

  sock_cte = accept(sock, (SOCKADDR *)&ip_c, NULL);
  printf("cliente conectado");
  printf("IP cliente: %s", inet_ntoa(ip_c.sin_addr));
  memset(msg_c, 0, 1024);
  strcpy(msg_c, "");//TODO ENVIAR LA PPAGINA
  send(sock_cte, msg_c, sizeof(msg_c),0);
  
  system("pause");
  closesocket(sock);
  WSACleanup();
  return 0;
}