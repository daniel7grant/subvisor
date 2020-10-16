# subvisord

A subset of the supervisord library, entirely rewritten in C, optimized for containers.

## Motivation

## Get started

You only need the subvisord binary ([Installation](#installation)) and an INI-formatted file, the `subvisord.conf`, and you can start running it with the `-c` flag:

```sh
subvisord -c /path/to/file/subvisord.conf
```

By default the subvisord forks, if you want to run it in the foreground (useful for Docker, systemd units and testing) you can add the `-n` flag. For the complete command-line API check `--help`.

### Simple startup

The simplest way is just adding the line `[subvisord]` to a file and list out the programs:

```ini
[subvisord]

[program:sleeping]
command=/bin/sleep 1000
```

## Installation

### Build from source

You can build subvisord from source, by cloning this repository and simply running `make` (it has no other dependencies apart from `make`). By default it works from the `src/` directory, and outputs the binaries to the `build/` directory. You can start the subvisord program by running `subvisord` from the `build/` directory:

```sh
make
build/subvisord --help
```

It should work on any POSIX-compliant OS with any libc (primarily tested on Manjaro + glibc, and Alpine + musl).
