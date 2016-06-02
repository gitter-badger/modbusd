#ifndef MODBUSD_H
#define MODBUSD_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <czmq.h>

#include <stdbool.h>
#include <modbus.h> // libmodbus
#include "uthash.h" // uthash

// Marco
#define VERSION "0.0.1"
#define IPC_SUB "ipc:///tmp/to.modbus"
#define IPC_PUB "ipc:///tmp/from.modbus"
#define MBTCP_RESP_TIMEOUT_SEC 3
//#define DEFAULT_TCP_PORT 502


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