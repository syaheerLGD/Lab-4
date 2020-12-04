#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>  //inet_addr
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdbool.h>

int main (int argc, char *argv[])
{
 int socket_desc, new_socket, c;
 struct sockaddr_in server, client;
 char data[2000], *reply;
 bool isExit = false;
 int bufsize = 1024;
 char buffer[bufsize];

 //Create socket
 socket_desc = socket(AF_INET, SOCK_STREAM, 0);

 if (socket_desc == -1)
 {
  printf("Could not create socket");
 }

 //Prepare the sockaddr_in structure
 server.sin_family = AF_INET;
 server.sin_addr.s_addr = INADDR_ANY;
 server.sin_port = htons(8888);

 //Bind
 if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
 {
  puts("bind failed");
 }

 puts("bind done");

 //Listen
 listen(socket_desc, 3);

 //Accept an incoming connection
 puts("Waiting for incoming connections... ");
 c = sizeof(struct sockaddr_in);
 new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);

 printf("\n****Press * and then Enter to let Server talk****\n");
 printf("*****Press # to terminate the connection*****\n");

 while(new_socket > 0)
 {
  strcpy(buffer, "...connected");
  send(new_socket, buffer, bufsize, 0);

 printf("Client: ");
 do{
  recv(new_socket, buffer, bufsize, 0);
  printf("%s ", buffer);
  if (*buffer == '#') {
   *buffer = '*';
   isExit = true;
  }
 }while(*buffer != '*');

 do{
  printf("\nServer: ");
  do{
   fgets(buffer, bufsize, stdin);
   send(new_socket, buffer, bufsize, 0);
   if(*buffer == '#'){
    send(new_socket, buffer, bufsize, 0);
    *buffer = '*';
    isExit = true;
   }
  }while(*buffer != '*');
  printf("Client: ");
  do{
   recv(new_socket, buffer, bufsize, 0);
   printf("%s ", buffer);
   if (*buffer == '#') {
    *buffer == '*';
    isExit = true;
   }
  }while(*buffer != '*');
 }while(!isExit);

 close(new_socket);
 printf("Connection End\n\n");
 isExit = false;
 exit(1);
 }

 close(socket_desc);

 return 0;
}
