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
- should we support float for registers

- set bits from byte (FC15)
- get bytes from bits (FC1, FC2)
- get float (FC3, FC4)
    - ABCD
    - BADC
    - CDAB
    - DCBA
- set float (FC16)
- filter
    - on change (bit, bits, reg, regs)
    - equality
    - scaling
