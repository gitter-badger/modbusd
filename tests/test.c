//
// hash_test.c
// taka-wang
//

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <czmq.h>

#include "../modbusd.h"

int enable_syslog = 1;

/*
{
	"ip": "192.168.3.2",
	"port": 502,
	"slave": 22,
	"tid": 1,
    "mode": "tcp",
	"cmd": "fc5",
	"addr": 250,
	"len": 10,
	"data": [1,2,3,4]
}

*/
// decode json string
void json_decode()
{
    char jstr[] = "{\n"
    "    \"ip\": \"192.168.3.2\",\n"
    "    \"port\": 502,\n"
    "    \"slave\": 22,\n"
    "    \"tid\": 1,\n"
    "    \"mode\": \"tcp\",\n"
    "    \"cmd\": \"fc5\",\n"
    "    \"addr\": 250,\n"
    "    \"len\": 10,\n"
    "    \"data\": [1,2,3,4]\n"
    "}";
    cJSON *json = cJSON_Parse(jstr);
    if (json)
    {
        printf("%s\n", cJSON_Print(json));
        printf("---------\n");
        printf("ip:%s\n", cJSON_GetObjectItem(json, "ip")->valuestring);
        printf("port:%d\n",cJSON_GetObjectItem(json, "port")->valueint);
        printf("mode:%s\n", cJSON_GetObjectItem(json, "mode")->valuestring);
        printf("addr:%d\n",cJSON_GetObjectItem(json, "addr")->valueint);
        
        // handle array
        cJSON * data = cJSON_GetObjectItem(json, "data");
        for (int i = 0 ; i < cJSON_GetArraySize(data) ; i++)
        {
            int subitem = cJSON_GetArrayItem(data, i)->valueint;
            printf("idx:%d,v:%d\n", i, subitem);
        }
        if (json != NULL) cJSON_Delete(json);
    }
}

/*
{
	"tid": 22,
	"data": [1,2,3,4],
	"status": "ok"
}
*/
// encode to json string
void json_encode()
{
    int mdata[4]={116,943,234,38793};
    cJSON *root;
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "tid", 22);
    cJSON_AddItemToObject(root,"data", cJSON_CreateIntArray(mdata,4));
    cJSON_AddStringToObject(root, "status", "ok");
    printf("%s\n", cJSON_Print(root));
    cJSON_Delete(root);
    
}

// init mbtcp handle and try to connect
int init_tcp_handle_and_connect()
{
    LOG(enable_syslog, "Init TCP handle");
    mbtcp_handle_t *handle = NULL;
    int ret = init_mbtcp_handle (&handle, "172.16.9.170", 502);
    ret = mbtcp_connect(&handle);

    LOG(enable_syslog, "Get TCP handle");
    handle = NULL;
    ret = get_mbtcp_handle (&handle, "172.16.9.170", 502);
    
    LOG(enable_syslog, "Try to connect to slave");
    ret = mbtcp_connect(&handle);
    ret = get_mbtcp_connection_status(&handle);
    return ret;
}

// add 1000 key-value pair to hashtable, then find these items
void multiple_add_find()
{
    BEGIN(enable_syslog);
    // important! initialize to NULL
    mbtcp_handle_t *servers = NULL;
    
    for (int idx = 0; idx < 1000; idx++)
    {
        mbtcp_handle_t *handle;
        handle = (mbtcp_handle_t*)malloc(sizeof(mbtcp_handle_t));
        memset(handle, 0, sizeof(mbtcp_handle_t));
        handle->connected = false;
        handle->key.ip   = "192.168.10.12";
        handle->key.port = idx;
        handle->ctx = modbus_new_tcp(handle->key.ip, handle->key.port);
        HASH_ADD(hh, servers, key, sizeof(mbtcp_key_t), handle);
        LOG(enable_syslog, "handle:%d, %p\n", idx, handle);
    }
    
    unsigned int num_users;
    num_users = HASH_COUNT(servers);
    LOG(enable_syslog, "there are %u users\n", num_users);
    LOG(enable_syslog,"==========================\n");
    
    for (int idx = 0; idx < 1000; idx++)
    {
        mbtcp_handle_t query, *ptr;
        memset(&query, 0, sizeof(mbtcp_handle_t));
        query.key.ip = "192.168.10.12";
        query.key.port = idx;
        HASH_FIND(hh, servers, &query.key, sizeof(mbtcp_key_t), ptr);
        
        if (ptr)
        {
            LOG(enable_syslog, "found: %d, %s, %d, %p\n", idx, ptr->key.ip, ptr->key.port, ptr);
        }
        else
        {
            LOG(enable_syslog, "not found: %d\n", idx);
        }
    }
}


void single_add_find()
{
    BEGIN(enable_syslog);
    
    // important! initialize to NULL
    mbtcp_handle_t *servers = NULL;
    mbtcp_handle_t ff, *p, *h1, *h2, *tmp;
    
    // server #1
    h1 = (mbtcp_handle_t*)malloc(sizeof(mbtcp_handle_t));
    // let alignment bytes being set to zero-value.
    // ref: https://troydhanson.github.io/uthash/userguide.html#_structure_keys
    memset(h1, 0, sizeof(h1));
    h1->connected = false;
    h1->key.ip   = "192.168.10.1";
    h1->key.port = 555;
    h1->ctx = modbus_new_tcp(h1->key.ip, h1->key.port);
    HASH_ADD(hh, servers, key, sizeof(mbtcp_key_t), h1);
    
    // server #2
    h2 = (mbtcp_handle_t*)malloc(sizeof(mbtcp_handle_t));
    memset(h2, 0, sizeof(h2));
    h2->connected = false;
    h2->key.ip   = "192.168.10.2";
    h2->key.port = 556;
    h2->ctx = modbus_new_tcp(h2->key.ip, h2->key.port);
    HASH_ADD(hh, servers, key, sizeof(mbtcp_key_t), h2);
    
    // find #1
    memset(&ff, 0, sizeof(mbtcp_handle_t));
    ff.key.ip = "192.168.10.1";
    ff.key.port = 555;
    HASH_FIND(hh, servers, &ff.key, sizeof(mbtcp_key_t), p);
    
    if (p)
    {
        LOG(enable_syslog, "Found %s, %d\n", p->key.ip, p->key.port);
    }
    else
    {
        LOG(enable_syslog, "Not found\n");
    }
    
    // find #2
    memset(&ff, 0, sizeof(mbtcp_handle_t));
    ff.key.ip = "192.168.10.2";
    ff.key.port = 556;
    HASH_FIND(hh, servers, &ff.key, sizeof(mbtcp_key_t), p);
    
    if (p)
    {
        LOG(enable_syslog, "Found %s, %d\n", p->key.ip, p->key.port);
    }
    else
    {
        LOG(enable_syslog, "Not found\n");
    }
    
    // delete
    HASH_ITER(hh, servers, p, tmp)
    {
        HASH_DEL(servers, p); // remove from hash
        modbus_free(p->ctx);  // release modbus context
        free(p);
    }
}

// ENTRY
int main(int argc, char *argv[])
{
    BEGIN(enable_syslog);
    json_decode();
    //json_encode();
    //init_tcp_handle_and_connect();
    //multiple_add_find();
    //single_add_find();
    
    exit(EXIT_SUCCESS);
}