#
# zmq client to modbusd
#

FROM takawang/ubuntu-node-zmq
MAINTAINER Taka Wang <taka@cmwang.net>

## Install node packages
ADD package.json /tmp/package.json
RUN npm install mocha -g && \
    cd /tmp && npm install && \
    mkdir -p /zclient && \
    cp -a /tmp/node_modules /zclient/

## Load test files
WORKDIR /zclient
ADD . /zclient

## Default command
CMD ["mocha", "client2.js"]