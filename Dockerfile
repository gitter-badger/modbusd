FROM ubuntu:14.04
MAINTAINER Taka Wang <taka@cmwang.net>
 
## Set environment variable
ENV DEBIAN_FRONTEND noninteractive
ENV MB_GIT https://github.com/stephane/libmodbus.git
ENV ZMQ_VER 3.2.5
ENV ZMQ_SRC https://github.com/zeromq/zeromq3-x/releases/download/v$ZMQ_VER/zeromq-$ZMQ_VER.tar.gz
ENV CZMQ_GIT git://github.com/zeromq/czmq.git

## Install required development packages
RUN sudo apt-get update && \
    apt-get install -y git wget build-essential autoconf libtool pkg-config cmake

RUN rm /bin/sh && ln -s /bin/bash /bin/sh

## Install libmodbus
WORKDIR  /
RUN git clone $MB_GIT
WORKDIR  /libmodbus
RUN ./autogen.sh && \
    ./configure && \
    make && \
    make install && \
    ldconfig

## Install libzmq
WORKDIR  /
RUN wget $ZMQ_SRC
RUN tar xvzf zeromq-$ZMQ_VER.tar.gz
WORKDIR /zeromq-$ZMQ_VER
RUN ./configure && \
    make && \
    make install && \
    ldconfig

## Install CZMQ
WORKDIR  /
RUN git clone $CZMQ_GIT
WORKDIR  /czmq
RUN ./autogen.sh && \
    ./configure && \
    make && \
    make install && \
    ldconfig

## Build modbusd
COPY . /modbusd/
RUN mkdir -p /modbusd/build
WORKDIR /modbusd/build
RUN cmake .. && \
    make && \
    make install && \
    cp modbusd /usr/bin/ && \
    cp ../modbusd.json /usr/bin

EXPOSE 502