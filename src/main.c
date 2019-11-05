#include <stdio.h>
#include <stdbool.h>
//BEGIN EPOLL
#include <sys/epoll.h>
//END EPOLL
//BEGIN OPEN LISTENING SOCKET
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
//END OPEN LISTENING SOCKET
#include "socket.h"
#include "parser.h"

int main_loop(int const incFd);

int main(int argc, char**argv)
{
  int const parse_res = parse_argc(argc, argv);
  if(parse_res){
    return parse_res > 0 ? 0 : -1;
  }
  printf("Listening on port %s, from all IP. Max text is 99 chars\n", get_args()->from.port);
  int const listenSocket = listen_socket_gen(get_args()->from.port);
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

  int const loop = main_loop(con);
  if(loop < 0){
    fprintf(stderr, "Something is failed\n");
    return -1;
  }
  close(con);
  return 0;
}


int main_loop(int const incFd)
{
  struct epoll_event event;

  int const epollFd = epoll_create(1);
  if (epollFd < 0) {
    perror("Error creating epoll object");
    return -1;
  }

  event.events = EPOLLIN;
  event.data.fd = incFd;
  if(epoll_ctl(epollFd, EPOLL_CTL_ADD, incFd, &event)){
    perror("Error adding listening socket to epoll");
    close(epollFd);
    return -1;
  }

  //OPENING THE OTHER END
  int const targetFd = send_socket_gen("www.duckduckgo.com", "80");
  if(targetFd < 0){
    perror("Error on creating connection");
    close(epollFd);
    return -1;
  }
  printf("connected to the target\n");


  event.events = EPOLLIN;
  event.data.fd = targetFd;
  if(epoll_ctl(epollFd, EPOLL_CTL_ADD, targetFd, &event)){
    perror("Error adding target socket to epoll");
    close(epollFd);
    return -1;
  }

  while(true) {
    char buffer[100];
    if(epoll_wait(epollFd, &event, 1, -1) == 0){
      printf("Timeout exceeded. Begin cleaning up\n");
      break;
    }
    int const connection = event.data.fd;
    ssize_t const len = recv(connection, buffer, 100, 0);
    if(len < 0){
      perror("Error on Reading message");
      return -1;
    }
    if(len == 0) {
      printf("Disconnection from %d\n", connection);
      break;
    }
    buffer[len] = '\0';

    int const bypassFd = (connection == incFd) ? targetFd : incFd;
    unsigned int const color = (connection == incFd) ? 32 : 33;
    printf("\x1b[%um%s\x1b[0m", color, buffer);

    ssize_t const sended = send(bypassFd, buffer, (size_t)len, 0);
    if(sended < 0){
      perror("Error on send message");
    }
    if(sended != len) {
      fprintf(stderr, "incorrect sended data. expected %zd, sended %zd\n", len, sended);
    }
  }

  close(targetFd);
  close(epollFd);
  return 0;
}
