#include <stdio.h>
//BEGIN OPEN LISTENING SOCKET
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
//END OPEN LISTENING SOCKET
#include "socket.h"

int listen_socket_gen(char const * port)
{
  int status;
  int yes = 1;
  int socketfd = 0;
  struct addrinfo hints;
  struct addrinfo *res = NULL;
  memset(&hints, 0, sizeof hints); // make sure the struct is empty
  hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
  hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

  if((status = getaddrinfo(NULL, port, &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s.", gai_strerror(status));
    return -1;
  }

  if((socketfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1){
    fprintf(stderr, "socket error: %s.", strerror(errno));
    goto SOCKERROR;
  }
  if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
    fprintf(stderr, "setsockopt error: %s.", strerror(errno));
    goto ELSEERROR;
  }
  if(bind(socketfd, res->ai_addr, res->ai_addrlen) == -1){
    fprintf(stderr, "socket bind error: %s.", strerror(errno));
    goto ELSEERROR;
  }
  freeaddrinfo(res);
  if(listen(socketfd, 10) == -1){
    fprintf(stderr, "socket listen error: %s.", strerror(errno));
    goto ELSEERROR;
  }
  return socketfd;
 ELSEERROR:
  close(socketfd);
 SOCKERROR:
  freeaddrinfo(res);
  return -1;
}

int send_socket_gen(char const * addr, char const * port)
{
  struct addrinfo hints;
  struct addrinfo *res = NULL;
  struct addrinfo * loop = NULL;
  int status;
  int sock = -1;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if((status = getaddrinfo(addr, port, &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s.\n", gai_strerror(status));
    return -1;
  }
  for(loop = res; loop != NULL; loop = loop->ai_next) {
    if ((sock = socket(loop->ai_family, loop->ai_socktype,
                         loop->ai_protocol)) == -1) {
      perror("client: socket");
      continue;
    }
    if (connect(sock, loop->ai_addr, loop->ai_addrlen) == -1) {
      close(sock);
      perror("client: connect"); //THIS THING MAY REPORT MULTIPLE TIMES
      continue;
    }
    break;
  }
  if (loop == NULL) {
    fprintf(stderr, "client: failed to connect\n");
    freeaddrinfo(res);
    return -1;
  }
  freeaddrinfo(res);
  return sock;
}
