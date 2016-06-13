#
# modbus simulation slave
#
FROM takawang/ubuntu-node-zmq
MAINTAINER Taka Wang <taka@cmwang.net>

## Install node packages
ADD package.json /tmp/package.json
RUN cd /tmp && npm install && \
    mkdir -p /mbserver && \
    cp -a /tmp/node_modules /mbserver/

## Load app files
WORKDIR /mbserver
ADD . /mbserver

## Default exported port
EXPOSE 502

## Default command
CMD [ "node", "server.js" ]

