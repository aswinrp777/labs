//UDPServer side

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netdb.h>
int main(int argc, char *argv[]) {
struct sockaddr_in server, client;
if (argc != 2) {
printf("Usage: %s <port>\n", argv[0]);
return 1;
}
int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
if (sockfd == -1) {
printf("Error in socket()\n");
return 1;
}
server.sin_family = AF_INET;
server.sin_addr.s_addr = INADDR_ANY;
server.sin_port = htons(atoi(argv[1]));
if (bind(sockfd, (struct sockaddr*)&server, sizeof(server)) < 0) {
printf("Error in bind()\n");
return 1;
}
char buffer[100];
socklen_t server_len = sizeof(server);
printf("Server waiting...\n");
if (recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&server, &server_len) < 0) {
printf("Error in recvfrom()\n");
return 1;
}
printf("Got a datagram: %s\n", buffer);
return 0;
}


//UDPClient side

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netdb.h>
int main(int argc, char *argv[]) {
struct sockaddr_in server;
if (argc != 3) {
printf("Usage: %s <server_address> <port>\n", argv[0]);
return 1;
int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
if (sockfd == -1) {
printf("Error in socket()\n");
return 1;
}
server.sin_family = AF_INET;
server.sin_port = htons(atoi(argv[2]));
if (inet_pton(AF_INET, argv[1], &server.sin_addr) <= 0) {
printf("Invalid address or address not supported\n");
return 1;
}
char buffer[100];
printf("Enter a message to be sent to the server: ");
fgets(buffer, sizeof(buffer), stdin);
size_t len = strlen(buffer);
if (len > 0 && buffer[len - 1] == '\n') {
buffer[len - 1] = '\0';
}
if (sendto(sockfd, buffer, strlen(buffer) + 1, 0, (struct sockaddr*)&server, sizeof(server)) < 0) {
printf("Error in sendto()\n");
return 1;
}
return 0;
}




