//
// taka-wang

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <errno.h>
//#include <signal.h>
#include <czmq.h>   // czmq
#include "uthash.h"


#define IPC_SUB "ipc:///tmp/to.modbus"
#define IPC_PUB "ipc:///tmp/from.modbus"

int main(int argc, char *argv[])
{
    zctx_t *zmq_context = zctx_new();
 
    void *zmq_sub = zsocket_new (zmq_context, ZMQ_SUB);
    zsocket_connect (zmq_sub, IPC_SUB);
    zsocket_set_subscribe (zmq_sub, ""); // set zmq sub filter    
    
    /* start receiving zmq command */
    printf("start command listener\n");
    while (!zctx_interrupted) // handle ctrl+c
    {
        zmsg_t *msg = zmsg_recv(zmq_sub);
    }
    
    printf("clean up\n");
    // clean up section
    zctx_destroy(&zmq_context);
    exit(0);
}