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
 * @brief Function pointer of modbus tcp function code
 *
 * Function pointer to `modbus tcp function code request` for `generic command handle`.
 *
 * @param handle Mbtcp handle.
 * @param req cJSON request object.
 * @return Modbus response string in JSON format.
 */
typedef char * (*mbtcp_fc)(mbtcp_handle_s *handle, cJSON *req);


/* ==================================================
 *  api
================================================== */

/**
 * @brief Generic modbus (TCP/RTU) error response handler.
 *
 * @param tid Transaction ID.
 * @param reason Fail reason string.
 * @return Modbus response string in JSON format.
 */
char * set_modbus_error_resp(int tid, const char *reason);

/**
 * @brief Init mbtcp handle (to hashtable) and try to connect
 *
 * @param ptr_handle Pointer to mbtcp handle.
 * @param ip IPv4 address string.
 * @param port Modbus TCP server port number.
 * @return Success or not.
 */
bool mbtcp_init_handle(mbtcp_handle_s **ptr_handle, char *ipv4, int port);

/**
 * @brief Get mbtcp handle from hashtable
 *
 * @param ptr_handle Pointer to mbtcp handle.
 * @param ip IPv4 address string.
 * @param port Modbus TCP server port number.
 * @return Success or not.
 */
bool mbtcp_get_handle(mbtcp_handle_s **ptr_handle, char *ipv4, int port);

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
 * @param fc Function pointer to modbus tcp FC handler.
 * @return Modbus response string in JSON format.
 */
char * mbtcp_cmd_hanlder(cJSON *req, mbtcp_fc fc);

/**
 * @brief Modbus TCP Read coils.
 *
 * @param handle Mbtcp handle.
 * @param req cJSON request object.
 * @return Modbus response string in JSON format.
 */
char * mbtcp_fc1_req(mbtcp_handle_s *handle, cJSON *req);

/**
 * @brief Modbus TCP Read discrete input.
 *
 * @param handle Mbtcp handle.
 * @param req cJSON request object.
 * @return Modbus response string in JSON format.
 */
char * mbtcp_fc2_req(mbtcp_handle_s *handle, cJSON *req);

/**
 * @brief Modbus TCP Read holding registers.
 *
 * @param handle Mbtcp handle.
 * @param req cJSON request object.
 * @return Modbus response string in JSON format.
 */
char * mbtcp_fc3_req(mbtcp_handle_s *handle, cJSON *req);

/**
 * @brief Modbus TCP Read input registers.
 *
 * @param handle Mbtcp handle.
 * @param req cJSON request object.
 * @return Modbus response string in JSON format.
 */
char * mbtcp_fc4_req(mbtcp_handle_s *handle, cJSON *req);

/**
 * @brief Modbus TCP Write single coil.
 *
 * @param handle Mbtcp handle.
 * @param req cJSON request object.
 * @return Modbus response string in JSON format.
 */
char * mbtcp_fc5_req(mbtcp_handle_s *handle, cJSON *req);

/**
 * @brief Modbus TCP Write single register.
 *
 * @param handle Mbtcp handle.
 * @param req cJSON request object.
 * @return Modbus response string in JSON format.
 */
char * mbtcp_fc6_req(mbtcp_handle_s *handle, cJSON *req);

/**
 * @brief Modbus TCP Write multiple coils.
 *
 * @param handle Mbtcp handle.
 * @param req cJSON request object.
 * @return Modbus response string in JSON format.
 */
char * mbtcp_fc15_req(mbtcp_handle_s *handle, cJSON *req);

/**
 * @brief Modbus TCP Write multiple registers.
 *
 * @param handle Mbtcp handle.
 * @param req cJSON request object.
 * @return Modbus response string in JSON format.
 */
char * mbtcp_fc16_req(mbtcp_handle_s *handle, cJSON *req);

#endif  // MODBUSD_H