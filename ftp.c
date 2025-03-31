//ftpserver

#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
int main() {
FILE *fp;
int sd, newsd, n, cli, bd, port, clilen;
char fileread[100], rcv[100];
struct sockaddr_in servaddr, cliaddr;
printf("Enter the port address: ");
scanf("%d", &port);
sd = socket(AF_INET, SOCK_STREAM, 0);
if (sd < 0) {
perror("Socket creation failed");
exit(1);
}
printf("Socket is created\n");
servaddr.sin_family = AF_INET;
servaddr.sin_addr.s_addr = INADDR_ANY;
servaddr.sin_port = htons(port);
bd = bind(sd, (struct sockaddr *)&servaddr,
sizeof(servaddr));
if (bd < 0) {
perror("Binding failed");
close(sd);
exit(1);
}
printf("Binded\n");
listen(sd, 5);
clilen = sizeof(cliaddr);
newsd = accept(sd, (struct sockaddr
*)&cliaddr, (socklen_t *)&clilen);
if (newsd < 0) {
perror("Accept failed");
close(sd);
exit(1);
}
printf("Accepted connection\n");
n = recv(newsd, rcv, sizeof(rcv) - 1, 0);
if (n <= 0) {
perror("Receive failed");
close(newsd);
close(sd);
exit(1);
}
rcv[n] = '\0';
printf("Client requested file: %s\n", rcv);
fp = fopen(rcv, "r");
if (fp == NULL) {
printf("File not found\n");
send(newsd, "error", 5, 0);
} else {
while (fgets(fileread, sizeof(fileread), fp)) {
if (send(newsd, fileread, strlen(fileread),
0) < 0) {
perror("Send failed");
break;
}
sleep(1);
}
fclose(fp);
send(newsd, "completed", 9, 0);
}
close(newsd);
close(sd);
return 0;
}


//ftpclient

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
int main() {
FILE *fp;
int csd, n, s, cport;
char name[100], rcvg[100], fname[100];
struct sockaddr_in servaddr;
printf("Enter the port: ");
scanf("%d", &cport);
csd = socket(AF_INET, SOCK_STREAM,
0);
if (csd < 0) {
perror("Socket creation failed");
exit(1);
}
printf("Socket is created\n");
servaddr.sin_family = AF_INET;
servaddr.sin_addr.s_addr =
inet_addr("127.0.0.1");
servaddr.sin_port = htons(cport);
if (connect(csd, (struct sockaddr *)&servaddr,
sizeof(servaddr)) < 0) {
perror("Connection failed");
close(csd);
exit(1);
}
printf("Connected\n");
printf("Enter the existing file name: ");
scanf("%s", name);
printf("Enter the new file name: ");
scanf("%s", fname);
fp = fopen(fname, "w");
if (fp == NULL) {
perror("Error opening new file");
close(csd);
exit(1);
}
send(csd, name, strlen(name), 0);
while (1) {
s = recv(csd, rcvg, sizeof(rcvg) - 1, 0);
if (s <= 0) {
perror("Receive error");
break;
}
rcvg[s] = '\0';
if (strcmp(rcvg, "error") == 0) {
printf("File is not available\n");
break;
}
if (strcmp(rcvg, "completed") == 0) {
printf("File is transferred.\n");
break;
}
fputs(rcvg, stdout);
fprintf(fp, "%s", rcvg);
}
fclose(fp);
close(csd);
return 0;
}

