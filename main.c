#include <stdio.h>
//BEGIN OPEN SOCKET
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
//END OPEN SOCKET
int listen_socket_gen(char const * port);


int main(int argc, char**argv)
{
  char buffer[100];
  for(int i = 0; i < argc; i++) {
    printf("%d, %s\n", i, argv[i]);
  }
  int const listenSocket = listen_socket_gen("8000");
  if(listenSocket < 0){
    perror("Error on socket generation\n");
    return -1;
  }
  int const con = accept(listenSocket, NULL, NULL);
  close(listenSocket);
  if(con < 0){
    perror("Error on accepting connection");
  }
  ssize_t const len = recv(con, buffer, 100, 0);
  if(con < 0){
    perror("Error on Reading message");
  }
  buffer[len] = '\0';
  printf("Message is %s\n", buffer);
  ssize_t const sended = send(con, buffer, (size_t)len, 0);
  if(sended < 0){
    perror("Error on send echo");
  }
  if(sended != len) {
    fprintf(stderr, "incorrect sended data. expected %zd, sended %zd\n", len, sended);
  }
  close(con);
  return 0;
}


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
