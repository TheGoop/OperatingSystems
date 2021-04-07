CFLAGS = -std=gnu17 -Wpedantic -Wall -O2 -pipe -fno-plt -fPIC
LDFLAGS = -lrt -Wl,-O1,--sort-common,--as-needed,-z,relro,-z,now

.PHONY: all
all: read-write-example open-example signal-example signal-example-2 shared-memory-example

read-write-example: read-write-example.o

open-example: open-example.o

signal-example: signal-example.o

signal-example-2: signal-example-2.o

shared-memory-example: shared-memory-example.o

.PHONY: clean
clean:
	rm -f read-write-example.o read-write-example
	rm -f open-example.o open-example
	rm -f signal-example.o signal-example
	rm -f signal-example-2.o signal-example-2
	rm -f shared-memory-example.o shared-memory-example
