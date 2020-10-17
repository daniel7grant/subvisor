# subvisord

A subset of the supervisord library, entirely rewritten in C, optimized for containers.

## Motivation

## Get started

You only need the subvisord binary ([Installation](#installation)) and an INI-formatted file, the `subvisord.conf`, and you can start running it with the `-c` flag:

```sh
subvisord -c /path/to/file/subvisord.conf
```

By default the subvisord daemonizes itself, if you want to run it in the foreground (useful for Docker, systemd units and testing) you can add the `-n` flag. For the complete command-line API check `--help`.

### Simple startup

The subvisord configuration is a Windows-style INI file, in which you have to define sections (in square brackets, e.g. `[subvisord]`), and for each section some or none key-value pairs (like `nodaemon=true`) line-by-line. It is important to note that subvisord tries to be as compatible with the supervisord configuration files as it can however it also accepts all `supervisor` containing options as `subvisor`, because why type so much...

The simplest way is just creating new file with the section `[subvisord]` (this is where the global configuration resides) and list out the programs as separate sections, with the name `[program:<uniquename>]`. The only requirement for program blocks is to define the `command` which should be executed (remember that this process should be started in the foreground).

You can start a basic webserver and php process with the following configuration:

```ini
[subvisord]

[program:nginx]
command=/usr/sbin/nginx -g 'daemon off;'

[program:php-fpm]
command=/usr/sbin/php-fpm7 -F
```

### Advanced configuration

For subprocesses subvisord has sane defaults: they are autostarted on startup and restarted in case of an unexpected exit. However if you want to define one-shot processes (processes that run and then exit expectedly), or change the failure exit codes or restart times and retries, you can do that too.

```ini
[subvisord]

[program:nginx]
command=/usr/sbin/nginx -g 'daemon off;'
# We set to always autorestart (even if it exists with 0)
# and retry it 10 times before considering it the state FATAL
autorestart=true
startretries=10

# Don't restart this process (one-shot)
[program:clear-cache]
command=/bin/rm -rf /var/cache/nginx
autorestart=false
```

### Privileges configuration

Subvisord is often started as root, however the principle of least privilege suggests to prefer processes started as non-root users. You can still keep running starting subvisord as root, since you can set the `user` key in the subvisord configurations to switch to another user on startup. You can either define globally in the `[subvisord]` section (then all processes will be started as that user), or set it for program sections one by one. 

You can use the global `user` setting:

```ini
[subvisord]
user=www

# These processes will start with the `www` user
[program:nginx]
command=/usr/sbin/nginx -g 'daemon off;'

[program:php-fpm]
command=/usr/sbin/php-fpm7 -F
```

or for each program one-by-one:

```ini
[subvisord]

[program:nginx]
command=/usr/sbin/nginx -g 'daemon off;'
user=nginx

[program:php-fpm]
command=/usr/sbin/php-fpm7 -F
user=php
```

### Logging configuration

One of the primary advantage of using subvisord is that it can manage and rotate the standard output and error for its subprocesses. By default it automatically creates log files for each subprocess in the temp directory (it can be modified in the global configuration with the `childlogdir` setting). It is however often preferred to manage the log files separately for each program: `stdout_logfile` and `stderr_logfile` can handle the output and error streams(or redirect error logs to stderr with `redirect_srderr`). Logfiles are also automatically rotated (moved to separate files) every 50MBs (or configured with `stdout/stderr_maxbytes`), until it reaches 10 files (or `stdout/stderr_backups`), after then, the oldest files are deleted. These settings allow a versatile handlings of output logging, without requiring external libraries.

Set logfiles for subprocesses via `childlogdir`:

```ini
[subvisord]
childlogdir=/var/log/subvisor
logfile = /tmp/supervisord.log
logfile_maxbytes = 50MB
logfile_backups=10
loglevel = info

# Output will be saved in /var/log/subvisor/nginx--stdout--{hash}.log
[program:nginx]
command=/usr/sbin/nginx -g 'daemon off;'
```

```ini
# Log parent debugging messages without rotation to /var/log
[subvisord]
logfile = /var/log/supervisord.log
logfile_maxbytes = 0
loglevel = debug

# Write logs to access and error files, with default rotation
[program:nginx]
command=/usr/sbin/nginx -g 'daemon off;'
stdout_logfile=/var/log/supervisor/nginx.access.log
stderr_logfile=/var/log/supervisor/nginx.error.log

# Collect stdout and stderr to one log file with 5MB rotations
[program:php-fpm]
command=/usr/sbin/php-fpm7 -F
redirect_stderr=true
stdout_logfile=/var/log/supervisor/php-fpm.log
stdout_logfile_maxbytes=5MB
stdout_logfile_backups=10
```

## Installation

### Build from source

You can build subvisord from source, by cloning this repository and simply running `make` (it has no other dependencies apart from `make`). By default it works from the `src/` directory, and outputs the binaries to the `build/` directory. You can start the subvisord program by running `subvisord` from the `build/` directory:

```sh
make
build/subvisord --help
```

It should work on any POSIX-compliant OS with any libc (primarily tested on Manjaro + glibc, and Alpine + musl).
