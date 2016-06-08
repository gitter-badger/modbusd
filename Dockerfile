FROM ubuntu:14.04
MAINTAINER Taka Wang <taka@cmwang.net>
 
## Set environment variable
ENV DEBIAN_FRONTEND noninteractive

## Install required development packages
RUN sudo apt-get update && apt-get install -y git wget build-essential autoconf libtool pkg-config cmake

RUN rm /bin/sh && ln -s /bin/bash /bin/sh

## Install libmodbus
WORKDIR  /
RUN git clone https://github.com/stephane/libmodbus/
WORKDIR  /libmodbus
RUN ./autogen.sh && \
    ./configure && \
    make && make install && ldconfig

## Install libzmq
WORKDIR  /
RUN wget https://github.com/zeromq/zeromq3-x/releases/download/v3.2.5/zeromq-3.2.5.tar.gz
RUN tar xvzf zeromq-3.2.5.tar.gz
WORKDIR /zeromq-3.2.5
RUN ./configure && make && make install && ldconfig

## Install CZMQ
WORKDIR  /
RUN git clone git://github.com/zeromq/czmq.git
WORKDIR  /czmq
RUN ./autogen.sh && ./configure && make && make install && ldconfig

## Clone modbusd
WORKDIR  /
RUN git clone -b dev https://github.com/taka-wang/modbusd.git
RUN mkdir -p /modbusd/build
WORKDIR /modbusd/build
RUN cmake .. && make && make install

EXPOSE 502