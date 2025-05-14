#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
void fib(int n, char *result)
{
  int a = 0, b = 1;
  char buffer[10];
  result[0] = '\0';
  for (int i = 0; i < n; i++)
  {
    snprintf(buffer, sizeof(buffer), "%d", a);
    strcat(buffer, result);
    int next = a + b;
    a = b;
    b = next;
  }
}
int main()
{
  int serversocket, clientsocket, port, i;
  struct sockaddr_in serveraddr, clientaddr;
  char buffer[50];
  char fib_series[50];
  struct serveraddr;
  socklen_t len;
  serversocket = socket(AF_INET, SOCK_STREAM, 0);
  bzero((char *)&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  printf("port number:");
  scanf("%d", &port);
  serveraddr.sin_port = htons(port);
  serveraddr.sin_addr.s_addr = INADDR_ANY;
  bind(serversocket, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
  bzero((char *)&clientaddr, sizeof(clientaddr));
  len = sizeof(clientaddr);
  listen(serversocket, 5);
  printf("\n waiting for client connection");
  printf("\n hai");
  clientsocket = accept(serversocket, (struct sockaddr *)&clientaddr, &len);
  printf(" \n reading message from client");
  read(clientsocket, buffer, sizeof(buffer));
  int n = atoi(buffer);
  printf("\n recieved number is:%d", n);
  fib(n, fib_series);
  printf("\n Sending message to client");
  write(clientsocket, fib_series, strlen(fib_series));
  close(clientsocket);
  close(serversocket);
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
int main()
{
  int serversocket, clientsocket, port;
  struct sockaddr_in serveraddr, clientaddr;
  struct serveraddr;
  socklen_t len;
  char buffer[50];
  char response[50];
  clientsocket = socket(AF_INET, SOCK_STREAM, 0);
  bzero((char *)&serveraddr, sizeof(serveraddr));
  len = sizeof(serveraddr);
  serveraddr.sin_family = AF_INET;
  printf("port number:");
  scanf("%d", &port);
  serveraddr.sin_port = htons(port);
  printf("\ntrying to connect to the server");
  connect(clientsocket, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
  printf("enter a number:");
  scanf("%s", buffer);
  printf("\nConnected to server");
  printf("\n sending message to client");
  send(clientsocket, buffer, strlen(buffer), 0);
  printf("\n Message recieving");
  recv(clientsocket, response, sizeof(response), 0);
  printf("\nfib series is %s\n", response);
  close(clientsocket);
}