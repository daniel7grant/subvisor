FROM debian:10

RUN apt-get update && \
	apt-get -y install build-essential git && \
	useradd test -u 1000

CMD ["/usr/bin/make", "test"]
