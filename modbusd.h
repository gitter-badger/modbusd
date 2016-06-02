#ifndef MODBUSD_H
#define MODBUSD_H

#include <stdbool.h>
#include <modbus.h> // libmodbus
#include "uthash.h" // uthash

// `structure key` for modbus tcp hash table
typedef struct {
    const char *ip;
    int port;
} mbtcp_key_t;

typedef struct {
    mbtcp_key_t key;    // key
    bool connected;     // is connect to modbus slave?
    modbus_t *ctx;      // modbus context pointer
    UT_hash_handle hh;  // makes this structure hashable
} mbtcp_handle_t;

// init modbus tcp context and try to connect
int init_mbtcp_ctx (modbus_t **ptr_ctx, const char *ip, int port);

int get_mbtcp_handle (mbtcp_handle_t **ptr_handle, const char *ip, int port);

#endif  // MODBUSD_H