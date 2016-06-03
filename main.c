//
// main.c
// taka-wang
//

#include "main.h"
#include "modbusd.h"

int enable_syslog = 1;
extern cJSON * config_json;

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
        zmsg_t *msg = zmsg_recv(zmq_sub); // recv zmsg
        if (msg != NULL)
        {
            LOG(enable_syslog, "recv msg:");
            zmsg_dump(msg);

            zframe_t *frame_mode = zmsg_pop(msg);
            zframe_t *frame_json = zmsg_pop(msg);
            char *mode = zframe_strdup(frame_mode);
            char *buf_json = zframe_strdup(frame_json);
            LOG(enable_syslog, "%s,%s\n", mode, buf_json);

            // parse json string
            cJSON *json = cJSON_Parse(buf_json);
            if (json != NULL)
            {
                char *cmd  = json_get_char(json, "cmd");
                
                if (mode == NULL)
                {
                    ERR(enable_syslog, "Wrong command format");
                }
                else if (strcmp(mode, "tcp") == 0)
                {
                    LOG(enable_syslog, "TCP:%s", cmd);
                } 
                else if (strcmp(mode, "rtu") == 0)
                {
                    LOG(enable_syslog, "rtu:%s", cmd);
                }
                else
                {
                    ERR(enable_syslog, "unsupport mode");
                }
                // release cJSON
                cJSON_Delete(json);
            }
            else
            {
                ERR(enable_syslog, "Fail to parse command");
            }
            
            // cleanup
            zframe_destroy(&frame_mode);
            zframe_destroy(&frame_json);
            zmsg_destroy(&msg);
        }
        else
        {
            ERR(enable_syslog, "Recv null message");
        }
    }
    
    // @resource clean up
    LOG(enable_syslog, "clean up");
    zctx_destroy(&zmq_context);
    exit(EXIT_SUCCESS);
}