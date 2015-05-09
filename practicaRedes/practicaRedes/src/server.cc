#include "server.h"


char* Server::readFile(const char* file_path){
  std::ifstream stream(file_path, std::fstream::in);

  if (stream.is_open()) {
    std::string buffer;
    std::string code;
    while (!stream.eof()) {
      std::getline(stream, buffer);

      buffer += "\n";
      code += buffer;
    }
    code += "\0";

    char* file_code = (char*)malloc(code.length() + 1);
    memset(file_code, 0, code.length() + 1);
    strncpy((char*)file_code, code.c_str(), code.length());

    printf("\n%d %d\n", code.length(), strlen(file_code));

    return file_code;
  }

  return NULL;
}

int Server::init(int port = 8080, const char* file_path = "assets/index.htm"){
  WSADATA wsa;
  SOCKET sock;
  SOCKET sock_cte;
  struct sockaddr_in ip, ip_c;
  char* html = NULL;

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
  ip.sin_port = htons(port);


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

  if (readFile(file_path) != NULL){
    std::string header = "HTTP/1.1 200 OK\r\n";
    header += "Content - Length: 24\r\n";
    header += "Content - Type: (text / html | image/ *| text/css) \r\n";
    header += "Server: Apache 2.0.23\r\n";
    header += "\r\n";

    memset(msg_c, 0, 1024);
    strcpy(msg_c, header.c_str());
    send(sock_cte, msg_c, strlen(msg_c), 0);

    html = readFile(file_path);
    send(sock_cte, html, strlen(html), 0);
    free(html);
  }
  else{
    std::string header = "HTTP/1.1 404 pagina no encontrada\r\n";
    header += "Content - Length: 24\r\n";
    header += "Content - Type: (text / html | image/ *| text/css) \r\n";
    header += "Server: Apache 2.0.23\r\n";
    header += "\r\n";

    memset(msg_c, 0, 1024);
    strcpy(msg_c, header.c_str());
    send(sock_cte, msg_c, strlen(msg_c), 0);
  }

  system("pause");
  closesocket(sock);
  WSACleanup();
}
