#ifndef fd_path_h
#define fd_path_h fd_path_h

#include <sys/un.h>
#include <sys/socket.h>

union address_t {
  sockaddr_un sun;
  sockaddr    sad;
  operator sockaddr*() {
    return &sad;
  };
};

extern address_t unix_path();

inline int pexit(const char *msg) {
  perror(msg);
  exit(1);
};

#endif
