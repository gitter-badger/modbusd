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
#include <stdbool.h>

#include <czmq.h>
#include <modbus.h>
#include "uthash.h"
#include "log.h"
#include "json.h"

// marco
#define MBTCP_RESP_TIMEOUT_SEC 30

//==================================================
// structure
//==================================================

// `structure key` for modbus tcp hash table
typedef struct 
{
    char *ip;
    int port;
} mbtcp_key_s;

// mbtcp handle type
typedef struct 
{
    mbtcp_key_s key;    // key
    bool connected;     // is connect to modbus slave?
    modbus_t *ctx;      // modbus context pointer
    UT_hash_handle hh;  // makes this structure hashable
} mbtcp_handle_s;

//==================================================
// api
//==================================================

// get mbtcp handle's connection status
bool get_mbtcp_connection_status(mbtcp_handle_s **ptr_handle);

// connect to mbtcp client via handle
int mbtcp_connect(mbtcp_handle_s **ptr_handle);

// init mbtcp handle (to hash) and try to connect
int init_mbtcp_handle(mbtcp_handle_s **ptr_handle, char *ip, int port);

// get mbtcp handle from hashtable
int get_mbtcp_handle(mbtcp_handle_s **ptr_handle, char *ip, int port);

#endif  // MODBUSD_H