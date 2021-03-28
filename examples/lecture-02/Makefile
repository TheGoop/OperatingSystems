CFLAGS = -std=c17 -Wpedantic -Wall -O2 -pipe -fno-plt
LDFLAGS = -Wl,-O1,--sort-common,--as-needed,-z,relro,-z,now

.PHONY: all
all: hello-world-c

hello-world-c: hello-world-c.o

.PHONY: clean
clean:
	rm -f hello-world-c.o hello-world-c
