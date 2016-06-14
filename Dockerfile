FROM takawang/ubuntu-modbus
MAINTAINER Taka Wang <taka@cmwang.net>

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
CMD service modbusd start && tail -F /tmp/modbusd.log