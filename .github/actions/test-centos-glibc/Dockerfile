FROM centos:8

RUN yum makecache && \
	yum -y install git && \
	yum -y groupinstall 'Development Tools' && \
	useradd test -u 1000

CMD ["/usr/bin/make", "test"]
