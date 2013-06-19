all: test_tunnel test_exec test_pcap
CFLAGS=-I../include/ -g -Wall
LDFLAGS=-lssh -L../build/libssh/

test_tunnel: test_tunnel.o authentication.o connection.o
	gcc -o $@ $^ $(LDFLAGS)

test_exec: test_exec.o authentication.o connection.o
	gcc -o $@ $^ $(LDFLAGS)

test_pcap: test_pcap.o
	gcc -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o test_tunnel
