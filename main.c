//
// main.c
// taka-wang
//

#include "modbusd.h"

    
void load_config()
{
    // TODO
}

// ENTRY
int main(int argc, char *argv[])
{
    openlog("modbusd", LOG_CONS | LOG_PID, 0);
  
    // @load external config
    load_config();

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
    printf("start command listener\n");
    syslog(LOG_INFO, "start command listener\n");

    while (!zctx_interrupted) // handle ctrl+c
    {
        zmsg_t *msg = zmsg_recv(zmq_sub);
        if (msg != NULL)
        {
            //zmsg_dump(msg);
        }
    }
    
    // @resource clean up
    syslog(LOG_INFO, "clean up\n");
    printf("clean up\n");
    zctx_destroy(&zmq_context);
    closelog();
    exit(EXIT_SUCCESS);
}