CFLAGS = -std=gnu17 -Wpedantic -Wall -O0 -pipe -fno-plt -fPIC -pthread
LDFLAGS = -lrt -pthread -Wl,-O1,--sort-common,--as-needed,-z,relro,-z,now

.PHONY: all
all: server client

server: server.o

client: client.o

.PHONY: clean
clean:
	rm -f server.o server
	rm -f client.o client
