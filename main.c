/**
 * @file main.c
 * @author taka-wang
*/

#include "main.h"
#include "modbusd.h"

int enable_syslog = 1; // set log to syslog, should load from external
extern cJSON * config_json;

/**
 * @brief Load configuration file
 *
 * @param fname Configuration file name string.
 * @return Void.
 */
static void load_config(const char *fname)
{
    BEGIN(enable_syslog);
    // TODO
}

/**
 * @brief Generic zmq response sender for modbus
 *
 * @param pub Zmq publisher.
 * @param mode Request mode. ex. tcp, rtu.
 * @param json_resp Response string in JSON format.
 * @return Void.
 */
static void send_modbus_zmq_resp(void * pub, char *mode, char *json_resp)
{
    BEGIN(enable_syslog);
    
    if (pub != NULL)
    {
        zmsg_t * zmq_resp = zmsg_new();
        zmsg_addstr(zmq_resp, mode);        // frame 1: mode
        zmsg_addstr(zmq_resp, json_resp);   // frame 2: resp
        zmsg_send(&zmq_resp, pub);          // send zmq msg
        // cleanup zmsg
        zmsg_destroy(&zmq_resp);
    }
    else
    {
        ERR(enable_syslog, "NULL publisher");
    }
}

// entry
int main()
{
    LOG(enable_syslog, "modbusd version: %s", VERSION);
    
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
    
    LOG(enable_syslog, "start request listener");
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
            int tid = json_get_int(req_json_obj, "tid");
            
            if (req_json_obj != NULL)
            {
                char *cmd = json_get_char(req_json_obj, "cmd");
                
                // @handle modbus tcp requests
                if (strcmp(mode, "tcp") == 0)
                {
                    // c doesn't support string switch case,
                    // but if-else style should be okay for small set.
                    if (strcmp(cmd, "fc1") == 0)
                    {
                        LOG(enable_syslog, "send fc1 req");
                        send_modbus_zmq_resp(zmq_pub, mode, mbtcp_cmd_hanlder(req_json_obj, mbtcp_fc1_req));
                    }
                    else if (strcmp(cmd, "fc2") == 0)
                    {
                        LOG(enable_syslog, "send fc2 req");
                        send_modbus_zmq_resp(zmq_pub, mode, mbtcp_cmd_hanlder(req_json_obj, mbtcp_fc2_req));
                    }
                    else if (strcmp(cmd, "fc3") == 0)
                    {
                        LOG(enable_syslog, "send fc3 req");
                        send_modbus_zmq_resp(zmq_pub, mode, mbtcp_cmd_hanlder(req_json_obj, mbtcp_fc3_req));
                    }
                    else if (strcmp(cmd, "fc4") == 0)
                    {
                        LOG(enable_syslog, "send fc4 req");
                        send_modbus_zmq_resp(zmq_pub, mode, mbtcp_cmd_hanlder(req_json_obj, mbtcp_fc4_req));
                    }
                    else if (strcmp(cmd, "fc5") == 0)
                    {
                        LOG(enable_syslog, "send fc5 req");
                    }
                    else if (strcmp(cmd, "fc6") == 0)
                    {
                        LOG(enable_syslog, "send fc6 req");
                    }
                    else if (strcmp(cmd, "fc15") == 0)
                    {
                        LOG(enable_syslog, "send fc15 req");
                    }
                    else if (strcmp(cmd, "fc16") == 0)
                    {
                        LOG(enable_syslog, "send fc16 req");
                    }
                    else
                    {
                        LOG(enable_syslog, "unsupport request");
                        send_modbus_zmq_resp(zmq_pub, mode, set_modbus_error_resp(tid, "unsupport request"));
                    }
                }
                // @handle modbus rtu requests
                else if (strcmp(mode, "rtu") == 0)
                {
                    LOG(enable_syslog, "rtu:%s", cmd);
                    // [TODO]
                    // send error response
                }
                // @unkonw mode
                else
                {
                    ERR(enable_syslog, "unsupport mode");
                    send_modbus_zmq_resp(zmq_pub, mode, set_modbus_error_resp(tid, "unsupport mode"));
                }
            }
            else
            {
                ERR(enable_syslog, "Fail to parse command string");
                send_modbus_zmq_resp(zmq_pub, mode, set_modbus_error_resp(tid, "Fail to parse command string"));
            }
            
            // @cleanup cJson object (auto mode)
            cJSON_Delete(req_json_obj);
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
}