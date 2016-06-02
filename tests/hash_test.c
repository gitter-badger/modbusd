//
// hash_test.c
// taka-wang
//

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <czmq.h>

#include "../modbusd.h"

int enable_syslog = 1;

int set_and_connect()
{
    mbtcp_handle_t *handle = NULL;
    int ret = init_mbtcp_handle (&handle, "172.16.9.170", 502);
    ret = mbtcp_connect(&handle);

    handle = NULL;
    ret = get_mbtcp_handle (&handle, "172.16.9.170", 502);
    
    ret = mbtcp_connect(&handle);
    ret = is_mbtcp_connected(&handle);
    return ret;
}

// add 1000 key-value pair to hashtable, then find these items
void multiple_add_find()
{
    BEGIN(enable_syslog);
    // important! initialize to NULL
    mbtcp_handle_t *servers = NULL;
    
    for (int idx = 0; idx < 1000; idx++)
    {
        mbtcp_handle_t *handle;
        handle = (mbtcp_handle_t*)malloc(sizeof(mbtcp_handle_t));
        memset(handle, 0, sizeof(mbtcp_handle_t));
        handle->connected = false;
        handle->key.ip   = "192.168.10.12";
        handle->key.port = idx;
        handle->ctx = modbus_new_tcp(handle->key.ip, handle->key.port);
        HASH_ADD(hh, servers, key, sizeof(mbtcp_key_t), handle);
        LOG(enable_syslog, "handle:%d, %p\n", idx, handle);
    }
    
    unsigned int num_users;
    num_users = HASH_COUNT(servers);
    LOG(enable_syslog, "there are %u users\n", num_users);
    LOG(enable_syslog,"==========================\n");
    
    for (int idx = 0; idx < 1000; idx++)
    {
        mbtcp_handle_t query, *ptr;
        memset(&query, 0, sizeof(mbtcp_handle_t));
        query.key.ip = "192.168.10.12";
        query.key.port = idx;
        HASH_FIND(hh, servers, &query.key, sizeof(mbtcp_key_t), ptr);
        
        if (ptr)
        {
            LOG(enable_syslog, "found: %d, %s, %d, %p\n", idx, ptr->key.ip, ptr->key.port, ptr);
        }
        else
        {
            LOG(enable_syslog, "not found: %d\n", idx);
        }
    }
}


void single_add_find()
{
    BEGIN(enable_syslog);
    
    // important! initialize to NULL
    mbtcp_handle_t *servers = NULL;
    mbtcp_handle_t ff, *p, *h1, *h2, *tmp;
    
    // server #1
    h1 = (mbtcp_handle_t*)malloc(sizeof(mbtcp_handle_t));
    // let alignment bytes being set to zero-value.
    // ref: https://troydhanson.github.io/uthash/userguide.html#_structure_keys
    memset(h1, 0, sizeof(h1));
    h1->connected = false;
    h1->key.ip   = "192.168.10.1";
    h1->key.port = 555;
    h1->ctx = modbus_new_tcp(h1->key.ip, h1->key.port);
    HASH_ADD(hh, servers, key, sizeof(mbtcp_key_t), h1);
    
    // server #2
    h2 = (mbtcp_handle_t*)malloc(sizeof(mbtcp_handle_t));
    memset(h2, 0, sizeof(h2));
    h2->connected = false;
    h2->key.ip   = "192.168.10.2";
    h2->key.port = 556;
    h2->ctx = modbus_new_tcp(h2->key.ip, h2->key.port);
    HASH_ADD(hh, servers, key, sizeof(mbtcp_key_t), h2);
    
    // find #1
    memset(&ff, 0, sizeof(mbtcp_handle_t));
    ff.key.ip = "192.168.10.1";
    ff.key.port = 555;
    HASH_FIND(hh, servers, &ff.key, sizeof(mbtcp_key_t), p);
    
    if (p)
    {
        LOG(enable_syslog, "Found %s, %d\n", p->key.ip, p->key.port);
    }
    else
    {
        LOG(enable_syslog, "Not found\n");
    }
    
    // find #2
    memset(&ff, 0, sizeof(mbtcp_handle_t));
    ff.key.ip = "192.168.10.2";
    ff.key.port = 556;
    HASH_FIND(hh, servers, &ff.key, sizeof(mbtcp_key_t), p);
    
    if (p)
    {
        LOG(enable_syslog, "Found %s, %d\n", p->key.ip, p->key.port);
    }
    else
    {
        LOG(enable_syslog, "Not found\n");
    }
    
    // delete
    HASH_ITER(hh, servers, p, tmp)
    {
        HASH_DEL(servers, p); // remove from hash
        modbus_free(p->ctx);  // release modbus context
        free(p);
    }
}

// ENTRY
int main(int argc, char *argv[])
{
    BEGIN(enable_syslog);
    set_and_connect();
    //multiple_add_find();
    //single_add_find();
    exit(EXIT_SUCCESS);
}