# modbusd
Modbus daemon 

## Setup development environment
```
sudo apt-get update
sudo apt-get install -y git autoconf libtool 
```
## Setup dependencies

### Install libmodbus library

[libmodbus repo](https://github.com/stephane/libmodbus)

```
git clone https://github.com/stephane/libmodbus/
cd libmodbus
./autogen.sh
./configure
# ./configure --prefix=/usr/local/
sudo make install
```

### Install zeromq library
TODO

---

## TODO List
- libmodbus version number
- FC
- FC argument
- daemon version number
- daemon pid
- zmq
- service (todo)
- read config
- default config
- default port
- keep connection 
- syslog (flag)
- max client (255)