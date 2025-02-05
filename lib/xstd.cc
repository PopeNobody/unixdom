#include "xstd.hh"
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

namespace xstd {
  using namespace std;

  void __err_syserr(bool abort, const char *file, unsigned line, const char *fmt, ...){
    va_list list;
    va_start(list,fmt);
    vdprintf(2,fmt,list);
    va_end(list);
    if(abort)
      exit(1);
  };
  int xopen(const char *filename, int flags, int mode){
    int fd=open(filename,flags,mode);
    if(fd<0)
      err_syserr("failed to open '%s': %m (%d)\n",filename,errno);
    return fd;
  }
  void xbind(int sockfd, const sockaddr *addr, const socklen_t len) {
    int res=bind(sockfd,addr,len);
    if(res==0)
      return;
    err_syserr("bind(%d,%p,%u) => %s\n",sockfd,addr,len,strerror(errno));
  };
};
