CFLAGS=-g -Wall -Wextra -O0
LDFLAGS=

default: build/subvisord

build:
	mkdir -p build

build/config: build
	mkdir -p build/config

build/logger: build
	mkdir -p build/logger

build/subvisord: build src/subvisord.c build/config/arguments.o build/config/baseconfig.o build/config/config.o build/config/programconfig.o build/logger/logger.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o build/subvisord src/subvisord.c build/config/arguments.o build/config/baseconfig.o build/config/config.o build/config/programconfig.o build/logger/logger.o

build/config/arguments.o: build/config src/config/arguments.c
	$(CC) $(CFLAGS) -c -o build/config/arguments.o src/config/arguments.c

build/config/baseconfig.o: build/config src/config/baseconfig.c
	$(CC) $(CFLAGS) -c -o build/config/baseconfig.o src/config/baseconfig.c

build/config/config.o: build/config src/config/config.c
	$(CC) $(CFLAGS) -c -o build/config/config.o src/config/config.c

build/config/programconfig.o: build/config src/config/programconfig.c
	$(CC) $(CFLAGS) -c -o build/config/programconfig.o src/config/programconfig.c

build/logger/logger.o: build/logger src/logger/logger.c
	$(CC) $(CFLAGS) -c -o build/logger/logger.o src/logger/logger.c
