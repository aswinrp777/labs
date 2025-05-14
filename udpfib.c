#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>

void fib(int n, char *result) {
    int a = 0, b = 1, next;
    char buffer[10];
    result[0] = '\0';

    for (int i = 0; i < n; i++) {
        snprintf(buffer, sizeof(buffer), "%d", a);
        strcat(result, buffer);
        if (i < n - 1)
            strcat(result, " ");
        next = a + b;
        a = b;
        b = next;
    }
}

int main() {
    int sockfd, port;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t len = sizeof(clientaddr);
    char buffer[50];
    char response[100];

    printf("Enter port number: ");
    scanf("%d", &port);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

    printf("Server is running... Waiting for client message\n");

    recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientaddr, &len);
    int n = atoi(buffer);
    printf("Received number from client: %d\n", n);

    fib(n, response);

    sendto(sockfd, response, strlen(response), 0, (struct sockaddr*)&clientaddr, len);
    printf("Sent Fibonacci series to client: %s\n", response);

    close(sockfd);
    return 0;
}


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>

int main() {
    int sockfd, port;
    struct sockaddr_in serveraddr;
    socklen_t len = sizeof(serveraddr);
    char buffer[50], response[100];

    printf("Enter port number: ");
    scanf("%d", &port);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    printf("Enter a number: ");
    scanf("%s", buffer);

    sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&serveraddr, len);
    printf("Sent number to server.\n");

    recvfrom(sockfd, response, sizeof(response), 0, (struct sockaddr*)&serveraddr, &len);
    printf("Fibonacci series received from server: %s\n", response);

    close(sockfd);
    return 0;
}

