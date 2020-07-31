CFLAGS=-g -Wall -Wextra -O0
LDFLAGS=

default: build/subvisord

build:
	mkdir -p build

build/config: build
	mkdir -p build/config

build/logger: build
	mkdir -p build/logger

build/subvisord: build src/subvisord.c build/config/arguments.o build/config/config.o build/config/programconfig.o build/logger/logger.o  build/utils/platform.posix.o build/utils/utils.o build/utils/utils.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o build/subvisord src/subvisord.c \
		build/config/arguments.o \
		build/config/config.o \
		build/config/programconfig.o \
		build/logger/logger.o \
		build/utils/platform.posix.o \
		build/utils/platform.win.o \
		build/utils/utils.o

build/config/arguments.o: build/config src/config/arguments.c
	$(CC) $(CFLAGS) -c -o build/config/arguments.o src/config/arguments.c

build/config/config.o: build/config src/config/config.c
	$(CC) $(CFLAGS) -c -o build/config/config.o src/config/config.c

build/config/programconfig.o: build/config src/config/programconfig.c
	$(CC) $(CFLAGS) -c -o build/config/programconfig.o src/config/programconfig.c

build/logger/logger.o: build/logger src/logger/logger.c
	$(CC) $(CFLAGS) -c -o build/logger/logger.o src/logger/logger.c

build/utils/platform.posix.o: build src/utils/platform.posix.c
	$(CC) $(CFLAGS) -c -o build/utils/platform.posix.o src/utils/platform.posix.c

build/utils/platform.win.o: build src/utils/platform.win.c
	$(CC) $(CFLAGS) -c -o build/utils/platform.win.o src/utils/platform.win.c

build/utils/utils.o: build src/utils/utils.c
	$(CC) $(CFLAGS) -c -o build/utils/utils.o src/utils/utils.c
