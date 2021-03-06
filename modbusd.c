/**
 * @file modbusd.c
 * @author taka-wang
 * @brief modbus daemon exported api
*/

#include "modbusd.h"

/* ==================================================
 *  global variable
================================================== */

extern int enable_syslog;                   // syslog flag
static mbtcp_handle_s *mbtcp_htable = NULL; // hashtable header
uint32_t tcp_conn_timeout_usec = 200000;    /* tcp connection
                                               timeout in usec */

/* ==================================================
 *  static functions
================================================== */

/**
 * @brief Combo function: get or init mbtcp handle.
 *
 * @param ptr_handle Pointer to mbtcp handle.
 * @param req cJSON request object.
 * @return Success or not.
 */
static bool lazy_init_mbtcp_handle(mbtcp_handle_s **ptr_handle, cJSON *req)
{
    BEGIN(enable_syslog);
    
    char *ip   = json_get_char (req, "ip");
    char *port = json_get_char (req, "port");
    
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
            // Unable to allocate mbtcp context,
            // maybe system resourse issue!
            return false;
        }
    }
}

/**
 * @brief Combo function: check mbtcp connection status, 
 *        if not connected, try to connect to slave.
 *
 * @param handle Mbtcp handle.
 * @param reason Pointer to fail reason string.
 * @return Success or not.
 */
static bool lazy_mbtcp_connect(mbtcp_handle_s *handle, char **reason)
{
    BEGIN(enable_syslog);
        
    if (mbtcp_get_connection_status(handle))
    {
        return true;
    }
    else
    {
        if (mbtcp_do_connect(handle, reason))
        {
             return true;
        }
        else
        {
            // could get fail reason from '*reason'
            return false;
        }
    }   
}


/**
 * @brief Generic mbtcp error number handler
 *
 * @param tid Transaction ID.
 * @param handle Mbtcp handle.
 * @param errnum Error number from modbus tcp handle.
 * @return Modbus error response string in JSON format.
 */  
static char * set_modbus_errno_resp(int tid, mbtcp_handle_s *handle, int errnum)
{
    BEGIN(enable_syslog);
    // [todo][enhance] reconnect proactively?
    // ... if the request interval is very large, 
    // we should try to reconnect automatically
    
    if (errnum == 104) // Connection reset by peer (i.e, tcp connection timeout)
    {
        handle->connected = false;
    }
    ERR(enable_syslog, "%s:%d", modbus_strerror(errnum), errnum);
    return modbus_set_error_resp(tid, modbus_strerror(errnum));
}

/**
 * @brief Generic mbtcp ok response without data (i.e., write func)
 *
 * @param tid Transaction ID.
 * @return Modbus ok response string in JSON format.
 */ 
static char * set_modbus_no_data_ok_resp(int tid)
{
    BEGIN(enable_syslog);

    cJSON *resp_root;
    resp_root = cJSON_CreateObject();
    cJSON_AddNumberToObject(resp_root, "tid", tid);
    cJSON_AddStringToObject(resp_root, "status", "ok");
    char * resp_json_str = cJSON_PrintUnformatted(resp_root);
    LOG(enable_syslog, "resp: %s", resp_json_str);
    // clean up
    cJSON_Delete(resp_root);
    return resp_json_str;     
}

/**
 * @brief Generic mbtcp ok response with data (i.e., read func)
 *
 * @param tid Transaction ID.
 * @param json_arr cJSON pointer to data array
 * @return Modbus ok response string in JSON format.
 */ 
static char * set_modbus_with_data_ok_resp(int tid, cJSON * json_arr)
{
    BEGIN(enable_syslog);

    cJSON *resp_root;
    resp_root = cJSON_CreateObject();
    cJSON_AddNumberToObject(resp_root, "tid", tid);
    cJSON_AddItemToObject(resp_root, "data", json_arr);
    cJSON_AddStringToObject(resp_root, "status", "ok");
    char * resp_json_str = cJSON_PrintUnformatted(resp_root);
    LOG(enable_syslog, "resp: %s", resp_json_str);
    // clean up
    cJSON_Delete(resp_root);
    return resp_json_str;
}

/**
 * @brief Help function. FC1, FC2 request handler
 *
 * @fc Function code 1 and 2 only.
 * @param handle Mbtcp handle.
 * @param req cJSON request object.
 * @return Modbus response string in JSON format.
 */
static char * mbtcp_read_bit_req(int fc, mbtcp_handle_s *handle, cJSON *req)
{
    BEGIN(enable_syslog);
    int addr = json_get_int(req, "addr");
    int len  = json_get_int(req, "len");
    int tid  = json_get_int(req, "tid");
    if (len > MODBUS_MAX_READ_BITS) // 2000
    {
        return modbus_set_error_resp(tid, "Too many bits requested");
    }
    else
    {
        uint8_t bits[len];
        // memory reset for variable length array
        memset(bits, 0, len * sizeof(uint8_t));
        int ret = 0;
        switch (fc)
        {
            case 1:
                ret = modbus_read_bits(handle->ctx, addr, len, bits);
                break;
            case 2:
                ret = modbus_read_input_bits(handle->ctx, addr, len, bits);
                break;
            default:
                return modbus_set_error_resp(tid, "Wrong function code");
        }

        if (ret < 0) 
        {
            return set_modbus_errno_resp(tid, handle, errno);
        } 
        else 
        {
            LOG(enable_syslog, "fc:%d, desired length: %d, read length:%d", fc, len, ret);
            
            // [todo]:remove; debug only
            for (int ii = 0; ii < ret; ii++) 
            {
                LOG(enable_syslog, "[%d]=%d", ii, bits[ii]);
            }

            // uint8_t array
            return set_modbus_with_data_ok_resp(tid, cJSON_CreateUInt8Array(bits, len));
        }
    }    
}

/**
 * @brief Help function. FC3, FC4 request handler
 *
 * @fc Function code 3 and 4 only.
 * @param handle Mbtcp handle.
 * @param req cJSON request object.
 * @return Modbus response string in JSON format.
 */
static char * mbtcp_read_reg_req(int fc, mbtcp_handle_s *handle, cJSON *req)
{
    BEGIN(enable_syslog);
    int addr = json_get_int(req, "addr");
    int len  = json_get_int(req, "len");
    int tid  = json_get_int(req, "tid");
    if (len > MODBUS_MAX_READ_REGISTERS) // 125
    {
        return modbus_set_error_resp(tid, "Too many registers requested");
    }
    else
    {
        uint16_t regs[len];
        // memory reset for variable length array
        memset(regs, 0, len * sizeof(uint16_t));
        int ret = 0;
        switch (fc)
        {
            case 3:
                ret = modbus_read_registers(handle->ctx, addr, len, regs);
                break;
            case 4:
                ret = modbus_read_input_registers(handle->ctx, addr, len, regs);
                break;
            default:
                return modbus_set_error_resp(tid, "Wrong function code");
        }
        
        if (ret < 0) 
        {
            return set_modbus_errno_resp(tid, handle, errno);
        } 
        else 
        {
            LOG(enable_syslog, "fc:%d, desired length: %d, read length:%d", fc, len, ret);
            
            // [todo]:remove; debug only
            for (int ii = 0; ii < ret; ii++) 
            {
                LOG(enable_syslog, "[%d]=%d", ii, regs[ii]);
            }
            
            // uint16_t array
            return set_modbus_with_data_ok_resp(tid, cJSON_CreateUInt16Array(regs, len));
        }
    }
}

/**
 * @brief Help function. FC5, FC6 request handler
 *
 * @fc Function code 5 and 6 only.
 * @param handle Mbtcp handle.
 * @param req cJSON request object.
 * @return Modbus response string in JSON format.
 */
static char * mbtcp_single_write_req(int fc, mbtcp_handle_s *handle, cJSON *req)
{
    BEGIN(enable_syslog);
    int addr = json_get_int(req, "addr");
    int tid  = json_get_int(req, "tid");
    int data = json_get_int(req, "data");
    int ret = 0;
    switch (fc)
    {
        case 5:
            ret = modbus_write_bit(handle->ctx, addr, data);
            break;
        case 6:
            ret = modbus_write_register(handle->ctx, addr, data);
            break;
        default:
            return modbus_set_error_resp(tid, "Wrong function code");
    }

    if (ret < 0) 
    {
        return set_modbus_errno_resp(tid, handle, errno);
    }
    else
    {
        return set_modbus_no_data_ok_resp(tid);   
    }
}

/**
 * @brief Help function. FC15, FC16 request handler
 *
 * @fc Function code 15 and 16 only.
 * @param handle Mbtcp handle.
 * @param req cJSON request object.
 * @return Modbus response string in JSON format.
 */
static char * mbtcp_multi_write_req(int fc, mbtcp_handle_s *handle, cJSON *req)
{
    BEGIN(enable_syslog);
    int addr = json_get_int(req, "addr");
    int len  = json_get_int(req, "len");
    int tid  = json_get_int(req, "tid");
    
    uint8_t *bits;      // FC15
    uint16_t *regs;     // FC16
    cJSON * data = NULL;
    int ret = 0;

    switch (fc)
    {
        case 15:
            // memory reset for variable length array
            bits = (uint8_t *) malloc(len * sizeof(uint8_t));
            memset(bits, 0, len * sizeof(uint8_t));
            // handle uint8_t array
            data = cJSON_GetObjectItem(req, "data");
            for (int i = 0 ; i < cJSON_GetArraySize(data) ; i++)
            {
                uint8_t subitem = cJSON_GetArrayItem(data, i)->valueint;
                bits[i] = subitem;
                LOG(enable_syslog, "[%d]=%d", i, bits[i]);
            }
            ret = modbus_write_bits(handle->ctx, addr, len, bits);
            free(bits);
            break;
        case 16:
            
            // memory reset for variable length array
            regs = (uint16_t *) malloc(len * sizeof(uint16_t));
            memset(regs, 0, len * sizeof(uint16_t));
            // handle uint16_t array
            data = cJSON_GetObjectItem(req, "data");
            for (int i = 0 ; i < cJSON_GetArraySize(data) ; i++)
            {
                uint16_t subitem = cJSON_GetArrayItem(data, i)->valueint;
                regs[i] = subitem;
                LOG(enable_syslog, "[%d]=%d", i, regs[i]);
            }
            ret = modbus_write_registers(handle->ctx, addr, len, regs);
            free(regs);
            break;
        default:
            return modbus_set_error_resp(tid, "Wrong function code");
    }

    if (ret < 0) 
    {
        return set_modbus_errno_resp(tid, handle, errno);
    } 
    else
    {
        return set_modbus_no_data_ok_resp(tid);         
    }
}

/* ==================================================
 *  public functions
================================================== */

char * modbus_set_error_resp(int tid, const char *reason)
{
    BEGIN(enable_syslog);
    
    cJSON *resp_root;
    resp_root = cJSON_CreateObject();
    cJSON_AddNumberToObject(resp_root, "tid", tid);
    cJSON_AddStringToObject(resp_root, "status", reason);
    char * resp_json_string = cJSON_PrintUnformatted(resp_root);
    LOG(enable_syslog, "resp: %s", resp_json_string);
    
    // clean up
    cJSON_Delete(resp_root);
    return resp_json_string;
}

bool mbtcp_get_connection_status(mbtcp_handle_s *handle)
{
    BEGIN(enable_syslog);
    
    if ((handle) == NULL)
    {
        ERR(enable_syslog, "NULL handle");
        return false;
    }
    
    LOG(enable_syslog, "%s:%s connected: %s", handle->key.ip, 
                                              handle->key.port, 
                                              handle->connected ? "true" : "false");
    return handle->connected;
}

bool mbtcp_do_connect(mbtcp_handle_s *handle, char ** reason)
{
    BEGIN(enable_syslog);
    
    if (handle == NULL)
    {
        ERR(enable_syslog, "NULL handle");
        *reason = "NULL handle";
        return false;
    }
    
    if (modbus_connect(handle->ctx) == -1) 
    {
        ERR(enable_syslog, "Connection failed: %s", modbus_strerror(errno));
        *reason = (char *)modbus_strerror(errno);
        return false;
    }
    else
    {
        LOG(enable_syslog, "%s:%s connected", handle->key.ip, handle->key.port);
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
        printf("ip:%s, port:%s\n", handle->key.ip, handle->key.port);
    }
}

bool mbtcp_init_handle(mbtcp_handle_s **ptr_handle, char *ip, char *port)
{
    BEGIN(enable_syslog);

    // create a mbtcp context
    modbus_t *ctx = modbus_new_tcp_pi(ip, port);
    
    if (ctx == NULL)
    {
        ERR(enable_syslog, "Unable to allocate mbtcp context");
        return false; // fail to allocate context
    }
        
    // set tcp connection timeout
    modbus_set_response_timeout(ctx, 0, tcp_conn_timeout_usec);
    LOG(enable_syslog, "set response timeout:%d", tcp_conn_timeout_usec);
    
    // @add context to mbtcp hashtable
    mbtcp_handle_s *handle;
    handle = (mbtcp_handle_s*)malloc(sizeof(mbtcp_handle_s));
    // let alignment bytes being set to zero-value!!
    memset(handle, 0, sizeof(mbtcp_handle_s));
    handle->connected = false;
    strcpy(handle->key.ip, ip);
    strcpy(handle->key.port, port);
    handle->ctx = ctx;

    HASH_ADD(hh, mbtcp_htable, key, sizeof(mbtcp_key_s), handle);
    LOG(enable_syslog, "Add %s:%s to mbtcp hashtable", handle->key.ip, mbtcp_htable->key.port);

    // call by reference to `mbtcp handle address`
    *ptr_handle = handle;

    // @connect to server
    char * reason = NULL;
    mbtcp_do_connect(handle, &reason);
    return true;
}

bool mbtcp_get_handle(mbtcp_handle_s **ptr_handle, char *ip, char *port)
{
    BEGIN(enable_syslog);
    
    mbtcp_handle_s query, *hash_ctx;
    memset(&query, 0, sizeof(mbtcp_handle_s));
    strcpy(query.key.ip, ip);
    strcpy(query.key.port, port);
    HASH_FIND(hh, mbtcp_htable, &query.key, sizeof(mbtcp_key_s), hash_ctx);
    
    if (hash_ctx != NULL)
    {
        LOG(enable_syslog, "tcp server %s:%s found", hash_ctx->key.ip, hash_ctx->key.port);
        // call by reference to `mbtcp handle address`
        *ptr_handle = hash_ctx; 
        return true;
    }
    else
    {
        ERR(enable_syslog, "tcp server %s:%s not found", query.key.ip, query.key.port);
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
        char * reason = NULL;
        if (lazy_mbtcp_connect(handle, &reason))
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
            return modbus_set_error_resp(tid, reason);
        }
    }
    else
    {
        return modbus_set_error_resp(tid, "init modbus tcp handle fail");
    }
}

char * mbtcp_set_response_timeout(int tid, long int timeout)
{
    BEGIN(enable_syslog);

    // set timeout
    tcp_conn_timeout_usec = timeout;
    
    cJSON *resp_root;
    resp_root = cJSON_CreateObject();
    cJSON_AddNumberToObject(resp_root, "tid", tid);
    cJSON_AddStringToObject(resp_root, "status", "ok");
    char * resp_json_string = cJSON_PrintUnformatted(resp_root);
    LOG(enable_syslog, "resp: %s", resp_json_string);
    
    // clean up
    cJSON_Delete(resp_root);
    return resp_json_string;
}

// Read coils.
char * mbtcp_fc1_req(mbtcp_handle_s *handle, cJSON *req)
{
    BEGIN(enable_syslog);
    return mbtcp_read_bit_req(1, handle, req);
}

// Read discrete input.
char * mbtcp_fc2_req(mbtcp_handle_s *handle, cJSON *req)
{
    BEGIN(enable_syslog);
    return mbtcp_read_bit_req(2, handle, req);
}

// Read holding registers.
char * mbtcp_fc3_req(mbtcp_handle_s *handle, cJSON *req)
{
    BEGIN(enable_syslog);   
    return mbtcp_read_reg_req(3, handle, req);  
}

// Read input registers.
char * mbtcp_fc4_req(mbtcp_handle_s *handle, cJSON *req)
{
    BEGIN(enable_syslog);
    return mbtcp_read_reg_req(4, handle, req);  
}

// Write single coil.
char * mbtcp_fc5_req(mbtcp_handle_s *handle, cJSON *req)
{
    BEGIN(enable_syslog);
    return mbtcp_single_write_req(5, handle, req);
}

// Write single register.
char * mbtcp_fc6_req(mbtcp_handle_s *handle, cJSON *req)
{
    BEGIN(enable_syslog);
    return mbtcp_single_write_req(6, handle, req);
}

// Write multiple coils.
char * mbtcp_fc15_req(mbtcp_handle_s *handle, cJSON *req)
{
    BEGIN(enable_syslog);
    return mbtcp_multi_write_req(15, handle, req);
}

// Write multiple registers.
char * mbtcp_fc16_req(mbtcp_handle_s *handle, cJSON *req)
{
    BEGIN(enable_syslog);
    return mbtcp_multi_write_req(16, handle, req);
}

