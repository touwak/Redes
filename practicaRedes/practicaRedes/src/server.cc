#include "server.h"


char* Server::readFile(const char* file_path, int * out_put_size){
  std::ifstream stream(file_path, std::ios::binary);

  if (stream.is_open()) {
    std::stringstream of_buffer;
    std::string buffer;
    std::string code;
    of_buffer << stream.rdbuf();
    code = of_buffer.str();

    *out_put_size = code.size();
    char* file_code = (char*)malloc(code.size() + 1);
    memset(file_code, 0, code.size() + 1);
    strncpy((char*)file_code, code.c_str(), code.size());

    return file_code;
  }

  return NULL;
}

int Server::init(int port = 8080, const char* file_path = "www/index.htm"){
  WSADATA wsa;
  SOCKET sock;
  SOCKET sock_cte;
  int code_size = 0;
  struct sockaddr_in ip, ip_c;
  char* html = NULL, server_reply[10000], res[5];
  int html_size = 10000;
  int recv_size;
  bool charged = false;

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

 


  while (true)
  {
    sock_cte = accept(sock, (SOCKADDR *)&ip_c, NULL);
    printf("\ncliente conectado");
    printf("\nIP cliente: %s\n", inet_ntoa(ip_c.sin_addr));

    if ((recv_size = recv(sock_cte, server_reply, 2000, 0)) != SOCKET_ERROR){
      server_reply[recv_size] = '\0';


      printf("\n");
      puts(server_reply);

      for (int i = 4; i < 8; ++i)
      {
        res[i - 4] = server_reply[i];
      }
      res[4] = '\0';

      //PNG
      if (strcmp(res, "/png") == 0){
        if (readFile("www/imagenes/batman.png", &code_size) != NULL){
          std::string header = "HTTP/1.1 200 OK\r\n";
          header += "Content - Type: image/* \r\n";
          header += "Content - Length: " + std::to_string(code_size) + "\r\n";
          header += "Server: Apache 2.0.23\r\n";
          header += "\r\n";

          memset(msg_c, 0, 1024);
          strcpy(msg_c, header.c_str());
          send(sock_cte, msg_c, strlen(msg_c), 0);

          html = readFile("www/imagenes/batman.png",&code_size);
          send(sock_cte, html, strlen(html), 0);
          free(html);
        }
      }//JPG
      else if (strcmp(res, "/jpg") == 0 || strcmp(res, "/fav") == 0){
        if (readFile("www/imagenes/2.jpg", &code_size) != NULL){

          html = readFile("www/imagenes/2.jpg", &code_size);

          std::string header = "HTTP/1.1 200 OK\r\n";
          header += "Content - Type: image/* \r\n";
          header += "Content - Length: " + std::to_string(code_size) + "\r\n";
          header += "Server: Apache 2.0.23\r\n";
          header += "\r\n";

          for (int i = 0; i < code_size; ++i){
            header += html[i];
          }

          send(sock_cte, header.c_str(), code_size + header.size(), 0);
          
          //send(sock_cte, html, strlen(html), 0);
          free(html);
        }
      }//CSS
      else if (strcmp(res, "/css") == 0){
        if (readFile("www/default.css", &code_size) != NULL){
          html = readFile("www/default.css", &code_size);

          std::string header = "HTTP/1.1 200 OK\r\n";
          header += "Content - Length: " + std::to_string(code_size) + "\r\n";
          header += "Content - Type: (text / html | image/ *| text/css) \r\n";
          header += "Server: Apache 2.0.23\r\n";
          header += "\r\n";

          memset(msg_c, 0, 1024);
          strcpy(msg_c, header.c_str());
          send(sock_cte, msg_c, strlen(msg_c), 0);

          
          send(sock_cte, html, strlen(html), 0);
          free(html);
        }
      }//favico
      /*
      else if (strcmp(res, "/fav") == 0){
        if (readFile("www/imagenes/favico.ico", &code_size) != NULL){
          html = readFile("www/imagenes/favico.ico", &code_size);

          std::string header = "HTTP/1.1 200 OK\r\n";
          header += "Content - Type: image/* \r\n";
          header += "Content - Length: " + std::to_string(code_size) + "\r\n";
          header += "Server: Apache 2.0.23\r\n";
          header += "\r\n";

          memset(msg_c, 0, 1024);
          strcpy(msg_c, header.c_str());
          send(sock_cte, msg_c, strlen(msg_c), 0);

          
          send(sock_cte, html, strlen(html), 0);
          free(html);
        }
      }
      */
      else if (!charged){//HTML
        if (readFile("www/index.htm", &code_size) != NULL){
          html = readFile(file_path, &code_size);

          std::string header = "HTTP/1.1 200 OK\r\n";
          header += "Content - Type: (text / html | image/ *| text/css) \r\n";
          header += "Content - Length: " + std::to_string(code_size) + "\r\n";
          header += "Server: Apache 2.0.23\r\n";
          header += "\r\n";

          memset(msg_c, 0, 1024);
          strcpy(msg_c, header.c_str());
          send(sock_cte, msg_c, strlen(msg_c), 0);

          
          send(sock_cte, html, strlen(html), 0);
          free(html);
          charged = true;
        }
        else{
          std::string header = "HTTP/1.1 404 pagina no encontrada\r\n";
          header += "Content - Type: (text / html | image/ *| text/css) \r\n";
          header += "Content - Length: 24\r\n";
          header += "Server: Apache 2.0.23\r\n";
          header += "\r\n";

          memset(msg_c, 0, 1024);
          strcpy(msg_c, header.c_str());
          send(sock_cte, msg_c, strlen(msg_c), 0);
        }
      }
    }


    //system("pause");
    closesocket(sock_cte);
  }
  closesocket(sock);
  WSACleanup();
 
}
