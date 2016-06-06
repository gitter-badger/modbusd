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
 *
 * Hash key. Ipv4 address and port composition.
 */
typedef struct 
{
    char ip[16];
    int port;
} mbtcp_key_s;

/**
 * @brief mbtcp handle type
 *
 * Hashable tcp handle strucut for connection keeping.
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
 *
 * @param tid Transaction ID.
 * @param reason Fail reason string.
 * @return Modbus response string in JSON format for zmsg.
 */
char * set_modbus_error_resp(int tid, const char *reason);

/**
 * @brief Init mbtcp handle (to hash) and try to connect
 *
 * @param ptr_handle Pointer to mbtcp handle.
 * @param ip IPv4 address string.
 * @param port Modbus TCP port number.
 * @return Success or not.
 */
bool mbtcp_init_handle(mbtcp_handle_s **ptr_handle, char *ip, int port);

/**
 * @brief Get mbtcp handle from hashtable
 *
 * @param ptr_handle Pointer to mbtcp handle.
 * @param ip IPv4 address string.
 * @param port Modbus TCP port number.
 * @return Success or not.
 */
bool mbtcp_get_handle(mbtcp_handle_s **ptr_handle, char *ip, int port);

/**
 * @brief List mbtcp hash table
 *
 * @return Void.
 */
void mbtcp_list_handles();

/**
 * @brief Connect to mbtcp slave via mbtcp hashed handle
 *
 * @param handle Mbtcp handle.  
 * @param reason Fail reason string.
 * @return Success or not.
 */
bool mbtcp_do_connect(mbtcp_handle_s *handle, char ** reason);

/**
 * @brief Get mbtcp handle's connection status
 *
 * @param handle Mbtcp handle.
 * @return Success or not. 
 */
bool mbtcp_get_connection_status(mbtcp_handle_s *handle);

/**
 * @brief Generic mbtcp command handler
 *
 * @param req cJSON request object.
 * @param fc Function pointer of modbus tcp function code handler.
 * @return Modbus response string in JSON format for zmsg.
 */
char * mbtcp_cmd_hanlder(cJSON *req, mbtcp_fc fc);

/**
 * @brief Do modbus tcp requests - FC1
 *
 * Read coils.  
 *
 * @param handle Mbtcp handle.
 * @param req cJSON request object.
 * @return Modbus response string in JSON format for zmsg.
 */
char * mbtcp_fc1_req(mbtcp_handle_s *handle, cJSON *req);

/**
 * @brief Do modbus tcp requests - FC2
 *
 * Read discrete input.
 *
 * @param handle Mbtcp handle.
 * @param req cJSON request object.
 * @return Modbus response string in JSON format for zmsg.
 */
char * mbtcp_fc2_req(mbtcp_handle_s *handle, cJSON *req);

/**
 * @brief Do modbus tcp requests - FC3
 *
 * Read holding registers.
 *
 * @param handle Mbtcp handle.
 * @param req cJSON request object.
 * @return Modbus response string in JSON format for zmsg.
 */
char * mbtcp_fc3_req(mbtcp_handle_s *handle, cJSON *req);

/**
 * @brief Do modbus tcp requests - FC4
 *
 * Read input registers.
 *
 * @param handle Mbtcp handle.
 * @param req cJSON request object.
 * @return Modbus response string in JSON format for zmsg.
 */
char * mbtcp_fc4_req(mbtcp_handle_s *handle, cJSON *req);

/**
 * @brief Do modbus tcp requests - FC5
 *
 * Write single coil.
 *
 * @param handle Mbtcp handle.
 * @param req cJSON request object.
 * @return Modbus response string in JSON format for zmsg.
 */
char * mbtcp_fc5_req(mbtcp_handle_s *handle, cJSON *req);

/**
 * @brief Do modbus tcp requests - FC6
 *
 * Write single register.
 *
 * @param handle Mbtcp handle.
 * @param req cJSON request object.
 * @return Modbus response string in JSON format for zmsg.
 */
char * mbtcp_fc6_req(mbtcp_handle_s *handle, cJSON *req);

/**
 * @brief Do modbus tcp requests - FC15
 *
 * Write multiple coils.
 *
 * @param handle Mbtcp handle.
 * @param req cJSON request object.
 * @return Modbus response string in JSON format for zmsg.
 */
char * mbtcp_fc15_req(mbtcp_handle_s *handle, cJSON *req);

/**
 * @brief Do modbus tcp requests - FC16
 *
 * Write multiple registers.
 *
 * @param handle Mbtcp handle.
 * @param req cJSON request object.
 * @return Modbus response string in JSON format for zmsg.
 */
char * mbtcp_fc16_req(mbtcp_handle_s *handle, cJSON *req);

#endif  // MODBUSD_H