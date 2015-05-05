#include "stdio.h"
#include "winsock2.h"

int main_cliente(){
  //CLIENTE
  WSADATA wsa;
  SOCKET sock;
  struct sockaddr_in ip;
  int broad = 1;
  

  WSAStartup(MAKEWORD(2, 0), &wsa);
  sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);//DEFINIMOS EL SOCKET

  ip.sin_family = AF_INET; //le decimos q es ipv4
  ip.sin_addr.s_addr = inet_addr("255.255.255.255");// ip broadcast
  ip.sin_port = htons(9999);//puerto por el que enviamos
  setsockopt(sock, SOL_SOCKET, SO_BROADCAST,(char *)&broad, sizeof(broad)); // definimos el socket de tipo broadcast
  sendto(sock, "connect:tok",6,0,(SOCKADDR*)&ip,sizeof(ip)); //mandamos el mensaje
  
  
  return 0;
}