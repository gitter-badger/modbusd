FROM takawang/ubuntu-modbus
MAINTAINER Taka Wang <taka@cmwang.net>

## Get ENV
ARG MODBUSD_VERSION=local
ENV MODBUSD_VERSION ${MODBUSD_VERSION}

## test
RUN echo $MODBUSD_VERSION

## Build modbusd
COPY . /modbusd/
RUN mkdir -p /modbusd/build
WORKDIR /modbusd/build
RUN cmake .. && \
    make && \
    make install

## Install init script
WORKDIR /modbusd/
RUN cp "config/service.sh" "/etc/init.d/modbusd" && \
    chmod +x /etc/init.d/modbusd && \
    update-rc.d modbusd defaults

## Default exported port
EXPOSE 502

## Default command
# note: log file does not exist, just keep container running
CMD service modbusd start && tail -F /var/log/modbusd.log