//
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

void test_tcp_context_hashtable()
{
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
        printf("found\n");
        printf("%s, %d\n", p->key.ip, p->key.port);
    }
    else
    {
        printf("not found\n");
    }
    
    // find #2
    memset(&ff, 0, sizeof(mbtcp_handle_t));
    ff.key.ip = "192.168.10.2";
    ff.key.port = 556;
    HASH_FIND(hh, servers, &ff.key, sizeof(mbtcp_key_t), p);
    
    if (p)
    {
        printf("found\n");
        printf("%s, %d\n", p->key.ip, p->key.port);
    }
    else
    {
        printf("not found\n");
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
    test_tcp_context_hashtable();
    exit(EXIT_SUCCESS);
}