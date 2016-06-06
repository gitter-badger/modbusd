/**
 * @file modbusd.h
 * @author taka-wang
 * @brief modbus daemon exported api
*/

#ifndef MODBUSD_H
#define MODBUSD_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <stdbool.h>

#include <czmq.h>
#include <modbus.h>
#include "uthash.h"
#include "log.h"
#include "json.h"

/* ==================================================
 *  marco
================================================== */
#define MBTCP_RESP_TIMEOUT_SEC 30

/* ==================================================
 *  struct
================================================== */

/**
 * @brief `structure key` for modbus tcp hash table
 */
typedef struct 
{
    char ip[16];
    int port;
} mbtcp_key_s;

/**
 * @brief mbtcp handle type
 */
typedef struct 
{
    mbtcp_key_s key;    /** key */
    bool connected;     /** is connect to modbus slave? */
    modbus_t *ctx;      /** modbus context pointer */
    UT_hash_handle hh;  /** makes this structure hashable */
} mbtcp_handle_s;


/**
 * @brief function pointer of modbus tcp function code
 *
 * Function pointer of `modbus tcp function code request` for `generic command handle`.
 */
typedef char * (*mbtcp_fc)(mbtcp_handle_s *handle, cJSON *req);


/* ==================================================
 *  api
================================================== */

/**
 * @brief Generic mbtcp error response handler.
 */
char * set_modbus_resp_error(int tid, const char *reason);

/**
 * @brief Init mbtcp handle (to hash) and try to connect
 */
bool mbtcp_init_handle(mbtcp_handle_s **ptr_handle, char *ip, int port);

/**
 * @brief Get mbtcp handle from hashtable
 */
bool mbtcp_get_handle(mbtcp_handle_s **ptr_handle, char *ip, int port);

/**
 * @brief List mbtcp hash table
 */
void mbtcp_list_handles();

/**
 * @brief Connect to mbtcp slave via mbtcp hashed handle
 */
bool mbtcp_do_connect(mbtcp_handle_s *handle);

/**
 * @brief Get mbtcp handle's connection status
 */
bool mbtcp_get_connection_status(mbtcp_handle_s *handle);

/**
 * @brief Generic mbtcp command handler
 */
char * mbtcp_cmd_hanlder(cJSON *req, mbtcp_fc fc);

/**
 * @brief Do modbus tcp requests - FC1
 */
char * mbtcp_fc1_req(mbtcp_handle_s *handle, cJSON *req);

/**
 * @brief Do modbus tcp requests - FC2
 */
char * mbtcp_fc2_req(mbtcp_handle_s *handle, cJSON *req);

/**
 * @brief Do modbus tcp requests - FC3
 */
char * mbtcp_fc3_req(mbtcp_handle_s *handle, cJSON *req);

/**
 * @brief Do modbus tcp requests - FC4
 */
char * mbtcp_fc4_req(mbtcp_handle_s *handle, cJSON *req);

/**
 * @brief Do modbus tcp requests - FC5
 */
char * mbtcp_fc5_req(mbtcp_handle_s *handle, cJSON *req);

/**
 * @brief Do modbus tcp requests - FC6
 */
char * mbtcp_fc6_req(mbtcp_handle_s *handle, cJSON *req);

/**
 * @brief Do modbus tcp requests - FC15
 */
char * mbtcp_fc15_req(mbtcp_handle_s *handle, cJSON *req);

/**
 * @brief Do modbus tcp requests - FC16
 */
char * mbtcp_fc16_req(mbtcp_handle_s *handle, cJSON *req);

#endif  // MODBUSD_H