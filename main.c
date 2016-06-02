//
// main.c
// taka-wang
//

#include "modbusd.h"

int enable_syslog = 1;
    
void load_config()
{
    // TODO
}

// ENTRY
int main(int argc, char *argv[])
{
    // @load external config
    load_config();
    
    mbtcp_handle_t *handle = NULL;
    int ret = init_mbtcp_handle (&handle, "172.16.9.170", 502);
    ret = mbtcp_connect(&handle);
    
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
    
    // @start receiving zmq command
    LOG(enable_syslog, "start command listener");

    while (!zctx_interrupted) // handle ctrl+c
    {
        zmsg_t *msg = zmsg_recv(zmq_sub);
        if (msg != NULL)
        {
            //zmsg_dump(msg);
        }
    }
    
    // @resource clean up
    LOG(enable_syslog, "clean up");
    zctx_destroy(&zmq_context);
    closelog();
    exit(EXIT_SUCCESS);
}