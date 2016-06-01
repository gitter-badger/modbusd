//
// taka-wang
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <czmq.h>
#include <modbus.h> // libmodbus
#include "uthash.h" // utHash
#include "modbusd.h"

// Marco
#define VERSION "0.0.1"
#define IPC_SUB "ipc:///tmp/to.modbus"
#define IPC_PUB "ipc:///tmp/from.modbus"
#define DEFAULT_TCP_PORT 502


// hash key struct for modbus tcp
typedef struct {
    const char *ip;
    int port;
} tcp_port_key_t;

typedef struct {
    tcp_port_key_t key; // key
    bool connected; 
    modbus_t *ctx;  // modbus context
    UT_hash_handle hh; // makes this structure hashable
} tcp_hash_t;


void test_hash()
{
    tcp_hash_t ff, *p, *h1, *tmp, *servers = NULL;
    h1 = (tcp_hash_t*)malloc(sizeof(tcp_hash_t));
    memset(h1, 0, sizeof(h1));
    h1.connected = false;
    h1->key.ip   = "192.168.10.1";
    h1->key.port = 555;
    h1.ctx = modbus_new_tcp(h1->key.ip, h1->key.port);
    HASH_ADD(hh, servers, key, sizeof(tcp_port_key_t), h1);
    
    memset(&ff, 0, sizeof(tcp_hash_t));
    ff.key.ip = "hello";
    ff.key.port = 1234;
    HASH_FIND(hh, servers, &ff.key, sizeof(tcp_port_key_t), p);
    if (p)
    {
        printf("found\n");
        printf("%s, %d\n", p->key.ip, p->key.port);
    }
    else
    {
        printf("not found\n");
    }
    
    HASH_ITER(hh, servers, p, tmp)
    {
        HASH_DEL(servers, p);
        free(p);
    }    
}

// ENTRY
int main(int argc, char *argv[])
{
    test_hash();
    
    // @load external config
    // TODO
    
    
    // @setup zmq
    zctx_t *zmq_context = zctx_new ();
    // init zmq subscriber: zmq_sub
    void *zmq_sub = zsocket_new (zmq_context, ZMQ_SUB);
    // bind zmq subscriber
    zsocket_bind (zmq_sub, IPC_SUB);
    // set zmq subscriber filter
    zsocket_set_subscribe (zmq_sub, ""); 
    // init zmq publisher: zmq_pub
    void *zmq_pub = zsocket_new (zmq_context, ZMQ_PUB);
    // bind zmq publisher
    zsocket_bind (zmq_pub, IPC_PUB);
    
    // try
    modbus_t *ctx = NULL;
    
    // @start receiving zmq command
    printf("start command listener\n");
    while (!zctx_interrupted) // handle ctrl+c
    {
        zmsg_t *msg = zmsg_recv(zmq_sub);
        if (msg != NULL)
        {
            //zmsg_dump(msg);
        }
    }
    
    // @resource clean up
    printf("clean up\n");
    zctx_destroy(&zmq_context);
    exit(EXIT_SUCCESS);
}