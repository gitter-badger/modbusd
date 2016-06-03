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
    
    LOG(enable_syslog, "start recv command listener");
    while (!zctx_interrupted) // handle ctrl+c
    {
        zmsg_t *msg = zmsg_recv(zmq_sub); // recv zmsg
        if (msg != NULL)
        {
            // get request mode (ex. tcp, rtu, others)
            zframe_t *frame_mode = zmsg_pop(msg);
            char *mode = zframe_strdup(frame_mode);

            // get request json string
            zframe_t *frame_json = zmsg_pop(msg);
            char *req_json_string = zframe_strdup(frame_json);

            // cleanup zmsg releated resources
            zmsg_destroy(&msg);
            zframe_destroy(&frame_mode);
            zframe_destroy(&frame_json);
            
            LOG(enable_syslog, "recv msg: %s, %s\n", mode, req_json_string);

            // parse json string
            cJSON *req_json_obj = cJSON_Parse(req_json_string);
            
            if (req_json_obj != NULL)
            {
                char *cmd = json_get_char(req_obj, "cmd");
                
                // @handle modbus tcp requests
                if (strcmp(mode, "tcp") == 0)
                {
                    // @get common command parameters
                    char *cmd = json_get_char (req_json_obj, "cmd");
                    int tid   = json_get_int  (req_json_obj, "tid");
                    int port  = json_get_int  (req_json_obj, "port");
                    int slave = json_get_int  (req_json_obj, "slave");
                    int addr  = json_get_int  (req_json_obj, "addr");
                    int len   = json_get_int  (req_json_obj, "len");
                    
                    // c doesn't support string switch case,
                    // but if-else style should be okay for small set.
                    if (strcmp(cmd, "fc1") == 0)
                    {
                        LOG(enable_syslog, "FC1 trigger");
                        
                        // TODO: do request
                        // ....
                        // ....
                        
                        // create cJSON object for response
                        int mdata[4] = {116, 943, 234, 38793};
                        cJSON *root;
                        root = cJSON_CreateObject();
                        cJSON_AddNumberToObject(root, "tid", tid);
                        cJSON_AddItemToObject(root, "data", cJSON_CreateIntArray(mdata, 4));
                        cJSON_AddStringToObject(root, "status", "ok");
                        char * resp_json_string = cJSON_PrintUnformatted(root);
                        LOG(enable_syslog, "resp:%s", resp_json_string);
                        // clean up
                        cJSON_Delete(root);
                        
                        // @create zmsg for response
                        zmsg_t * zmq_resp = zmsg_new();
                        zmsg_addstr(zmq_resp, "tcp");     // frame 1
                        zmsg_addstr(zmq_resp, json_resp); // frame 2
                        zmsg_send(&zmq_resp, zmq_pub);    // send
                        zmsg_destroy(&zmq_resp);          // cleanup

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
                    
                    // cleanup (auto mode)
                    cJSON_Delete(req_json_obj);
                }
                // @handle modbus rtu requests
                else if (strcmp(mode, "rtu") == 0)
                {
                    LOG(enable_syslog, "rtu:%s", cmd);
                    // TODO
                }
                // @unkonw mode
                else
                {
                    ERR(enable_syslog, "unsupport mode");
                }
            }
            else
            {
                ERR(enable_syslog, "Fail to parse command string");
            }
        }
        else
        {
            // @depress this debug message
            //ERR(enable_syslog, "Recv null message");
        }
    }
    
    // @resource clean up
    LOG(enable_syslog, "clean up");
    zctx_destroy(&zmq_context);
    exit(EXIT_SUCCESS);
}