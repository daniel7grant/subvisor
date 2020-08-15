# TODO list

## Supervisord core

Core features of the supervisord specification, this should be implemented before all.

* ~~read configurations from conf file~~
* ~~read configurations from arguments~~
* ~~choose configuration file in order~~
* ~~validate configuration~~
* allow environmental variables / templating
* ~~configure process forking~~
* ~~spawn processes~~
* ~~monitor process states~~
* ~~restart processes automatically~~
* start multiple processes
* redirect stdout and stderr
* rotate logs of processes
* handle processes as groups
* pidproxy for processes

## Supervisord possible

Additional features of supervisord, these are to be decided for implementation.

* RPC-XML interface
* FCGI
* events

## Supervisord extensions

Extensions of supervisord, to further aid usage in containers.

* add configuration to stop supervisor if process fails (critical)
* add healthcheck option
* add reload processes option (USR2)
* add dry-run (test configuration) flag
* redirect all logs to stdout

## PID 1 responsibilities

Handle the classic PID 1 problem, to allow running as an init process.

* reap zombie processes
* handle signals

## Others
* add licence