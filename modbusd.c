//
// modbusd.c
// taka-wang
//

#include "modbusd.h"

// syslog flag
extern int enable_syslog;
// hashtable header
static mbtcp_handle_s *mbtcp_htable = NULL;
// tcp connection timeout in usec
uint32_t tcp_conn_timeout_usec = 200000;

static void set_mbtcp_resp_error(char *reason)
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

static bool lazy_init_mbtcp_handle(mbtcp_handle_s **ptr_handle, cJSON *req)
{
    BEGIN(enable_syslog);
    
    char *ip = json_get_char (req, "ip");
    int port = json_get_int  (req, "port");
    
    if (mbtcp_get_handle (ptr_handle, ip, port))
	{
	   return true;
	}
	else
	{
        if (mbtcp_init_handle(ptr_handle, ip, port))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

static bool lazy_mbtcp_connect(mbtcp_handle_s *handle, cJSON *req)
{
    BEGIN(enable_syslog);
    
    int slave = json_get_int(req, "slave");
    if (mbtcp_get_connection_status(handle))
	{
        return true;
	}
	else
	{
		if (mbtcp_do_connect(handle))
		{
             return true;
		}
		else
		{
            return false;
		}
	}   
}

bool mbtcp_get_connection_status(mbtcp_handle_s *handle)
{
    BEGIN(enable_syslog);
    
    if ((handle) == NULL)
    {
        ERR(enable_syslog, "NULL handle");
        return false;
    }
    
    LOG(enable_syslog, "%s:%d connected: %s", handle->key.ip, 
                                              handle->key.port, 
                                              handle->connected ? "true" : "false");
    return handle->connected;
}

bool mbtcp_do_connect(mbtcp_handle_s *handle)
{
    BEGIN(enable_syslog);
    
    if (handle == NULL)
    {
        ERR(enable_syslog, "NULL handle");
        return false;
    }
    
    if (modbus_connect(handle->ctx) == -1) 
    {
        ERR(enable_syslog, "Connection failed: %s", modbus_strerror(errno));
        return false;
    }
    else
    {
        LOG(enable_syslog, "%s:%d connected", handle->key.ip, handle->key.port);
        // set connection status to true
        handle->connected = true;
        return true;
    }
}

void mbtcp_list_handles() 
{
    BEGIN(enable_syslog);
    mbtcp_handle_s * handle;

    for (handle = mbtcp_htable; handle != NULL; handle = handle->hh.next)
    {
        printf("ip:%s, port:%d\n", handle->key.ip, handle->key.port);
    }
}

bool mbtcp_init_handle(mbtcp_handle_s **ptr_handle, char *ip, int port)
{
    BEGIN(enable_syslog);

    // create a mbtcp context
    modbus_t *ctx = modbus_new_tcp(ip, port);
    
    if (ctx == NULL)
    {
        ERR(enable_syslog, "Unable to allocate mbtcp context");
        return false; // fail to allocate context
    }
        
    // set tcp connection timeout
    modbus_set_response_timeout(ctx, 0, tcp_conn_timeout_usec);
    
    // @add context to mbtcp hashtable
    mbtcp_handle_s *handle;
    handle = (mbtcp_handle_s*)malloc(sizeof(mbtcp_handle_s));
    // let alignment bytes being set to zero-value!!
    memset(handle, 0, sizeof(mbtcp_handle_s));
    handle->connected = false;
    strcpy(handle->key.ip, ip);
    handle->key.port  = port;
    handle->ctx = ctx;

    HASH_ADD(hh, mbtcp_htable, key, sizeof(mbtcp_key_s), handle);
    LOG(enable_syslog, "Add %s:%d to mbtcp hashtable", handle->key.ip, mbtcp_htable->key.port);

    // call by reference to `mbtcp handle address`
    *ptr_handle = handle;

    // @connect to server
    mbtcp_do_connect(handle);
    return true;
}

bool mbtcp_get_handle(mbtcp_handle_s **ptr_handle, char *ip, int port)
{
    BEGIN(enable_syslog);
    
    mbtcp_handle_s query, *hash_ctx;
    memset(&query, 0, sizeof(mbtcp_handle_s));
    strcpy(query.key.ip, ip);
    query.key.port = port;
    HASH_FIND(hh, mbtcp_htable, &query.key, sizeof(mbtcp_key_s), hash_ctx);
    
    if (hash_ctx != NULL)
    {
        LOG(enable_syslog, "tcp server %s:%d found", hash_ctx->key.ip, hash_ctx->key.port);
        // call by reference to `mbtcp handle address`
        *ptr_handle = hash_ctx; 
        return true;
    }
    else
    {
        ERR(enable_syslog, "tcp server %s:%d not found", query.key.ip, query.key.port);
        *ptr_handle = NULL; 
        return false; // not found
    }
}

void mbtcp_cmd_hanlder(cJSON *req, fp_mbtcp_fc fc)
{
    BEGIN(enable_syslog);
    mbtcp_handle_s *handle = NULL;
    
    if (lazy_init_mbtcp_handle(&handle, req))
    {
        if (lazy_mbtcp_connect(handle, req))
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

void mbtcp_fc1_req(mbtcp_handle_s *handle, cJSON *req)
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

void mbtcp_fc2_req(mbtcp_handle_s *handle, cJSON *req)
{
    BEGIN(enable_syslog);
    // TODO    
}
