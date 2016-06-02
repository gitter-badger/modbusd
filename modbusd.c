//
// modbusd.c
// taka-wang
//

#include "modbusd.h"

// hashtable header
mbtcp_handle_t *mbtcp_htable = NULL;
// tcp connection timeout in usec
uint32_t tcp_conn_timeout_usec = 200000;

// connect to mbtcp client via handle
int mbtcp_connect(mbtcp_handle_t **ptr_handle)
{
    printf("mbtcp_connect\n");
    
    if (*ptr_handle == NULL)
    {
        printf("NULL handle");
        return -1;
    }
    
    if (modbus_connect(*ptr_handle.ctx) == -1) 
    {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        return -2;
    }
    else
    {
        printf("%s:%d connected\n", *ptr_handle.key.ip, *ptr_handle.key.port);
        *ptr_handle.connected = true;
        return 0;
    }
}

// init mbtcp handle and try to connect
int init_mbtcp_handle (mbtcp_handle_t **ptr_handle, const char *ip, int port)
{
    printf("init_mbtcp_handle\n");

    // create a mbtcp context
    modbus_t *ctx = modbus_new_tcp(ip, port);
    
    if (ctx == NULL)
    {
        fprintf(stderr, "Unable to allocate mbtcp context\n");
        return -1; // fail to allocate context
    }
        
    // set tcp connection timeout
    modbus_set_response_timeout(ctx, 0, tcp_conn_timeout_usec);
    
    // @add context to mbtcp hashtable
    mbtcp_handle_t *mb_handler;
    mb_handler = (mbtcp_handle_t*)malloc(sizeof(mbtcp_handle_t));
    // let alignment bytes being set to zero-value!!
    memset(mb_handler, 0, sizeof(mbtcp_handle_t));
    mb_handler->connected = false;
    mb_handler->key.ip   = ip;
    mb_handler->key.port = port;
    mb_handler->ctx = ctx;

    HASH_ADD(hh, mbtcp_htable, key, sizeof(mbtcp_key_t), mb_handler);
    printf("Add %s:%d to tcp hashtable\n", mb_handler->key.ip, mbtcp_htable->key.port);

    // call by reference to `mbtcp handle address`
    *ptr_handle = mb_handler;

    // @connect to server without slave id
    int ret = mbtcp_connect(&mb_handler);
    return 0;
}

// get mbtcp handle from hashtable
int get_mbtcp_handle (mbtcp_handle_t **ptr_handle, const char *ip, int port)
{
    printf("get_mbtcp_handle\n");
    
    mbtcp_handle_t query, *hash_ctx;
    memset(&query, 0, sizeof(mbtcp_handle_t));
    query.key.ip = ip;
    query.key.port = port;
    HASH_FIND(hh, mbtcp_htable, &query.key, sizeof(mbtcp_key_t), hash_ctx);
    
    if (hash_ctx)
    {
        printf("found\n");
        printf("%s, %d\n", hash_ctx->key.ip, hash_ctx->key.port);
        // call by reference to `mbtcp handle address`
        *ptr_handle = hash_ctx; 
        return 0;
    }
    else
    {
        printf("not found\n");
        *ptr_handle = NULL; 
        return -1; // not found
    }
}
