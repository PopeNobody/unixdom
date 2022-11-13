#include <errno.h>
#include <string>
#include <string.h>
#include <iostream>
#include <vector>
#include <array>

using namespace std;

struct errno_t {
  struct data_t {
    int code;
    string name;
    string emsg;
    data_t(int code, const string &name)
      : code(code),name(name)
    {
    }
  };
  data_t data;
  errno_t(int code, const string &name)
    : data(code,name)
  {
  };
  string name() const {
    return data.name;
  };
  string emsg() const {
    if(!data.emsg.size())
      const_cast<string&>(data.emsg)=strerror(data.code);
    return data.emsg;
  };
};
vector<errno_t> errnos;

int main(int, char**){
  return 0;
};
