FROM node:4.4.5
MAINTAINER Taka Wang <taka.wang@adlinktech.com>
 
## Set environment variable
ENV DEBIAN_FRONTEND noninteractive

## Install deps
RUN apt-get update && apt-get install -y libzmq3-dev

## Clone modbusd
RUN git clone -b dev https://github.com/taka-wang/modbusd.git
WORKDIR  /modbusd/tests/zclient

## Install app dependencies
RUN npm install

