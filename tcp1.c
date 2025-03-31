//TCPServer Side

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>

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
    socklen_t client_len = sizeof(client);

    printf("Server waiting for messages...\n");
    while (1) {       
        if (recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client, &client_len) < 0) {
            printf("Error in recvfrom()\n");
            return 1;
        }        
        printf("Client says: %s\n", buffer);        
        if (strcmp(buffer, "exit") == 0) {
            printf("Server is shutting down.\n");
            break;
        }        
        printf("Enter reply: ");
        fgets(buffer, sizeof(buffer), stdin);
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        if (sendto(sockfd, buffer, strlen(buffer) + 1, 0, (struct sockaddr*)&client, client_len) < 0) {
            printf("Error in sendto()\n");
            return 1;
        }
    }   
    return 0;
}


//TCPClient side

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    struct sockaddr_in server;
    if (argc != 3) {
        printf("Usage: %s <server_address> <port>\n", argv[0]);
        return 1;
    }
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
    while (1) {
        printf("Enter message to send to server: ");
        fgets(buffer, sizeof(buffer), stdin);
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        if (sendto(sockfd, buffer, strlen(buffer) + 1, 0, (struct sockaddr*)&server, sizeof(server)) < 0) {
            printf("Error in sendto()\n");
            return 1;
        }

        if (strcmp(buffer, "exit") == 0) {
            printf("Exiting chat...\n");
            break;
        }
        if (recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL) < 0) {
            printf("Error in recvfrom()\n");
            return 1;
        }       
        printf("Server says: %s\n", buffer);
    }
    return 0;
}

