//TCPServer

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>

#define PORT 8080

int main(int argc, char const* argv[]){
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
	char* hello = "Hi There!";
	
	if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	
	if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    if(bind(server_fd,(struct sockaddr*)&address, sizeof(address)) < 0){
    	perror("bind failed");
    	exit(EXIT_FAILURE);
    }
    
    if(listen(server_fd, 4) < 0){
    	perror("listen failed");
    	exit(EXIT_FAILURE);
    }
    
    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    
    valread = read(new_socket,buffer,1024);
    printf("Received message: %s\n",buffer);
    send(new_socket, hello, strlen(hello),0);
    printf("Message sended successfully to Client\n");
    
    close(new_socket);
    close(server_fd);
    return 0;
 }
 
 //TCPClient
 
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

int main(int argc, char const* argv[]) {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char* hello = "Hi There!";
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n socket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

   
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\n Invalid address/Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\n connection failed \n");
        return -1;
    }

    send(sock, hello, strlen(hello), 0);
    printf("Message sended successfully to Server\n");

    valread = read(sock, buffer, 1024);
    printf("Message received from Server: %s\n", buffer);

    
    close(sock);
    return 0;
}
