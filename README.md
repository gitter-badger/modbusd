# modbusd
Modbus daemon 

## Setup development dependencies

```bash
sudo apt-get update
sudo apt-get install -y git build-essential autoconf libtool pkg-config cmake
```

---

## Setup OSS libs dependencies

### Install libmodbus library (3.1.4)

```bash
git clone https://github.com/stephane/libmodbus/
cd libmodbus
./autogen.sh
./configure
# ./configure --prefix=/usr/local/
make
sudo make install
sudo ldconfig
```

### Install libzmq (3.2.5)

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

### Install czmq (high-level C binding for zeromq)

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

| FC    | Description            |  API                                                                                                                                                 |
|:-----:|------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------|
| 0x01  | read coils             | [int modbus_read_bits(modbus_t *ctx, int addr, int nb, uint8_t *dest)](http://libmodbus.org/docs/v3.1.4/modbus_read_bits.html)                       |  
| 0x02  | read discrete input    | [int modbus_read_input_bits(modbus_t *ctx, int addr, int nb, uint8_t *dest)](http://libmodbus.org/docs/v3.1.4/modbus_read_input_bits.html)           |
| 0x03  | read holding registers | [int modbus_read_registers(modbus_t *ctx, int addr, int nb, uint16_t *dest)](http://libmodbus.org/docs/v3.1.4/modbus_read_registers.html)            |
| 0x04  | read input registers   | [int modbus_read_input_registers(modbus_t *ctx, int addr, int nb, uint16_t *dest)](http://libmodbus.org/docs/v3.1.4/modbus_read_input_registers.html)|
| 0x05  | write single coil      | [int modbus_write_bit(modbus_t *ctx, int addr, int status)](http://libmodbus.org/docs/v3.1.4/modbus_write_bit.html)                                  |
| 0x06  | write single register  | [int modbus_write_register(modbus_t *ctx, int addr, int value)](http://libmodbus.org/docs/v3.1.4/modbus_write_register.html)                         |
| 0x0F  | write multi coils      | [int modbus_write_bits(modbus_t *ctx, int addr, int nb, const uint8_t *src)](http://libmodbus.org/docs/v3.1.4/modbus_write_bits.html)                |
| 0x10  | write multi registers  | [int modbus_write_registers(modbus_t *ctx, int addr, int nb, const uint16_t *src)](http://libmodbus.org/docs/v3.1.4/modbus_write_registers.html)     |

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

- [libmodbus](http://libmodbus.org)
- [libzmq](https://github.com/zeromq/libzmq)
- [czmq](https://github.com/zeromq/czmq)
- [uthash](https://troydhanson.github.io/uthash)
- [cJSON](https://github.com/DaveGamble/cJSON)

---

## Documentations

- [uthash user guide](http://troydhanson.github.io/uthash/userguide.html)
- [libmodbus api document](http://libmodbus.org/docs/v3.1.4/)
- [libmodbus header](https://github.com/stephane/libmodbus/blob/master/src/modbus.h)
- [cJSON examples](https://github.com/DaveGamble/cJSON)