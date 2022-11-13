CXXFLAGS:= -ggdb3 -O0 
LDFLAGS:= -static
MAKEFLAGS:= -rR
CXX:=g++
AR:=ar

include /dev/null #$(wildcard all_deps.mk)


ALL_X:= $(ACT_X)
ALL_X+= old-client old-server
ALL_X+= fd-server fd-client

ACT_X:= client server errno

IGN_X:=$(filter-out $(ACT_X),$(ALL_X))
IGN_X_CC:=$(patsubst %,%.cc,$(IGN_X))
IGN_X_II:=$(patsubst %,%.i,$(IGN_X_CC))
IGN_X_OO:=$(patsubst %,%.o,$(IGN_X_CC))

all: $(ACT_X) 

LIST_XX:=$(ACT_X)

LIST_CC:=$(filter-out $(IGN_X_CC),$(sort $(wildcard *.cc)))
LIST_II:=$(filter-out $(IGN_X_II),$(patsubst %.cc,%.cc.i,$(LIST_CC)))
LIST_OO:=$(filter-out $(IGN_X_OO),$(patsubst %.cc,%.cc.o,$(LIST_CC)))

LIST_AR:=libxstd.a
LIST_AR_CC:= $(filter-out $(patsubst %,%.cc, $(LIST_XX)), $(LIST_CC))
LIST_AR_OO:= $(patsubst %.cc,%.cc.o,$(LIST_AR_CC)) 
LIST_AR_II:= $(patsubst %.cc,%.cc.i,$(LIST_AR_CC)) 

$(LIST_AR): $(LIST_AR_OO)
	$(AR) -r $@ $^

$(LIST_II): $(wildcard *.hh)

$(LIST_XX): %: %.cc.o  ld_flags $(LIST_AR)
	$(CXX)    $< -o $@ $(shell cat ld_flags)   

$(LIST_OO): %.cc.o: %.cc.i   cxxflags all_deps.mk
	$(CXX) -c $< -o $@ $(shell cat cxxflags)

$(LIST_II): %.cc.i: %.cc cppflags
	$(CXX)  -E $< -o $@  $(shell cat cppflags)

all_deps.mk: scr/gen_dep_list.pl $(LIST_II)
	vi_perl scr/gen_dep_list.pl

errno.list.cc: errno.cc.i scr/gen_errno_list.pl
	vi_perl scr/gen_errno_list.pl

cxxflags cppflags ld_flags:
	touch $@

.PRECIOUS: $(LIST_CC) $(LIST_II) $(LIST_OO) $(LIST_XX)

clean:
	rm -f $(ACT_X)

fd-server fd-client: fd-path.o

