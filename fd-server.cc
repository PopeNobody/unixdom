//#include "stderr.h"
#include <sys/un.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <string>

using std::string;

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

union address_t {
  sockaddr_un sun;
  sockaddr    sad;
  operator sockaddr*() {
    return &sad;
  };
};

int main(int argc, char **argv)
{
  const char *filename = "./z7.c";

  if (argc > 1)
    filename = argv[1];

  string path=getenv("HOME");
  path+="/";
  path+="fdpass.sock";
  int s = socket(AF_UNIX, SOCK_STREAM, 0);
  address_t sun;
  sun.sun.sun_family=AF_UNIX;
  strcpy(sun.sun.sun_path,path.c_str());

  unlink(sun.sun.sun_path); 
  err_remark(sun.sun.sun_path); 
  if(bind(s,sun,sizeof(sun))) {
    perror("bind");
    exit(1);
  };
  listen(s,5);
  address_t peer;
  socklen_t size=sizeof(peer);
  int sock = accept(s, peer, &size);
  if(sock<0){
    perror("accept");
    exit(2);
  };
//     int fd = open(filename, O_RDONLY);
//     if (fd < 0)
//       dprintf(2,"Failed to open file %s for reading\n", filename);
//   
//     wyslij(sock, fd);
//   
//     close(fd);
//     struct timespec spec = { .tv_sec = 1, .tv_nsec = 500000000};
//     nanosleep(&spec , 0);
//     err_remark("Parent exits\n");
  return 0;
}
