#ifndef errno_hh
#define errno_hh errno_hh

#include <string>
#include <vector>

namespace err {
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
    static const vector<errno_t> &errs();
  };
}

#endif
