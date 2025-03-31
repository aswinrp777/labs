//SMTPServer

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main() {
    int server_socket, client_socket, port;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;
    char buffer[BUFFER_SIZE];

   
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

  
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;

    printf("Enter the port number: ");
    scanf("%d", &port);
    server_addr.sin_port = htons(port);

 
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    
    if (listen(server_socket, 5) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("SMTP Server is running on port %d and waiting for connections...\n", port);

    addr_len = sizeof(client_addr);
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);
    if (client_socket < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Client connected.\n");
    send(client_socket, "220 Simple SMTP Server\n", 24, 0);

    
    for (int i = 0; i < 3; i++) {
        memset(buffer, 0, BUFFER_SIZE);
        recv(client_socket, buffer, BUFFER_SIZE, 0);
        printf("Received: %s\n", buffer);
        send(client_socket, "250 OK\n", 7, 0);
    }

    printf("Email received successfully!\n");
    
    printf("\nSERVER SHUTTING DOWN!!!!\n");
    send(client_socket, "221 Bye\n", 8, 0);
    

    close(client_socket);
    close(server_socket);
    return 0;
}

//SMTPClient

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUFFER_SIZE 1024

int main() {
    int client_socket, port;
    struct sockaddr_in server_addr;
    struct hostent *server;
    char from[50], to[50], mail[BUFFER_SIZE], request[BUFFER_SIZE], response[BUFFER_SIZE];

   
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;

    printf("Enter the port number: ");
    scanf("%d", &port);
    server_addr.sin_port = htons(port);

    printf("Enter the server address: ");
    char server_name[100];
    scanf("%s", server_name);
    server = gethostbyname(server_name);
    if (!server) {
        fprintf(stderr, "Error: No such host\n");
        exit(EXIT_FAILURE);
    }
    memcpy(&server_addr.sin_addr, server->h_addr, server->h_length);

    
    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    recv(client_socket, response, BUFFER_SIZE, 0);
    printf("Server: %s", response);

    
    printf("Enter your email: ");
    scanf("%s", from);

    
    printf("Enter recipient email: ");
    scanf("%s", to);

    
    printf("Enter your message: ");
    getchar(); // Clear newline
    fgets(mail, BUFFER_SIZE, stdin);
    mail[strcspn(mail, "\n")] = '\0'; 

    
    snprintf(request, sizeof(request), "MAIL FROM: %s\n", from);
    send(client_socket, request, strlen(request), 0);
    recv(client_socket, response, BUFFER_SIZE, 0);
    printf("Server: %s", response);

    snprintf(request, sizeof(request), "RCPT TO: %s\n", to);
    send(client_socket, request, strlen(request), 0);
    recv(client_socket, response, BUFFER_SIZE, 0);
    printf("Server: %s", response);

    snprintf(request, sizeof(request), "DATA\n%s\n.\n", mail);
    send(client_socket, request, strlen(request), 0);
    recv(client_socket, response, BUFFER_SIZE, 0);
    printf("Server: %s", response);

    snprintf(request, sizeof(request), "QUIT\n");
    send(client_socket, request, strlen(request), 0);
    recv(client_socket, response, BUFFER_SIZE, 0);
    printf("Server: %s", response);

    printf("Email sent successfully!\n");
    
    printf("\nCLIENT CLOSING DOWN!!!!\n");

    close(client_socket);
    return 0;
}


