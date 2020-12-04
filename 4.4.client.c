#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>  //inet_addr
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <netdb.h>
#include <stdbool.h>

int main (int argc, char *argv[])
{
 int socket_desc;
 struct sockaddr_in server;
 char *message, server_reply[2000];
 bool isExit = false;
 int bufsize = 1024;
 char buffer[bufsize];

 //Create socket
 socket_desc = socket(AF_INET, SOCK_STREAM, 0);

 if (socket_desc == -1)
 {
  printf("Could not create socket");
 }

 server.sin_addr.s_addr = inet_addr("192.168.43.200");
 server.sin_family = AF_INET;
 server.sin_port = htons(22);

 //Connect to remote server
 if (connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
 {
  puts("connect error");
  return 1;
 }

 puts("Connected\n");

 printf("\n****Press * , then Enter to let Server talk****\n");
 printf("*****Press # to stop the connection*****\n");

 recv(socket_desc, buffer, bufsize, 0);

 do {
  printf("Client: ");
   do {
   fgets (buffer, bufsize, stdin);
   send(socket_desc, buffer, bufsize, 0);
            if (*buffer == '#') {
                send(socket_desc, buffer, bufsize, 0);
                *buffer = '*';
                isExit = true;
            }
  }while (*buffer != 42);

  printf("Server: ");

  do {
   recv(socket_desc, buffer, bufsize, 0);
            printf("%s ", buffer);
            if (*buffer == '#') {
                *buffer = '*';
                isExit = true;
            }
  }while (*buffer != 42);
 }while (!isExit);

 printf("Connection End!\n\n");

 close(socket_desc);

 return 0;
}
