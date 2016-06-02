#include "modbusd.h"


// ENTRY
int main(int argc, char *argv[])
{
    mbtcp_handle_t *handle = NULL;
    int i = init_mbtcp_ctx(&handle, "192.168.1.234", 1502);
    if (modbus_connect(handle->ctx) == -1) 
    {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        //modbus_free(ctx);
        return -1;
    }
    
    handle = NULL;
    i = get_mbtcp_handle (&handle, "192.168.1.234", 1502);
    if (modbus_connect(handle->ctx) == -1) 
    {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        //modbus_free(ctx);
        return -1;
    }
    
    
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
    //modbus_t *ctx = NULL;
    
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