//server

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void generate_primes(int n, char *result) {
    int count = 0, num = 2;
    char buffer[10];
    result[0] = '\0';

    while (count < n) {
        int is_prime = 1;
        for (int i = 2; i * i <= num; i++) {
            if (num % i == 0) {
                is_prime = 0;
                break;
            }
        }

        if (is_prime) {
            snprintf(buffer, sizeof(buffer), "%d ", num);
            strcat(result, buffer);
            count++;
        }
        num++;
    }
}

int main() {
    int serversocket, clientsocket, port;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t len;
    char buffer[50];
    char prime_series[512];

    serversocket = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;

    printf("Enter port number: ");
    scanf("%d", &port);

    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    bind(serversocket, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    listen(serversocket, 5);

    printf("Waiting for client connection...\n");
    len = sizeof(clientaddr);
    clientsocket = accept(serversocket, (struct sockaddr *)&clientaddr, &len);

    read(clientsocket, buffer, sizeof(buffer));
    int n = atoi(buffer);
    printf("Received number: %d\n", n);

    generate_primes(n, prime_series);
    write(clientsocket, prime_series, strlen(prime_series));

    close(clientsocket);
    close(serversocket);
    return 0;
}



//client

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main() {
    int clientsocket, port;
    struct sockaddr_in serveraddr;
    socklen_t len;
    char buffer[50];
    char response[512];

    clientsocket = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&serveraddr, sizeof(serveraddr));

    printf("Enter port number: ");
    scanf("%d", &port);

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = INADDR_ANY; 

    printf("Connecting to server...\n");
    connect(clientsocket, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

    printf("Enter a number: ");
    scanf("%s", buffer);

    send(clientsocket, buffer, strlen(buffer), 0);
    recv(clientsocket, response, sizeof(response), 0);

    printf("Prime numbers: %s\n", response);
    close(clientsocket);

    return 0;
}


