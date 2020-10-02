#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <ctype.h>

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

int main(int argc, char* argv[]) {

  // atividade de preenchimento da estrutura de endereço IP
  // e porta de conexão, precisa ser zerado o restante da estrutura
  // struct sockaddr_in {
  //  short            sin_family;   // e.g. AF_INET, AF_INET6
  //  unsigned short   sin_port;     // e.g. htons(3490)
  //  struct in_addr   sin_addr;     // see struct in_addr, below
  //  char             sin_zero[8];  // zero this if you want to
  // };
  int port[19];
  std::size_t aux1, aux2, aux3;
  std::string hostname[19], strAux[19], object[19], ipAddress[19];
  int j = 1;
  if (argc > 15){
	  printf("Too many arguments\n");
	  return 7;
  }
  if (argc == 1){
	  hostname[j] = "www.google.com";
	  object[j] = "/";
	  port[j] = 80;
  }
  else {
	  while (j < argc){
		  strAux[j] = argv[j];
		  aux1 = strAux[j].find("http");
		  if (aux1 == std::string::npos){
			  aux2 = strAux[j].find(":");
			  aux3 = strAux[j].find("/");
			  hostname[j] = strAux[j].substr(0, aux2);
		  }
		  else{
			  aux1 += 4;
			  aux1 = strAux[j].find("://", aux1);
			  aux1 += 3;
			  aux2 = strAux[j].find(":", aux1);
			  aux3 = strAux[j].find("/", aux1);
			  hostname[j] = strAux[j].substr(aux1, aux2 - aux1);
		  }
		  if (aux2 != std::string::npos)
			  port[j] = atoi(strAux[j].substr(aux2+1, aux3-aux2-1).c_str());
		  else
			  port[j] = 80;
		  if (aux3 != std::string::npos)
			  object[j] = strAux[j].substr(aux3, strAux[j].size() - aux3);
		  else
			  object[j] = "/";
		  j++;
	  }
  }
  j = 1;
  
  int state, i;
  bool ip, noargs = (argc == 1);
  for (int r = 1; r < argc || noargs; r++){
	  state = 0;
	  i = 0;
	  ip = false;
	  while (i < (int) hostname[r].size()){
		  if (i == 0 && isdigit(hostname[r][i]))
			  state++;
		  else if ((state == 1 || state == 3 || state == 5) && hostname[r][i] == '.'){
			  state++;
		  }
		  else if ((state == 2 || state == 4 || state == 6) && isdigit(hostname[r][i])){
			  state++;
		  }
		  else if (state == 7 && hostname[r][i] == '.'){
			  break;
		  }
		  else if (!isdigit(hostname[r][i]) && hostname[r][i] != '.'){
			  break;
		  }
		  i++;
		  if (state == 7 && i == (int) hostname[r].size()){
			  ip = true;
		  }
		  
	  }
	  std::cout << hostname[r] << " " << port[r] << " " << object[r] << std::endl;
	  if (!ip){
		  struct addrinfo hints;
		  struct addrinfo* res;
		  memset(&hints, 0, sizeof(hints));
		  hints.ai_family = AF_INET; // IPv4
		  hints.ai_socktype = SOCK_STREAM; // TCP
		  int status = 0;
		  if ((status = getaddrinfo(hostname[r].c_str(), "80", &hints, &res)) != 0) {
			  std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
			  return 8;
		  }
		  for(struct addrinfo* p = res; p != 0; p = p->ai_next) {
			  // a estrutura de dados eh generica e portanto precisa de type cast
			  struct sockaddr_in* ipv4 = (struct sockaddr_in*)p->ai_addr;

			  // e depois eh preciso realizar a conversao do endereco IP para string
			  char ipstr[INET_ADDRSTRLEN] = {'\0'};
			  inet_ntop(p->ai_family, &(ipv4->sin_addr), ipstr, sizeof(ipstr));
			  ipAddress[r] = ipstr;
		  }

		  freeaddrinfo(res); // libera a memoria alocada dinamicamente para "res"
	  }
	  else {
		  ipAddress[r] = hostname[r];
	  }
	  std::cout << "r " << r << std::endl;
	  std::cout << ipAddress[r] << std::endl;
  
	  struct sockaddr_in serverAddr;
	  serverAddr.sin_family = AF_INET;
	  serverAddr.sin_port = htons(port[r]);     // short, network byte order
	  
	  serverAddr.sin_addr.s_addr = inet_addr(ipAddress[r].c_str());
	  memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

	  
	  // cria o socket TCP IP
	  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	  // conecta com o servidor atraves do socket
	  if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
		perror("connect");
		return 2;
	  }

	  // a partir do SO, eh possivel obter o endereço IP usando 
	  // pelo cliente (nos mesmos) usando a funcao getsockname
	  struct sockaddr_in clientAddr;
	  socklen_t clientAddrLen = sizeof(clientAddr);
	  if (getsockname(sockfd, (struct sockaddr *)&clientAddr, &clientAddrLen) == -1) {
		perror("getsockname");
		return 3;
	  }

	  // em caso de multiplos endereços, isso permite o melhor controle
	  // a rotina abaixo, imprime o valor do endereço IP do cliente
	  char ipstr[INET_ADDRSTRLEN] = {'\0'};
	  inet_ntop(clientAddr.sin_family, &clientAddr.sin_addr, ipstr, sizeof(ipstr));
	  std::cout << "Set up a connection from: " << ipstr << ":" <<
		ntohs(clientAddr.sin_port) << std::endl;

	  // trecho de código para leitura e envio de dados nessa conexao
	  // buffer eh o buffer de dados a ser recebido no socket com 20 bytes
	  // input eh para a leitura do teclado
	  // ss eh para receber o valor de volta
	  char buf[5000] = {0};
	  std::string input = "";
	  std::stringstream ss;
	  ss.str("");

	  // zera o buffer
	  memset(buf, '\0', sizeof(buf));

	  ss << "GET "<< object[r] << " HTTP/1.0\r\n\r\n";
	  input = ss.str().c_str();

	  // converte a string lida em vetor de bytes 
	  // com o tamanho do vetor de caracteres
	  if (send(sockfd, input.c_str(), input.size(), 0) == -1) {
		perror("send");
		return 4;
	  }

	  // recebe no buffer uma certa quantidade de bytes ate 20 
	  int l = recv(sockfd, buf, 5000, 0);
	  if (l == -1) {
		perror("recv");
		return 5;
	  }
	  std::cout << "l " << l << std::endl;
	  char *token = NULL, *token2;
	  token = strtok(buf, "\n");
	  std::string parse_aux = token;
	  std::cout << parse_aux << std::endl;
	  bool ok = false;
	  if (parse_aux.substr(9, 3) == "200")
		  ok = true;
	  while (token) {
		  printf("Current token: %s\n", token);
		  token2 = strtok(NULL, "\n");
		  if (token2 == NULL && ok){
			  ss.str("");
			  ss << "file" << r << ".txt";
			  std::fstream f(ss.str().c_str(), std::fstream::out);
			  f << token;
			  f.close();
			  printf("len %lu\n", strlen(token));
		  }
		  token = token2;
	  }


	  // fecha o socket
	  close(sockfd);
	  noargs = false;
  }

  return 0;
}