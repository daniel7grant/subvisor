CFLAGS=-g -Wall -Wextra -Wno-unused-parameter -O0
LDFLAGS=

default: build/subvisord

build:
	mkdir -p build

build/config: build
	mkdir -p build/config

build/logger: build
	mkdir -p build/logger

build/process: build
	mkdir -p build/process

build/tests: build
	mkdir -p build/process

build/utils: build
	mkdir -p build/utils

build/subvisord: build src/subvisord.c build/config/arguments.o build/config/config.o build/config/configtypes.o \
build/config/programconfig.o build/logger/logger.o build/process/process.o build/utils/platform.o build/utils/utils.o build/utils/utils.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o build/subvisord src/subvisord.c \
		build/config/arguments.o \
		build/config/config.o \
		build/config/configtypes.o \
		build/config/programconfig.o \
		build/logger/logger.o \
		build/process/process.o \
		build/utils/platform.o \
		build/utils/utils.o

build/config/arguments.o: build/config src/config/arguments.c
	$(CC) $(CFLAGS) -c -o build/config/arguments.o src/config/arguments.c

build/config/config.o: build/config src/config/config.c
	$(CC) $(CFLAGS) -c -o build/config/config.o src/config/config.c

build/config/configtypes.o: build/config src/config/configtypes.c
	$(CC) $(CFLAGS) -c -o build/config/configtypes.o src/config/configtypes.c

build/config/programconfig.o: build/config src/config/programconfig.c
	$(CC) $(CFLAGS) -c -o build/config/programconfig.o src/config/programconfig.c

build/logger/logger.o: build/logger src/logger/logger.c
	$(CC) $(CFLAGS) -c -o build/logger/logger.o src/logger/logger.c

build/process/process.o: build/process src/process/process.c
	$(CC) $(CFLAGS) -c -o build/process/process.o src/process/process.c

build/utils/platform.o: build/utils src/utils/platform.c
	$(CC) $(CFLAGS) -c -o build/utils/platform.o src/utils/platform.c

build/utils/utils.o: build/utils src/utils/utils.c
	$(CC) $(CFLAGS) -c -o build/utils/utils.o src/utils/utils.c

test: unity build/test
	./build/test

build/test: build build/testutils.o src/subvisord.c build/config/arguments.o build/config/config.o build/config/configtypes.o \
build/config/programconfig.o build/logger/logger.o build/process/process.o build/utils/platform.o build/utils/utils.o build/utils/utils.o
	$(CC) $(CFLAGS) -o build/test tests/test.c \
		unity/src/unity.c \
		build/testutils.o \
		build/config/arguments.o \
		build/config/config.o \
		build/config/configtypes.o \
		build/config/programconfig.o \
		build/logger/logger.o \
		build/process/process.o \
		build/utils/platform.o \
		build/utils/utils.o

build/testutils.o: build tests/testutils.c
	$(CC) $(CFLAGS) -c -o build/testutils.o tests/testutils.c

unity:
	[[ -d unity ]] || git clone https://github.com/ThrowTheSwitch/Unity.git unity

.PHONY: test
