# modbusd
Modbus daemon 

## Setup development dependencies
```bash
sudo apt-get update
sudo apt-get install -y git autoconf libtool 
```

---

## Setup OSS libs dependencies

### Install libmodbus library

Currently, we use libmodbus version 3.1.4.

```bash
git clone https://github.com/stephane/libmodbus/
cd libmodbus
./autogen.sh
./configure
# ./configure --prefix=/usr/local/
sudo make install
sudo ldconfig
```

#### Install libzmq(zeromq) 3.2.5

```bash
wget http://download.zeromq.org/zeromq-3.2.5.tar.gz
tar xvzf zeromq-3.2.5.tar.gz
cd zeromq-3.2.5
./configure
make
sudo make install
sudo ldconfig # refresh shared library cache
#sudo ln -sf /usr/local/lib/libzmq.so /usr/lib/libzmq.so
```

#### Install czmq (high-level C binding for zeromq)

```bash
git clone git://github.com/zeromq/czmq.git
cd czmq
./autogen.sh
./configure
make
sudo make install
sudo ldconfig
```

---

## TODO List
- libmodbus version number
- keep connection 
- FC
- FC argument
- daemon version number
- daemon pid
- zmq
- service (todo)
- read config
- default config
- default port
- syslog (flag)
- max client (255)

## External libraries list
- libmodbus
- libzmq
- czmq
- uthash

---

## Documentations
- [uthash user guide](http://troydhanson.github.io/uthash/userguide.html)
- [libmodbus api document](http://libmodbus.org/docs/v3.1.4/)