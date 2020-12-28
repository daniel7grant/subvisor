FROM alpine:3

RUN apk add --no-cache build-base git && \
	adduser test -Du 1000

CMD ["/usr/bin/make", "test"]
