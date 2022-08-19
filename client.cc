/*
 * File client.c
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "connection.hh"

int main(int argc, char *argv[])
{
  struct sockaddr_un addr;
  int i;
  int ret;
  int data_socket;

  /* Create local socket. */

  data_socket = socket(AF_UNIX, SOCK_STREAM, 0);
  if (data_socket == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }
  /*
   * For portability clear the whole structure, since some
   * implementations have additional (nonstandard) fields in
   * the structure.
   */

  memset(&addr, 0, sizeof(struct sockaddr_un));

  /* Connect socket to socket address */

  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

  ret = connect (data_socket, (const struct sockaddr *) &addr,
      sizeof(struct sockaddr_un));
  if (ret == -1) {
    fprintf(stderr, "The server is down.\n");
    exit(EXIT_FAILURE);
  }


  if(fork()){
    dup2(data_socket,0);
    close(data_socket);
  } else {
    dup2(data_socket,1);
    close(data_socket);
  };
  
  char buffer[2];
  for(;;){
    while(read(0,buffer,1))
      write(1,buffer,1);
  }
  /* Unlink the socket. */

  unlink(SOCKET_NAME);

  exit(EXIT_SUCCESS);
}
