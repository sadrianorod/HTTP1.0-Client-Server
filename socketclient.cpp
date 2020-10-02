// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#define PORT 8080 //Select other Port if necessary


int client(const char *message, int port){

	//Initializing vars
	int sock = 0, valread; 
	struct sockaddr_in serv_addr;  
	char buffer[1024] = {0}; 

	//Try to create a socket
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	//Connection using IPV4
	serv_addr.sin_family = AF_INET;  
	//Connection using port argument
	serv_addr.sin_port = htons(port); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return -1; 
	} 

	send(sock , message , strlen(message) , 0 ); 
	printf("Message was sent\n"); 
	valread = read( sock , buffer, 1024); 
	printf("%s\n",buffer ); 

	return 1;
}

int main(int argc, char const *argv[]) 
{	
	int ans = 0;
	ans = client("Hello World!",PORT);
	if(ans != 1){
		perror("Error\n");
	}
	return 0; 
} 
