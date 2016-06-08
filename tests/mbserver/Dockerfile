FROM node:4-onbuild
MAINTAINER Taka Wang <taka.wang@adlinktech.com>
 
## Set environment variable
ENV DEBIAN_FRONTEND noninteractive

## Clone modbusd
RUN git clone -b dev https://github.com/taka-wang/modbusd.git
WORKDIR  /modbusd/tests/mbserver

