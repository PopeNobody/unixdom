#include "fd-path.h"
#include <string>
#include <stdlib.h>

using std::string;

address_t unix_path() {
  const char *home=getenv("HOME");
  if(!home) {
    dprintf(2,"home not set");
    exit(1);
  };
  
  string path=home;
  path+="/";
  path+="fdpass.sock";

  address_t sun;
  sun.sun.sun_family=AF_UNIX;
  strcpy(sun.sun.sun_path,path.c_str());
  return sun;
};
