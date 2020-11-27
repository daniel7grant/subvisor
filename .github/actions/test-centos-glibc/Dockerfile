FROM centos:8

RUN yum makecache && \
	yum -y install git && \
	yum -y groupinstall 'Development Tools'

CMD ["/usr/bin/make", "test"]
