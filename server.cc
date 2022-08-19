/*
 * File server.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "connection.hh"

int main(int argc, char *argv[])
{
  struct sockaddr_un name;
  int down_flag = 0;
  int ret;
  int connection_socket;
  int data_socket;
  int result;

  /*
   * In case the program exited inadvertently on the last run,
   * remove the socket.
   */

  unlink(SOCKET_NAME);

  /* Create local socket. */

  connection_socket = socket(AF_UNIX, SOCK_STREAM, 0);
  if (connection_socket == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  /*
   * For portability clear the whole structure, since some
   * implementations have additional (nonstandard) fields in
   * the structure.
   */

  memset(&name, 0, sizeof(struct sockaddr_un));

  /* Bind socket to socket name. */

  name.sun_family = AF_UNIX;
  strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);

  ret = bind(connection_socket, (const struct sockaddr *) &name,
      sizeof(struct sockaddr_un));
  if (ret == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  /*
   * Prepare for accepting connections. The backlog size is set
   * to 20. So while one request is being processed other requests
   * can be waiting.
   */

  ret = listen(connection_socket, 20);
  if (ret == -1) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  /* This is the main loop for handling connections. */

  for (;;) {

    /* Wait for incoming connection. */

    data_socket = accept(connection_socket, NULL, NULL);
    if (data_socket == -1) {
      perror("accept");
      exit(EXIT_FAILURE);
    }
    dprintf(2,"accept\n");
    close(connection_socket);

    if(fork()){
      dup2(data_socket,0);
      close(data_socket);
    } else {
      dup2(data_socket,1);
      close(data_socket);
    };
    break;
  }
  char buffer[2];
  while(read(0,buffer,1))
    write(1,buffer,1);
  /* Unlink the socket. */

  unlink(SOCKET_NAME);

  exit(EXIT_SUCCESS);
}

