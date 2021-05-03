CFLAGS = -std=gnu17 -Wpedantic -Wall -O0 -pipe -fno-plt -fPIC
LDFLAGS = -lrt -lpthread -Wl,-O1,--sort-common,--as-needed,-z,relro,-z,now

.PHONY: all
all: create-fork create-pthread

create-fork: create-fork.o

create-pthread: create-pthread.o

.PHONY: clean
clean:
	rm -f create-fork.o create-fork
	rm -f create-pthread.o create-pthread
