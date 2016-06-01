#ifndef MODBUSD_H
#define MODBUSD_H

#include <stdbool.h>
#include <modbus.h> // libmodbus
#include "uthash.h" // utHash

// hash key struct for modbus tcp
typedef struct {
    const char *ip;
    int port;
} tcp_port_key_t;

typedef struct {
    tcp_port_key_t key; // key
    bool connected;     // is connect to modbus slave?
    modbus_t *ctx;      // modbus context
    UT_hash_handle hh;  // makes this structure hashable
} tcp_hash_t;

#endif  // MODBUSD_H