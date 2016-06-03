//
// modbusd.h
// taka-wang
//

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
#include <modbus.h>
#include "uthash.h"
#include "log.h"
#include "json.h"

// marco
#define IPC_SUB "ipc:///tmp/to.modbus"
#define IPC_PUB "ipc:///tmp/from.modbus"
#define MBTCP_RESP_TIMEOUT_SEC 30

//==================================================
// structure
//==================================================

// `structure key` for modbus tcp hash table
typedef struct 
{
    const char *ip;
    int port;
} mbtcp_key_t;

// mbtcp handle type
typedef struct 
{
    mbtcp_key_t key;    // key
    bool connected;     // is connect to modbus slave?
    modbus_t *ctx;      // modbus context pointer
    UT_hash_handle hh;  // makes this structure hashable
} mbtcp_handle_t;

//==================================================
// api
//==================================================

// get mbtcp handle's connection status
bool get_mbtcp_connection_status(mbtcp_handle_t **ptr_handle);

// connect to mbtcp client via handle
int mbtcp_connect(mbtcp_handle_t **ptr_handle);

// init mbtcp handle (to hash) and try to connect
int init_mbtcp_handle(mbtcp_handle_t **ptr_handle, const char *ip, int port);

// get mbtcp handle from hashtable
int get_mbtcp_handle(mbtcp_handle_t **ptr_handle, const char *ip, int port);

#endif  // MODBUSD_H