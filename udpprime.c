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
    int sockfd, port;
    struct sockaddr_in serveraddr, clientaddr;
    socklen_t len = sizeof(clientaddr);
    char buffer[50];
    char prime_series[512];

    printf("Enter port number: ");
    scanf("%d", &port);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    printf("UDP Server is running and waiting for a message...\n");

    recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientaddr, &len);
    int n = atoi(buffer);
    printf("Received number: %d\n", n);

    generate_primes(n, prime_series);

    sendto(sockfd, prime_series, strlen(prime_series), 0, (struct sockaddr *)&clientaddr, len);
    printf("Sent prime numbers to client.\n");

    close(sockfd);
    return 0;
}


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main() {
    int sockfd, port;
    struct sockaddr_in serveraddr;
    socklen_t len = sizeof(serveraddr);
    char buffer[50];
    char response[512];

    printf("Enter port number: ");
    scanf("%d", &port);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    printf("Enter a number: ");
    scanf("%s", buffer);

    sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&serveraddr, len);
    printf("Sent number to server.\n");

    recvfrom(sockfd, response, sizeof(response), 0, (struct sockaddr *)&serveraddr, &len);
    printf("Prime numbers: %s\n", response);

    close(sockfd);
    return 0;
}


