LIBS  = -lssl -lcrypto -lcurl
INCS = -I./src/ -Isrc/kaa/utilities/ \
		-Isrc/kaa/collections/ \
		-Isrc/kaa/ \
		-Isrc/kaa/avro_src/ \
		-Isrc/kaa/avro_src/avro/ \
		-Isrc/kaa/gen/ \
		-Isrc/kaa/kaa_protocols/kaa_tcp/ \
		-Isrc/kaa/platform/ \
		-Isrc/kaa/platform-impl/common/ \
		-Isrc/kaa/platform-impl/posix/

SRCS1=$(wildcard src/kaa/collections/*.c) \
		$(wildcard src/kaa/*.c) \
		$(wildcard src/kaa/avro_src/*.c) \
		$(wildcard src/kaa/avro_src/avro/*.c) \
		$(wildcard src/kaa/gen/*.c) \
		$(wildcard src/kaa/kaa_protocols/kaa_tcp/*.c) \
		$(wildcard src/kaa/platform/*.c) \
		$(wildcard src/kaa/platform-impl/common/*.c) \
		$(wildcard src/kaa/platform-impl/posix/*.c) \
		$(wildcard src/kaa/utilities/*.c) 
	
CFLAGS = -g -Wall -Wextra
CFLAGS = -DKAA_MAX_LOG_LEVEL=4

# Should be equivalent to your list of C files, if you don't build selectively

all: kaa_emu_application

linux: kaa_emu_linux

kaa_emu_application: $(SRCS1)
	$(CC) $(INCS) -o $@ $^ $(CFLAGS) $(LIBS)

kaa_emu_linux: $(SRCS1)
	$(CC) $(INCS) -DEMULATOR -o $@ $^ $(CFLAGS) $(LIBS)
	
clean:
	rm -rf kaa_emu_application
