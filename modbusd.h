#ifndef MODBUSD_H
#define MODBUSD_H

#include <stdbool.h>
#include <modbus.h> // libmodbus
#include "uthash.h" // uthash

// `structure key` for modbus tcp hash table
typedef struct {
    const char *ip;
    int port;
} key_mbtcp_t;

typedef struct {
    key_mbtcp_t key;    // key
    bool connected;     // is connect to modbus slave?
    modbus_t *ctx;      // modbus context pointer
    UT_hash_handle hh;  // makes this structure hashable
} hash_mbtcp_t;

#endif  // MODBUSD_H