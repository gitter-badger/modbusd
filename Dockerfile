FROM ubuntu:14.04
MAINTAINER Taka Wang <taka@cmwang.net>
 
## Set environment variable
ENV DEBIAN_FRONTEND noninteractive

## Install required development packages
RUN sudo apt-get update && apt-get install -y git build-essential autoconf libtool pkg-config cmake

RUN rm /bin/sh && ln -s /bin/bash /bin/sh