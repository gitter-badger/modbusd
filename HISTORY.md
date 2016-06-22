# v0.2.1

## Fix
- fix uthash sizeof issue (1000 items)
- fix char pointer key issue (hash)
- modbus_connect hang issue (timeout)
- handle 'reset by peer' issue 

---

## Done
- implement keep connection mechanism 
- implement FC (1~6, 15, 16)
- daemon version number
- syslog (flag)
- implement read config
- implement default config
- implement dummy modbus server for testing
- support ipv4/v6 string
- refactor int port to char * port
- support docker compose
- support valgrind
- implement change timeout command
- support mocha and async test
- add versioneye support
- support armv7
- deploy doxygen document automatically

## TODO List
- enhance test cases
- enhance reconnect mechanism

# Backlog/Wish List
- [] set bits from byte (FC15) (only support big endian, i.e., 0x12345678)
    - length: 8x
- [] get bytes from bits (FC1, FC2) (only support big endian, i.e., 0x12345678)
    - length: 8x
- [] byte swap (FC3, FC4) (default: big endian, little endian)
- [] read heximal/decimal (FC3, FC4)
- [] write heximal/decimal (FC6, FC15, FC16)
- [] get 32-bit float (FC3, FC4)
    - length: 2x
        - ABCD
        - BADC
        - CDAB
        - DCBA
- [] set 32-bit float (FC16)
    - length: 2x
- [] on changed filter (bit, bits, word, words; FC1~FC4)
- [] threshold (FC3, FC4)
- [] scaling