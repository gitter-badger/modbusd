/**
 * @file json.h
 * @author taka-wang
 * @brief cJSON utility wrapper header
*/

#ifndef JSON_H //prevent double inclusion
#define JSON_H

#include "json/cJSON.h" //cJson library

/**
 * @brief Get char string via key from cJSON object
 */
char * json_get_char(cJSON *inJson, const char *key);

/**
 * @brief Get int via key from cJSON object
 */
int json_get_int(cJSON *inJson, const char *key);

/**
 * @brief Load json file to cJSON
 */ 
int file_to_json(const char *fname, cJSON **outJson);

/**
 * @brief Save cJSON to json file
 */
int json_to_file(const char *fname, cJSON *inJson);

/**
 * @brief Release cJSON root (auto mode)
 */
void json_release(cJSON* inJson);

#endif // JSON_H