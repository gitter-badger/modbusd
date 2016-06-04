//
// modbusd.c
// taka-wang
//

#include "modbusd.h"

// syslog flag
extern int enable_syslog;
// hashtable header
static mbtcp_handle_s *mbtcp_htable = NULL;
// tcp connection timeout in usec
uint32_t tcp_conn_timeout_usec = 200000;

// check mbtcp handle is connected or not
bool get_mbtcp_connection_status(mbtcp_handle_s **ptr_handle)
{
    BEGIN(enable_syslog);
    
    if ((*ptr_handle) == NULL)
    {
        ERR(enable_syslog, "NULL handle");
        return false;
    }
    
    LOG(enable_syslog, "%s:%d connected? %s", (*ptr_handle)->key.ip, 
                                              (*ptr_handle)->key.port, 
                                              (*ptr_handle)->connected ? "true" : "false");
    return (*ptr_handle)->connected;
}

// connect to mbtcp client via handle
int mbtcp_connect(mbtcp_handle_s **ptr_handle)
{
    BEGIN(enable_syslog);
    
    if ((*ptr_handle) == NULL)
    {
        ERR(enable_syslog, "NULL handle");
        return -1;
    }
    
    if (modbus_connect((*ptr_handle)->ctx) == -1) 
    {
        ERR(enable_syslog, "Connection failed: %s", modbus_strerror(errno));
        return -2;
    }
    else
    {
        LOG(enable_syslog, "%s:%d connected", (*ptr_handle)->key.ip, (*ptr_handle)->key.port);
        (*ptr_handle)->connected = true;
        return 0;
    }
}

// init mbtcp handle and try to connect
int init_mbtcp_handle(mbtcp_handle_s **ptr_handle, char *ip, int port)
{
    BEGIN(enable_syslog);

    // create a mbtcp context
    modbus_t *ctx = modbus_new_tcp(ip, port);
    
    if (ctx == NULL)
    {
        ERR(enable_syslog, "Unable to allocate mbtcp context");
        return -1; // fail to allocate context
    }
        
    // set tcp connection timeout
    modbus_set_response_timeout(ctx, 0, tcp_conn_timeout_usec);
    
    // @add context to mbtcp hashtable
    mbtcp_handle_s *handle;
    handle = (mbtcp_handle_s*)malloc(sizeof(mbtcp_handle_s));
    // let alignment bytes being set to zero-value!!
    memset(handle, 0, sizeof(mbtcp_handle_s));
    handle->connected = false;
    strcpy(handle->key.ip, ip);
    handle->key.port  = port;
    handle->ctx = ctx;

    HASH_ADD(hh, mbtcp_htable, key, sizeof(mbtcp_key_s), handle);
    LOG(enable_syslog, "Add %s:%d to mbtcp hashtable", handle->key.ip, mbtcp_htable->key.port);

    // call by reference to `mbtcp handle address`
    *ptr_handle = handle;

    // @connect to server without slave id
    int ret = mbtcp_connect(&handle);
    return 0;
}

// list mbtcp hash table
void list_mbtcp_handle() 
{
    BEGIN(enable_syslog);
    mbtcp_handle_s *s;

    for (s = mbtcp_htable; s != NULL; s=s->hh.next)
    {
        printf("ip:%s, port:%d\n", (s->key).ip, s->key.port);
    }
}

// get mbtcp handle from hashtable
int get_mbtcp_handle(mbtcp_handle_s **ptr_handle, char *ip, int port)
{
    BEGIN(enable_syslog);
    
    mbtcp_handle_s query, *hash_ctx;
    memset(&query, 0, sizeof(mbtcp_handle_s));
    strcpy(query.key.ip, ip);
    query.key.port = port;
    HASH_FIND(hh, mbtcp_htable, &query.key, sizeof(mbtcp_key_s), hash_ctx);
    
    if (hash_ctx)
    {
        LOG(enable_syslog, "tcp server %s:%d found", hash_ctx->key.ip, hash_ctx->key.port);
        // call by reference to `mbtcp handle address`
        *ptr_handle = hash_ctx; 
        return 0;
    }
    else
    {
        ERR(enable_syslog, "tcp server %s:%d not found", query.key.ip, query.key.port);
        *ptr_handle = NULL; 
        return -1; // not found
    }
}