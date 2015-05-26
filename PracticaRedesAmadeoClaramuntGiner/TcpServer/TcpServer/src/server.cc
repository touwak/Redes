#include "server.h"

//read files
char* Server::readFile(const char* file_path, int * out_put_size){
  std::ifstream stream(file_path, std::ios::binary | std::ios::in);

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

char* Server::readImage(const char* file_path, int * out_put_size){
  FILE * pFile;
  long lSize;
  char * buffer;
  size_t result;

  pFile = fopen(file_path, "rb");
  if (pFile == NULL) { fputs("File error", stderr); exit(1); }

  // obtain file size:
  fseek(pFile, 0, SEEK_END);
  lSize = ftell(pFile);
  *out_put_size = lSize;
  rewind(pFile);

  // allocate memory to contain the whole file:
  buffer = (char*)malloc(sizeof(char)*lSize);
  if (buffer == NULL) { 
    fputs("Memory error", stderr); exit(2); 
  }

  // copy the file into the buffer:
  result = fread(buffer, 1, lSize, pFile);
  if (result != lSize) { 
    fputs("Reading error", stderr); exit(3); 
  }

  fclose(pFile);
  return buffer;
}

//server
int Server::init(int port){
  WSADATA wsa;
  SOCKET sock;
  SOCKET sock_cte;
  int code_size = 0;
  struct sockaddr_in ip, ip_c;
  char* html = NULL, server_reply[10000];
  char *res;
  char *web_res;
  std::string aux_res, index_path, error_path;
  char *image_ext;
  char *web_ext;
  int html_size = 10000;
  int recv_size;
  bool charged = false;
  char msg_c[1024];

  char *host;
  char *server_reply2;

  
 

  if (WSAStartup(MAKEWORD(2, 0), &wsa)){
    printf("Error!");
    return 1;
  }

  sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (sock == INVALID_SOCKET){
    printf("ERROR in socket(): %ld\n", WSAGetLastError());
    WSACleanup();
    return 1;
  }

  ip.sin_family = AF_INET;
  ip.sin_addr.s_addr = inet_addr("127.0.0.1");
  ip.sin_port = htons(port);


  if (bind(sock, (SOCKADDR*)&ip, sizeof(ip)) == SOCKET_ERROR){
    printf("\nERROR in bind: %ld\n", WSAGetLastError());
    return 1;
  }
  //opening port
  if (listen(sock, SOMAXCONN)){
    printf("ERROR opening the port");
    return 1;
  }

  printf("\nserver started\n");


  while (true)
  {
    sock_cte = accept(sock, (SOCKADDR *)&ip_c, NULL);
    printf("\ncliente conected");
    printf("\nIP client: %s\n", inet_ntoa(ip_c.sin_addr));

    if ((recv_size = recv(sock_cte, server_reply, 2000, 0)) != SOCKET_ERROR){
      server_reply[recv_size] = '\0';

      server_reply2 = new char[recv_size];
      memset(server_reply2, '\0', recv_size);
      strcpy(server_reply2, server_reply);


      printf("\n");
      puts(server_reply);
      
      if (recv_size > 1){
        res = strtok(server_reply, " ");
        res = strtok(NULL, " ");
        
        web_res = new char[strlen(res)];
        memset(web_res, '\0', strlen(res));
        strcpy(web_res, res);


        //requested host
        host = strtok(server_reply2, "\n");
        host = strtok(NULL, " ");
        host = strtok(NULL, ":");
        
        //change the directory to the vhost or normal
        if (strcmp(host, "www.dominio1.com") == 0){
          aux_res = "vhost/dominio1.com";
          index_path = "vhost/dominio1.com/index.htm";
          error_path = "vhost/dominio1.com/error.htm";
        }
        else if (strcmp(host, "www.dominio2.com") == 0){
          aux_res = "vhost/dominio2.com";
          index_path = "vhost/dominio2.com/index.htm";
          error_path = "vhost/dominio2.com/error2.htm";
        }
        else{
          aux_res = "www";
          index_path = "www/index.htm";
          error_path = "www/error.htm";
        }
        aux_res += res;

        //css ext
        web_ext = strtok(web_res, "/");
        web_ext = strtok(web_res, ".");
        web_ext = strtok(NULL, " ");

        //image ext
        image_ext = strtok(res, "/");
        image_ext = strtok(NULL, "/");
        image_ext = strtok(image_ext, ".");
        image_ext = strtok(NULL, " ");

        if (image_ext == NULL){
          image_ext = "0";
        }
        if (res == NULL){
          res = "0";
        }
        if (web_ext == NULL){
          web_ext = "0";
        }
      }


      // IMAGES //////////////
      // send the images of the web
      if (strcmp(image_ext, "png") == 0 || strcmp(image_ext, "jpg") == 0 || strcmp(image_ext, "gif") == 0){

          html = readImage(aux_res.c_str(), &code_size);
          
          std::string header = "HTTP/1.1 200 OK\n";
          header += "content-type: image/*\n";
          header += "accept-ranges: bytes\n";
          header += "content-length: " + std::to_string(code_size) + "\n";
          header += "Server: Apache 2.0.23\n";
          header += "\n";

          memset(msg_c, 0, 1024);
          strcpy(msg_c, header.c_str());
          send(sock_cte, msg_c, strlen(msg_c), 0);

          
          send(sock_cte, html, code_size, 0);
          free(html);

      }/// WEB /////////////
      // send the css file
      else if (strcmp(web_ext, "css") == 0 ){
        //CSS
        std::string header = "HTTP/1.1 200 OK\n";
        header += "content-type: text/css\n";
        header += "accept-ranges: bytes\n";
        header += "content - length: " + std::to_string(code_size) + "\n";
        //header += "Server: Apache 2.0.23\r\n";
        header += "\n";

        html = readFile(aux_res.c_str(), &code_size);
        memset(msg_c, 0, 1024);
        strcpy(msg_c, header.c_str());
        send(sock_cte, msg_c, strlen(msg_c), 0);

        send(sock_cte, html, strlen(html), 0);
        free(html);
      }// send the index of the web
      else if (strcmp(res,"/") == 0){

        if (readFile(index_path.c_str(), &code_size) != NULL){

          std::string header = "HTTP/1.1 200 OK\n";
          header += "content - type: text/html\n";
          header += "accept - ranges: bytes\n";
          header += "content - length: " + std::to_string(code_size) + "\n";
          header += "Server: Apache 2.0.23\n";
          header += "\n";

          memset(msg_c, 0, 1024);
          strcpy(msg_c, header.c_str());
          send(sock_cte, msg_c, strlen(msg_c), 0);

          //HTML
          html = readFile(index_path.c_str(), &code_size);
          send(sock_cte, html, strlen(html), 0);
          free(html);
        }
      }
      else{//send a error web if the requested web don't exist
        if (readFile(error_path.c_str(), &code_size) != NULL){
          std::string header = "HTTP/1.1 404 pagina no encontrada\n";
          header += "Content-Type: (text / html | image/ *| text/css)\n";
          header += "accept-ranges: bytes\n";
          header += "content - length: " + std::to_string(code_size) + "\n";
          header += "Server: Apache 2.0.23\n";
          header += "\n";

          memset(msg_c, 0, 1024);
          strcpy(msg_c, header.c_str());
          send(sock_cte, msg_c, strlen(msg_c), 0);

          html = readFile(error_path.c_str(), &code_size);
          send(sock_cte, html, strlen(html), 0);
          free(html);

        }
      }
    }


    
    closesocket(sock_cte);
  }
  closesocket(sock);
  WSACleanup();
 
}
