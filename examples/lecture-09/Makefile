CFLAGS = -std=gnu17 -Wpedantic -Wall -O0 -pipe -fno-plt -fPIC
LDFLAGS = -lrt -Wl,-O1,--sort-common,--as-needed,-z,relro,-z,now

.PHONY: all
all: srtf

srtf: srtf.o

.PHONY: clean
clean:
	rm -f srtf.o srtf
