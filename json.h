//
// json.h cJSON utility wrapper header
// taka-wang
//

#ifndef JSON_H //prevent double inclusion
#define JSON_H

#include "json/cJSON.h" //cJson library

// get char string via key from cJSON object
char * json_get_char(cJSON *inJson, const char *key);

// get int via key from cJSON object
int json_get_int(cJSON *inJson, const char *key);

// load json file to cJSON 
int file_to_json(const char *fname, cJSON **outJson);

// save cJSON to json file
int json_to_file(const char *fname, cJSON *inJson);

// release cJSON root (auto mode)
void json_release(cJSON* inJson);

#endif // JSON_H