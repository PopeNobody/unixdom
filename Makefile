CXXFLAGS:= -ggdb3 -O0 
LDFLAGS:= -static
MAKEFLAGS:= -rR
CXX:=g++
AR:=ar

ALL_X:= $(ACT_X)
ALL_X+= bin/old-client bin/old-server
ALL_X+= bin/fd-server bin/fd-client

ACT_X:= bin/client bin/server bin/errno

all: $(ACT_X) 

LIST_XX:=$(filter-out $(IGN_X_CC),$(sort $(wildcard */*.cc) ))
LIST_CC:=$(filter-out $(IGN_X_CC),$(sort $(wildcard */*.cc) ))
LIST_II:=$(filter-out $(IGN_X_II),$(patsubst %.cc,%.cc.i,$(LIST_CC)))
LIST_OO:=$(filter-out $(IGN_X_OO),$(patsubst %.cc,%.cc.o,$(LIST_CC)))

#LIST_AR:=libxstd.a
LIST_AR_CC:= $(filter-out $(patsubst %,%.cc, $(LIST_XX)), $(LIST_CC))
LIST_AR_OO:= $(patsubst %.cc,%.cc.o,$(LIST_AR_CC)) 
LIST_AR_II:= $(patsubst %.cc,%.cc.i,$(LIST_AR_CC)) 

$(LIST_AR): $(LIST_AR_OO)
	$(AR) -r $@ $^

$(LIST_II): $(wildcard inc/*.hh)

$(warning $(LIST_XX))
$(LIST_XX): %: %.cc.o  etc/ld_flags $(LIST_AR)
	$(CXX)    $< -o $@ @etc/ld_flags

$(LIST_OO): %.cc.o: %.cc.i   etc/cxxflags tmp/all_deps.mk
	$(CXX) -c $< -o $@ @etc/cxxflags

$(LIST_II): %.cc.i: %.cc etc/cppflags
	$(CXX)  -E $< -o $@  @etc/cppflags

tmp/all_deps.mk: scr/gen_dep_list.pl $(LIST_II)
	vi-perl scr/gen_dep_list.pl
	@grep . $@ /dev/null

src/errno.list.cc: errno.cc.i scr/gen_errno_list.pl
	vi-perl scr/gen_errno_list.pl
	@grep . $@ /dev/null

.PRECIOUS: $(LIST_CC) $(LIST_II) $(LIST_OO) $(LIST_XX)

clean:
	rm -f $(ACT_X) $(LIST_II) $(LIST_OO) $(LIST_XX) -r tmp

fd-server fd-client: fd-path.o

