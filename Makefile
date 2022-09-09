CXXFLAGS:= -ggdb3 -O0 
LDFLAGS:= -static

X:= client server old-client old-server
X+= fd-server fd-client

all: $(X)

clean:
	rm -f $(X)

