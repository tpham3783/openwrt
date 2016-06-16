
LIBS  = -lssl -lcrypto -lcurl
INCS = -I./src/ \
		-Isrc/kaa/platform-impl/common/ \
		-I.src/kaa/platform/ \
		-Isrc/kaa/platform-impl/posix/ \
		-Isrc/kaa/utilities/ \
		-Isrc/kaa/collections/ \
		-Isrc/kaa/ \
		-Isrc/kaa/avro_src/ \
		-Isrc/kaa/avro_src/avro/ \
		-Isrc/kaa/gen/ \
		-Isrc/kaa/kaa_protocols/kaa_tcp/ 
		
#-I/projects/ap102b.telit/staging_dir/toolchain-mipsel_24kec+dsp_gcc-4.8-linaro_musl-1.1.10/include/ \


SRCS1=$(filter-out src/kaa/platform-impl/common/sha.c,$(wildcard src/kaa/platform-impl/common/*.c))  \
		$(wildcard src/kaa/platform/*.c) \
		$(wildcard src/kaa/platform-impl/posix/*.c) \
		$(wildcard src/kaa/utilities/*.c)  \
		$(wildcard src/kaa/collections/*.c) \
		$(wildcard src/kaa/*.c) \
		$(wildcard src/kaa/avro_src/*.c) \
		$(wildcard src/kaa/avro_src/avro/*.c) \
		$(wildcard src/kaa/gen/*.c) \
		$(wildcard src/kaa/kaa_protocols/kaa_tcp/*.c) \



CFLAGS =  -Wall -Wextra -pg
CFLAGS = -DKAA_MAX_LOG_LEVEL=4 -g

# Should be equivalent to your list of C files, if you don't build selectively

all: kaa_emu_application

linux: kaa_emu_linux

kaa_emu_application: $(SRCS1)
	$(CC) $(INCS) -o $@ $^ $(CFLAGS) $(LIBS)

kaa_emu_linux: $(SRCS1)
	$(CC) $(INCS) -DEMULATOR -o $@ $^ $(CFLAGS) $(LIBS)
	
#.PHONY: clean

clean:
	rm -rf kaa_emu_application



 
