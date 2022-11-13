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

int connection_socket;
int data_socket;
struct sockaddr_un addr;
int ret;

//   void fork_and_dup() {
//       if(fork()){
//         //shutdown(data_socket,SHUT_RD);
//         dup2(data_socket,0);
//         close(data_socket);
//       } else {
//         //shutdown(data_socket,SHUT_WR);
//         dup2(data_socket,1);
//         close(data_socket);
//       };
//   };
void bind_and_accept() {
  int ret = bind(connection_socket, (const struct sockaddr *) &addr,
      sizeof(struct sockaddr_un));
  if (ret == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  ret = listen(connection_socket, 20);
  if (ret == -1) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
  data_socket = accept(connection_socket, NULL, NULL);


  dprintf(2,"accept\n");
  if (data_socket == -1) {
    perror("accept");
    exit(EXIT_FAILURE);
  }
  close(connection_socket);
};
void open_socket() {
  connection_socket = socket(AF_UNIX, SOCK_STREAM, 0);
  if (connection_socket == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }
  memset(&addr, 0, sizeof(struct sockaddr_un));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);
}
void hook_up() {
  data_socket=connection_socket;
  connection_socket=-1;
  ret = connect (data_socket, (const struct sockaddr *) &addr,
      sizeof(struct sockaddr_un));
  if (ret == -1) {
    perror("connect");
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char *argv[])
{
  open_socket();
  hook_up();
  unlink(SOCKET_NAME);
  //fork_and_dup();
  dprintf(2,"data_socket: %d\n",data_socket);

  char buffer[2];
  while(read(0,buffer,1))
    write(1,buffer,1);
  /* Unlink the socket. */

  unlink(SOCKET_NAME);

  exit(EXIT_SUCCESS);
}
