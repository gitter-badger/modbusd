//
// main.c
// taka-wang
//

#include "main.h"
#include "modbusd.h"

int enable_syslog = 1;
extern cJSON * config_json;


// load configuration file
static void load_config()
{
    BEGIN(enable_syslog);
    // TODO
}


// generic mbtcp error response handler
static void set_mbtcp_resp_error(char * reason)
{
    BEGIN(enable_syslog);
    // TODO
    LOG(enable_syslog, "%s", reason); // debug
    
    /*
    // @create zmsg for response
    zmsg_t * zmq_resp = zmsg_new();
    zmsg_addstr(zmq_resp, "tcp");            // frame 1: mode
    zmsg_addstr(zmq_resp, resp_json_string); // frame 2: resp
    zmsg_send(&zmq_resp, zmq_pub);           // send zmq msg
    zmsg_destroy(&zmq_resp);                 // cleanup
    */
}

// do modbus tcp requests
static void mbtcp_fc1_req(mbtcp_handle_s *ptr_handle, cJSON *ptr_req)
{
    BEGIN(enable_syslog);
    // TODO    
        /*
    if (ok)
    {
        // ok, send response
    }
    else
    {
        // fail, send response
    }
    */
}

// do modbus tcp requests
static void mbtcp_fc2_req(mbtcp_handle_s *ptr_handle, cJSON *ptr_req)
{
    BEGIN(enable_syslog);
    // TODO    
}

// combo func: check connection status,
// if not connected, try to connect to slave
static bool lazy_mbtcp_connect(mbtcp_handle_s *ptr_handle, cJSON *ptr_req, fp_mbtcp_fc fc)
{
    BEGIN(enable_syslog);
    
    int slave = json_get_int(ptr_req, "slave");
    if (mbtcp_get_connection_status(ptr_handle))
	{
        return true;
	}
	else
	{
		if (mbtcp_do_connect(ptr_handle))
		{
             return true;
		}
		else
		{
            return false;
		}
	}   
}

// combo func: get or init mbtcp handle
static bool lazy_init_mbtcp_handle(mbtcp_handle_s **handle, cJSON *req, fp_mbtcp_fc fc)
{
    BEGIN(enable_syslog);
    
    char *ip = json_get_char(req, "ip");
    int port = json_get_int (req, "port");
    
    if (mbtcp_get_handle (&handle, ip, port))
	{
	   return true;
	}
	else
	{
        if (mbtcp_init_handle(&handle, ip, port))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

// generic mbtcp command handler
void mbtcp_cmd_hanlder(cJSON *req, fp_mbtcp_fc fc)
{
    BEGIN(enable_syslog);
    mbtcp_handle_s *handle = NULL;
    
    // check handle
    if (lazy_init_mbtcp_handle(&handle, req, fc))
    {
        // check connection
        if (lazy_mbtcp_connect(handle, req, fc))
        {
            // todo: set slave id
		    fc(handle, req);
        }
        else
        {
            // [enhance]: get reason from modbus response
			set_mbtcp_resp_error("fail to connect");
        }
    }
    else
    {
        set_mbtcp_resp_error("init modbus tcp handle fail");
    }
    END(enable_syslog);
}

// entry
int main()
{
    LOG(enable_syslog, "modbusd version: %s", VERSION);
    
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
            
            if (req_json_obj != NULL)
            {
                char *cmd = json_get_char(req_json_obj, "cmd");
                
                // @handle modbus tcp requests
                if (strcmp(mode, "tcp") == 0)
                {
                    // @get common command parameters
                    char *ip  = json_get_char (req_json_obj, "ip");
                    int port  = json_get_int  (req_json_obj, "port");

                    int tid   = json_get_int  (req_json_obj, "tid");
                    int slave = json_get_int  (req_json_obj, "slave");
                    int addr  = json_get_int  (req_json_obj, "addr");
                    int len   = json_get_int  (req_json_obj, "len");
                    
                    
                    // c doesn't support string switch case,
                    // but if-else style should be okay for small set.
                    if (strcmp(cmd, "fc1") == 0)
                    {
                        LOG(enable_syslog, "FC1 trigger");
                        
                        // @do request
                        mbtcp_cmd_hanlder(req_json_obj, mbtcp_fc1_req);


                        // @create cJSON object for response
                        int mdata[4] = {116, 943, 234, 38793};
                        cJSON *resp_root;
                        resp_root = cJSON_CreateObject();
                        cJSON_AddNumberToObject(resp_root, "tid", tid);
                        cJSON_AddItemToObject(resp_root, "data", cJSON_CreateIntArray(mdata, 4));
                        cJSON_AddStringToObject(resp_root, "status", "ok");
                        char * resp_json_string = cJSON_PrintUnformatted(resp_root);
                        LOG(enable_syslog, "resp:%s", resp_json_string);
                        // clean up
                        cJSON_Delete(resp_root);
                        
                        // @create zmsg for response
                        zmsg_t * zmq_resp = zmsg_new();
                        zmsg_addstr(zmq_resp, "tcp");            // frame 1: mode
                        zmsg_addstr(zmq_resp, resp_json_string); // frame 2: resp
                        zmsg_send(&zmq_resp, zmq_pub);           // send zmq msg
                        zmsg_destroy(&zmq_resp);                 // cleanup

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
                        // send error response
                    }
                }
                // @handle modbus rtu requests
                else if (strcmp(mode, "rtu") == 0)
                {
                    LOG(enable_syslog, "rtu:%s", cmd);
                    // TODO
                    // send error response
                }
                // @unkonw mode
                else
                {
                    ERR(enable_syslog, "unsupport mode");
                    // send error response
                }
                
                // @cleanup (auto mode)
                cJSON_Delete(req_json_obj);
            }
            else
            {
                ERR(enable_syslog, "Fail to parse command string");
                // maybe send error response
                // 
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
}