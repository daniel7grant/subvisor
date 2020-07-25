# subvisord

A subset of the supervisord library, entirely rewritten in C.

## TODO
Supervisord core:
* ~~read configurations from conf file~~
* ~~read configurations from arguments~~
* ~~choose configuration file in order~~
* validate configuration
* allow environmental variables
* configure process forking
* spawn processes with popen
* monitor process states
* restart processes automatically
* redirect stdout and stderr
* rotate logs of processes
* handle processes as groups
* pidproxy for processes

Supervisord possible:
* FCGI + other interfaces
* events

Supervisord extensions:
* add healthcheck option
* add reload processes option (USR2)
* add configuration to stop supervisor if process fails (critical)
* redirect all logs to stdout

PID 1 responsibilities:
* reap zombie processes
* handle signals

Others:
* add licence