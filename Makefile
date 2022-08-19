CXXFLAGS:= -ggdb3 -O0 

X:= client server old-client old-server

all: $(X)

clean:
	rm -f $(X)

