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

            // get mode (ex. tcp, rtu, others)
            zframe_t *frame_mode = zmsg_pop(msg);
            char *mode = zframe_strdup(frame_mode);
            
            // get command json string
            zframe_t *frame_json = zmsg_pop(msg);
            char *json_string = zframe_strdup(frame_json);
            
            LOG(enable_syslog, "recv msg: %s, %s\n", mode, json_string);

            // parse json string
            cJSON *json = cJSON_Parse(json_string);
            
            if (json != NULL)
            {
                char *cmd = json_get_char(json, "cmd");
                if (strcmp(mode, "tcp") == 0)
                {
                    LOG(enable_syslog, "TCP:%s", cmd);
                    
                    // c doesn't support string switch case,
                    // but if-else style should be okay for small set.
                    if (strcmp(cmd, "fc1") == 0)
                    {
                        LOG(enable_syslog, "FC1 trigger");
                    }
                    else if (strcmp(cmd, "fc2") == 0)
                    {
                        LOG(enable_syslog, "FC2 trigger");
                    }
                    else if (strcmp(cmd, "fc3") == 0)
                    {
                        LOG(enable_syslog, "FC3 trigger");
                    }
                    else if (strcmp(cmd, "fc4") == 0)
                    {
                        LOG(enable_syslog, "FC4 trigger");
                    }
                    else if (strcmp(cmd, "fc5") == 0)
                    {
                        LOG(enable_syslog, "FC5 trigger");
                    }
                    else if (strcmp(cmd, "fc6") == 0)
                    {
                        LOG(enable_syslog, "FC6 trigger");
                    }
                    else if (strcmp(cmd, "fc15") == 0)
                    {
                        LOG(enable_syslog, "FC15 trigger");
                    }
                    else if (strcmp(cmd, "fc16") == 0)
                    {
                        LOG(enable_syslog, "FC16 trigger");
                    }
                    else
                    {
                        LOG(enable_syslog, "unsupport command");
                    }
                } 
                else if (strcmp(mode, "rtu") == 0)
                {
                    LOG(enable_syslog, "rtu:%s", cmd);
                    // TODO
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
                ERR(enable_syslog, "Fail to parse command string");
            }
            
            // cleanup
            zframe_destroy(&frame_mode);
            zframe_destroy(&frame_json);
            zmsg_destroy(&msg);
        }
        else
        {
            // depress this debug message
            //ERR(enable_syslog, "Recv null message");
        }
    }
    
    // @resource clean up
    LOG(enable_syslog, "clean up");
    zctx_destroy(&zmq_context);
    exit(EXIT_SUCCESS);
}