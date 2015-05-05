#include "stdio.h"
#include "winsock2.h"



/*
int main(){
  WSAData wsa;

  struct sockaddr_in ip;

  if (WSAStartup(MAKEWORD(2, 0), &wsa)){
    printf("\n Errpr en carga WSAStartup: %s", WSAGetLastError());
    return 1;
  }
  printf("\n Todo correcto!! \n");

  char nombre[50];
  gethostname(nombre, sizeof(nombre));
  printf("\n el nombre del equipo es: %S\n", nombre);
  
  SOCKET sock;
  sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock == INVALID_SOCKET){
    printf("Error en socket(): %ld\n", WSAGetLastError());
    WSACleanup();
   // return 1;
  }

  ip.sin_family = AF_INET;
  ip.sin_addr.s_addr = inet_addr("0.0.0.0");
  ip.sin_port = htons(27000);
  printf("IP y puerto asignado %s : %d", inet_ntoa(ip.sin_addr), ntohs(ip.sin_port));

  if (bind(sock, (SOCKADDR*)&ip, sizeof(ip) == SOCKET_ERROR)){
    printf("\nError en bind: %ld", WSAGetLastError());
    //return 1;
  }

  if (listen(sock, SOMAXCONN)){
    printf("Error en apertura de puerto");
    //return 1;
  }

  

  system("pause");
  closesocket(sock);
  WSACleanup();
  
  getchar();
  return 0;
}*/

#include "stdio.h"
#include "winsock2.h"
int main(int argc, char ** argv)
{
  WSAData wsa;
  SOCKET sock;
  SOCKET sock_cte;
  struct sockaddr_in ip, ipc;
  if (WSAStartup(MAKEWORD(2, 0), &wsa))
  {
    printf("Error");
    return 1;
  }
  //Definimos IP
  ip.sin_family = AF_INET;
  ip.sin_addr.s_addr = inet_addr("0.0.0.0");
  ip.sin_port = htons(7070);
  //Defino Socket
  sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (!sock)
  {
    printf("Error al crear el socket");
    return 1;
  }
  //Ejecuto el BIND
  if (bind(sock, (SOCKADDR *)&ip, sizeof(ip)))
  {
    printf("Error en BIND");
    return 1;
  }
  //Abro el puerto!!
  if (listen(sock, SOMAXCONN))
  {
    printf("Error Apertura puerto");
    return 1;
  }
  sock_cte = accept(sock, (SOCKADDR *)&ipc, NULL);
  printf("Cliente conectado con éxito\n");
  printf("IP cliente:%s", inet_ntoa(ipc.sin_addr));
  printf("\n Puerto cliente:%d", ntohs(ipc.sin_port));
  system("pause");
  closesocket(sock);
  WSACleanup();
}