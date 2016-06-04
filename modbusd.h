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
    char ip[16];
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
bool mbtcp_get_connection_status(mbtcp_handle_s **ptr_handle);

// connect to mbtcp client via handle
bool mbtcp_do_connect(mbtcp_handle_s **ptr_handle);

// init mbtcp handle (to hash) and try to connect
bool mbtcp_init_handle(mbtcp_handle_s **ptr_handle, char *ip, int port);

// list mbtcp hash table
void mbtcp_list_handles();

// get mbtcp handle from hashtable
bool mbtcp_get_handle(mbtcp_handle_s **ptr_handle, char *ip, int port);

#endif  // MODBUSD_H