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

#include "modbusd.h"

// Marco
#define VERSION "0.0.1"
#define IPC_SUB "ipc:///tmp/to.modbus"
#define IPC_PUB "ipc:///tmp/from.modbus"
#define MBTCP_RESP_TIMEOUT_SEC 3
//#define DEFAULT_TCP_PORT 502

// hashtable
hash_mbtcp_t *mbtcp_htable = NULL;

int init_mbtcp_ctx(const char *ip, int port)
{
    printf("init_mbtcp_ctx\n");
    modbus_t *ctx;
    ctx = modbus_new_tcp(ip, port);
    if (ctx == NULL)
    {
        fprintf(stderr, "Unable to allocate mbtcp context\n");
        return -1;
    }
    
    // set response timeout
    modbus_set_response_timeout(ctx, MBTCP_RESP_TIMEOUT_SEC, 0);
    
    // add ctx to mbtcp hashtable
    hash_mbtcp_t *mb_handler;
    mb_handler = (hash_mbtcp_t*)malloc(sizeof(hash_mbtcp_t));
    memset(mb_handler, 0, sizeof(mb_handler));
    mb_handler->connected = false;
    mb_handler->key.ip   = ip;
    mb_handler->key.port = port;
    mb_handler->ctx = ctx;
    HASH_ADD(hh, mbtcp_htable, key, sizeof(key_mbtcp_t), mb_handler);
    //printf("Add %s to hashtable\n", mb_handler->key.ip);
    return 0;
/*
    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }
*/
}

void get_mbtcp_ctx()
{
    
}


// ENTRY
int main(int argc, char *argv[])
{
    int i = init_mbtcp_ctx("127.0.0.1", 1502);
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