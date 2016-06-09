#
# zmq client to modbusd
#

FROM node:4.4.5
MAINTAINER Taka Wang <taka@cmwang.net>
 
## Set environment variable
ENV DEBIAN_FRONTEND noninteractive

## Install deps
RUN apt-get update && apt-get install -y libzmq3-dev

## Install node packages
ADD package.json /tmp/package.json
RUN cd /tmp && npm install && \
    mkdir -p /zclient && \
    cp -a /tmp/node_modules /zclient/

## Load app files
WORKDIR /zclient
ADD . /zclient

## Default command
CMD ["node", "client2.js"]