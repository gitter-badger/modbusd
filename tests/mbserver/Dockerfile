#
# modbus simulation slave
#
FROM node:4.4.5
MAINTAINER Taka Wang <taka.wang@adlinktech.com>
 
## Set environment variable
ENV DEBIAN_FRONTEND noninteractive

## Install node packages
ADD package.json /tmp/package.json
RUN cd /tmp && npm install && \
    mkdir -p /mbserver && \
    cp -a /tmp/node_modules /mbserver/

EXPOSE 502
CMD [ "node", "server.js" ]

