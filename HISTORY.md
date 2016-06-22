# v0.2.1

## Done
- [x] fix uthash sizeof issue (1000 items); cause: memset to the wrong size 
- [x] fix char pointer key issue (hash); use char array instead of char pointer (unkown length)
- [x] modbus_connect hang issue; set tcp timeout
- [x] handle 'reset by peer' issue; workaround: set connection flag to false :warning:
- [x] implement keep connection mechanism via hashtable
- [x] implement FC (1~6, 15, 16)
- [x] assign daemon version number from the latest git tag
- [x] implement syslog and flag mechanism
- [x] implement read/write config mechanism
- [x] define default config
- [x] implement dummy modbus server in node.js for testing
- [x] support ipv4/v6 ip address string
- [x] refactor int port to char * port
- [x] support docker compose
- [x] support valgrind (disable now)
- [x] implement set/get timeout command
- [x] support mocha and async test (runs slow on cloud server)
- [x] add versioneye support (depends check)
- [x] support armhf
- [x] deploy doxygen document automatically

## TODO List
- [ ] add [libmodbus test cases](https://github.com/stephane/libmodbus/tree/master/tests)
- [ ] enhance test cases
- [ ] enhance reconnect mechanism :clap:
- [ ] refine field name for psmb
- [ ] add unit test for byte order :clap:

# Backlog / Wish List
- [ ] set bits from byte (FC15) (only support big endian, i.e., 0x12345678)
    - length: octets
    - heximal or decimal?
- [ ] get bytes from bits (FC1, FC2) (only support big endian, i.e., 0x12345678)
    - length: octets
    - heximal or decimal?
- [ ] byte swap (FC3, FC4) (default: big endian, little endian) :thumbsup:
- [ ] read heximal/decimal (FC3, FC4) :thumbsup:
- [ ] write heximal/decimal (FC6, FC15, FC16) :thumbsdown:
- [ ] get 32-bit IEEE-754 float (FC3, FC4)
    - length: even number
        - ABCD [Big Endian]
        - BADC (byte swap) [Mid-Big Endian]
        - CDAB (word swap) [Mid-Little Endian]
        - DCBA (byte and word swap) [Little Endian]
- [ ] set 32-bit IEEE-754 float (FC16) :thumbsdown:
    - length: even number
- [ ] on changed filter (bit, bits, word, words; FC1~FC4) :clap:
- [ ] threshold (FC3, FC4) :question:
- [ ] scaling :scream:
- [ ] address mapping :fearful: