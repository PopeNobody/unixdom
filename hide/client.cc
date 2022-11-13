/*
 * File client.c
 */

#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "connection.hh"
#include "xstd.hh"

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

struct pipe_t {
  int ifd;
  int ofd;
  bool done;
  pipe_t()
    :ifd(-1),ofd(-1),done(true)
  {
  }
  pipe_t(int ifd,int ofd)
    :ifd(ifd), ofd(ofd),done(false)
  {
  }
};
pipe_t net_w;//(0,data_socket);
pipe_t net_r;//(data_socket,1);

using namespace xstd;

void open_socks() {
  int fd=0;
  while(fd<FD_SETSIZE){
    fd=xopen("/dev/null",O_RDONLY);
    dprintf(2,"open: %d\n",fd);
  };
  int free=0;
  while(free<4){
    int rand=random()%FD_SETSIZE;
    if(close(rand))
      ++free;
  };
};
int main(int argc, char *argv[])
{
  open_socks();
  open_socket();
  hook_up();
  unlink(SOCKET_NAME);
  //fork_and_dup();
  dprintf(2,"data_socket: %d\n",data_socket);

  net_w = pipe_t(0,data_socket);
  net_r = pipe_t(data_socket,1);
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(0,&fds);
  FD_SET(data_socket,&fds);
  for(int i=0;i<FD_SETSIZE;i++){
    if(FD_ISSET(i,&fds)) {
      const char *socket_id="";
      if(i==data_socket){
        socket_id=": data_socket";
      } else if ( i==0 ) {
        socket_id=": stdin";
      };
      dprintf(2,"FD_ISSET(%d,fd_set)=>1%s\n",i,socket_id);
    };
  };

  char buffer[2];
  while(read(0,buffer,1))
    write(1,buffer,1);
  /* Unlink the socket. */

  unlink(SOCKET_NAME);

  exit(EXIT_SUCCESS);
}
