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

## Modbus function code
| FC    | Description            | API                                                                                             |
|:-----:|------------------------|-------------------------------------------------------------------------------------------------|
| 0x01  | read coils             | [modbus_read_bits](http://libmodbus.org/docs/v3.1.4/modbus_read_bits.html)                      |   
| 0x02  | read discrete input    | [modbus_read_input_bits](http://libmodbus.org/docs/v3.1.4/modbus_read_input_bits.html)          |
| 0x03  | read holding registers | [modbus_read_registers](http://libmodbus.org/docs/v3.1.4/modbus_read_registers.html)            |
| 0x04  | read input registers   | [modbus_read_input_registers](http://libmodbus.org/docs/v3.1.4/modbus_read_input_registers.html)|
| 0x05  | write single coil      | [modbus_write_bit](http://libmodbus.org/docs/v3.1.4/modbus_write_bit.html)                      |
| 0x06  | write single register  | [modbus_write_register](http://libmodbus.org/docs/v3.1.4/modbus_write_register.html)            |
| 0x0F  | write multi coil       | [modbus_write_bits](http://libmodbus.org/docs/v3.1.4/modbus_write_bits.html)                    |
| 0x10  | write multi registers  | [modbus_write_registers](http://libmodbus.org/docs/v3.1.4/modbus_write_registers.html)          |

---

## TODO List
- libmodbus version number
- keep connection 
- FC(1~6, 15, 16)
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
- [libmodbus](http://libmodbus.org/)
- [libzmq](https://github.com/zeromq/libzmq)
- [czmq](https://github.com/zeromq/czmq)
- [uthash](https://troydhanson.github.io/uthash/)

---

## Documentations
- [uthash user guide](http://troydhanson.github.io/uthash/userguide.html)
- [libmodbus api document](http://libmodbus.org/docs/v3.1.4/)