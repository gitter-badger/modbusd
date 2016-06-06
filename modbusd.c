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

// generic mbtcp error response handler
static char * set_mbtcp_resp_error(int tid, char *reason)
{
    BEGIN(enable_syslog);
    // @create cJSON object for response
    cJSON *resp_root;
    resp_root = cJSON_CreateObject();
    cJSON_AddNumberToObject(resp_root, "tid", tid);
    cJSON_AddStringToObject(resp_root, "status", reason);
    char * resp_json_string = cJSON_PrintUnformatted(resp_root);
    LOG(enable_syslog, "resp:%s", resp_json_string);
    // clean up
    cJSON_Delete(resp_root);
    return resp_json_string;
}

// combo func: get or init mbtcp handle
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

// combo func: check connection status,
// if not connected, try to connect to slave
static bool lazy_mbtcp_connect(mbtcp_handle_s *handle, cJSON *req)
{
    BEGIN(enable_syslog);
        
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

char * mbtcp_cmd_hanlder(cJSON *req, mbtcp_fc fc)
{
    BEGIN(enable_syslog);
    mbtcp_handle_s *handle = NULL;
    int tid  = json_get_int(req, "tid");
    if (lazy_init_mbtcp_handle(&handle, req))
    {
        if (lazy_mbtcp_connect(handle, req))
        {
            // set slave id
            int slave = json_get_int(req, "slave");
            LOG(enable_syslog, "slave id: %d", slave);
            modbus_set_slave(handle->ctx, slave);
		    return fc(handle, req);
        }
        else
        {
            // [enhance]: get reason from modbus response
			return set_mbtcp_resp_error(tid, "fail to connect");
        }
    }
    else
    {
        return set_mbtcp_resp_error(tid, "init modbus tcp handle fail");
    }
}

char * mbtcp_fc1_req(mbtcp_handle_s *handle, cJSON *req)
{
    BEGIN(enable_syslog);
    int addr = json_get_int(req, "addr");
    int len  = json_get_int(req, "len");
    int tid  = json_get_int(req, "tid");
    if (len > MODBUS_MAX_READ_BITS) // 2000
    {
        return set_mbtcp_resp_error(tid, "register lenth too long");
    }
    else
    {
        uint8_t bits[len];
        memset(bits, 0, len * sizeof(uint8_t));
    
        int ret = modbus_read_bits(handle->ctx, addr, len, bits);
        if (ret < 0) 
        {
            ERR(enable_syslog, "%s", modbus_strerror(errno));
            return set_mbtcp_resp_error(tid, modbus_strerror(errno));
        } 
        else 
        {
            LOG(enable_syslog, "desired length: %d, read length:%d", len, ret);
            for (int ii = 0; ii < ret; ii++) 
            {
                LOG(enable_syslog, "[%d]=%d", ii, bits[ii]);
            }

            // @create cJSON object for response
            cJSON *resp_root;
            resp_root = cJSON_CreateObject();
            cJSON_AddNumberToObject(resp_root, "tid", tid);
            cJSON_AddItemToObject(resp_root, "data", cJSON_CreateUInt8Array(bits, len));
            cJSON_AddStringToObject(resp_root, "status", "ok");
            char * resp_json_string = cJSON_PrintUnformatted(resp_root);
            LOG(enable_syslog, "resp:%s", resp_json_string);
            // clean up
            cJSON_Delete(resp_root);
            return resp_json_string;
        }
    }
}

char * mbtcp_fc2_req(mbtcp_handle_s *handle, cJSON *req)
{
    BEGIN(enable_syslog);
    // TODO    
}
