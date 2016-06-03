//
// main.c
// taka-wang
//

#include "modbusd.h"

int enable_syslog = 1;
extern cJSON * config_json;

int file_to_json(const char *fname, cJSON **outJson)
{
    FILE *fPtr = fopen(fname,"rb");
    if (fPtr) 
    {
        fseek(fPtr, 0, SEEK_END);
        long len = ftell(fPtr);
        fseek(fPtr, 0, SEEK_SET);
        char *data = (char*) malloc (len+1);
        int _ = fread(data, 1, len, fPtr); _=_;
        fclose(fPtr);
        *outJson = cJSON_Parse(data);
        free(data);
        return outJson ? 0 : 1;
    }
    else
    {
        return -1;
    }
}


// load configuration file
void load_config()
{
    BEGIN(enable_syslog);
    // TODO
}

// entry
int main(int argc, char *argv[])
{
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
    exit(EXIT_SUCCESS);
}