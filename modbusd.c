//
// taka-wang
//



#include "modbusd.h"

// hashtable
mbtcp_handle_t *mbtcp_htable = NULL;

// init mbtcp handle and try to connect
int init_mbtcp_handle (mbtcp_handle_t **ptr_handle, const char *ip, int port)
{
    printf("init_mbtcp_ctx\n");

    // create a mbtcp context
    modbus_t *ctx = modbus_new_tcp(ip, port);
    
    if (ctx == NULL)
    {
        fprintf(stderr, "Unable to allocate mbtcp context\n");
        return -1;
    }
    
    // call by reference to `mbtcp handle address`
    *ptr_handle = ctx;
    
    // set response timeout
    modbus_set_response_timeout(ctx, MBTCP_RESP_TIMEOUT_SEC, 0);
    
    // @add context to mbtcp hashtable
    mbtcp_handle_t *mb_handler;
    mb_handler = (mbtcp_handle_t*)malloc(sizeof(mbtcp_handle_t));
    // let alignment bytes being set to zero-value.
    memset(mb_handler, 0, sizeof(mb_handler));
    mb_handler->connected = false;
    mb_handler->key.ip   = ip;
    mb_handler->key.port = port;
    mb_handler->ctx = ctx;
    HASH_ADD(hh, mbtcp_htable, key, sizeof(mbtcp_key_t), mb_handler);
    printf("Add %s:%d to tcp hashtable\n", mb_handler->key.ip, mbtcp_htable->key.port);

    // @connect to slave
    if (modbus_connect(ctx) == -1) 
    {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
    }
    else
    {
        printf("%s:%d connected\n", mb_handler->key.ip, mbtcp_htable->key.port);
        mb_handler->connected = true;
    }
    return 0;
}

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
        return -1;
    }
    
}


