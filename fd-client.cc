//#include "stderr.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include "fd-path.h"

void err_remark(const char *msg){
  dprintf(2,"%s\n",msg);
};
void err_syserr(const char *msg){
  dprintf(2,"%s\n",msg);
};
static
void wyslij(int socket, int fd)  // send fd by socket
{
    struct msghdr msg = { 0 };
    char buf[CMSG_SPACE(sizeof(fd))];
    memset(buf, '\0', sizeof(buf));
    struct iovec io = { .iov_base = (void*)"ABC", .iov_len = 3 };

    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);

    struct cmsghdr * cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(fd));

    *((int *) CMSG_DATA(cmsg)) = fd;

    msg.msg_controllen = CMSG_SPACE(sizeof(fd));

    if (sendmsg(socket, &msg, 0) < 0)
        err_syserr("Failed to send message\n");
}

static
int odbierz(int socket)  // receive fd from socket
{
    struct msghdr msg = {0};

    char m_buffer[256];
    struct iovec io = { .iov_base = m_buffer, .iov_len = sizeof(m_buffer) };
    msg.msg_iov = &io;
    msg.msg_iovlen = 1;

    char c_buffer[256];
    msg.msg_control = c_buffer;
    msg.msg_controllen = sizeof(c_buffer);

    if (recvmsg(socket, &msg, 0) < 0)
        err_syserr("Failed to receive message\n");

    struct cmsghdr * cmsg = CMSG_FIRSTHDR(&msg);

    unsigned char * data = CMSG_DATA(cmsg);

    dprintf(2,"About to extract fd\n");
    int fd = *((int*) data);
    dprintf(2,"Extracted fd %d\n", fd);

    return fd;
}

int main(int argc, char **argv)
{
  const char *filename = "./z7.c";

  if (argc > 1)
    filename = argv[1];

  err_remark("Child at play\n");

  int sock = socket(AF_UNIX,SOCK_STREAM,0);

  address_t addr=unix_path();

  if(connect(sock,addr,sizeof(addr)))
    pexit("connect");

  struct timespec spec = { .tv_sec = 0, .tv_nsec = 500000000}; 
  nanosleep(&spec,0);

  int fd = odbierz(sock);

  char buffer[256];
  ssize_t nbytes;
  while ((nbytes = read(fd, buffer, sizeof(buffer))) > 0)
    write(1, buffer, nbytes);
  printf("Done!\n");
  close(fd);
  return 0;
}
