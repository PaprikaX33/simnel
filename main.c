#include <stdio.h>
//BEGIN OPEN LISTENING SOCKET
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
//END OPEN LISTENING SOCKET
#include "socket.h"

int main(int argc, char**argv)
{
  char buffer[100];
  for(int i = 0; i < argc; i++) {
    printf("%d, %s\n", i, argv[i]);
  }
  printf("Listening on port 9000, from all IP. Max text is 99 chars\n");
  int const listenSocket = listen_socket_gen("9000");
  if(listenSocket < 0){
    perror("Error on socket generation\n");
    return -1;
  }
  int const con = accept(listenSocket, NULL, NULL);
  close(listenSocket);
  if(con < 0){
    perror("Error on accepting connection");
    return -1;
  }
  ssize_t const len = recv(con, buffer, 100, 0);
  if(con < 0){
    perror("Error on Reading message");
    return -1;
  }
  buffer[len] = '\0';
  printf("Message is\n%s\n", buffer);
  ssize_t const sended = send(con, buffer, (size_t)len, 0);
  if(sended < 0){
    perror("Error on send echo");
  }
  if(sended != len) {
    fprintf(stderr, "incorrect sended data. expected %zd, sended %zd\n", len, sended);
  }
  close(con);
  //OPENING THE OTHER END
  int const target = send_socket_gen("localhost", "8000");
  if(target < 0){
    perror("Error on accepting connection");
    return -1;
  }
  printf("connected to the target\n");
  ssize_t const sended_target = send(target, buffer, (size_t)len, 0);
  if(sended_target < 0){
    perror("Error on send echo");
  }
  if(sended_target != len) {
    fprintf(stderr, "incorrect sended data. expected %zd, sended %zd\n", len, sended_target);
  }
  close(target);
  return 0;
}
