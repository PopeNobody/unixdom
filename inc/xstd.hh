#ifndef xstd
#define xstd xstd
#include <sys/socket.h>
#include <stdarg.h>
#include <functional>

namespace xstd {
  using namespace std;

  void __err_syserr(bool abort, const char *file, unsigned line, const char *fmt, ...);
  #define err_syserr(fmt,...) __err_syserr(false,__FILE__,__LINE__,fmt, __VA_ARGS__)

  int xopen(const char *filename, int flags, int mode=0);
  void xbind(int sockfd, const sockaddr *addr, const socklen_t len);
};
#endif
