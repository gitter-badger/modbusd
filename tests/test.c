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

//=================================
// Test functions
//=================================

// decode json string
void test_json_decode()
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

// encode to json string
void test_json_encode()
{
    int mdata[4]={116,943,234,38793};
    cJSON *root;
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "tid", 22);
    cJSON_AddItemToObject(root,"data", cJSON_CreateIntArray(mdata,4));
    cJSON_AddStringToObject(root, "status", "ok");
    printf("%s\n", cJSON_Print(root));
    cJSON_Delete(root);
    /*
    {
        "tid": 22,
        "data": [1,2,3,4],
        "status": "ok"
    }
    */
}

// init mbtcp handle and try to connect
int test_init_tcp_handle_and_connect()
{
    LOG(enable_syslog, "Init TCP handle");
    mbtcp_handle_s *handle = NULL;
    mbtcp_init_handle (&handle, "172.16.9.170", 502);
    //mbtcp_do_connect(&handle);

    LOG(enable_syslog, "Get TCP handle");
    handle = NULL;
    mbtcp_get_handle (&handle, "172.16.9.170", 502);
    
    LOG(enable_syslog, "Try to connect to slave");
    mbtcp_do_connect(&handle);
    mbtcp_get_connection_status(handle);
    return 0;
}

// add 1000 key-value pair to hashtable, then find these items
void test_multiple_add_find()
{
    BEGIN(enable_syslog);
    // important! initialize to NULL
    mbtcp_handle_s *servers = NULL;
    
    for (int idx = 0; idx < 1000; idx++)
    {
        mbtcp_handle_s *handle;
        handle = (mbtcp_handle_s*)malloc(sizeof(mbtcp_handle_s));
        memset(handle, 0, sizeof(mbtcp_handle_s));
        handle->connected = false;
        strcpy(handle->key.ip, "192.168.10.12");
        handle->key.port = idx;
        handle->ctx = modbus_new_tcp(handle->key.ip, handle->key.port);
        HASH_ADD(hh, servers, key, sizeof(mbtcp_key_s), handle);
        LOG(enable_syslog, "handle:%d, %p\n", idx, handle);
    }
    
    unsigned int num_users;
    num_users = HASH_COUNT(servers);
    LOG(enable_syslog, "there are %u users\n", num_users);
    LOG(enable_syslog,"==========================\n");
    
    for (int idx = 0; idx < 1000; idx++)
    {
        mbtcp_handle_s query, *ptr;
        memset(&query, 0, sizeof(mbtcp_handle_s));
        strcpy(query.key.ip, "192.168.10.12");
        query.key.port = idx;
        HASH_FIND(hh, servers, &query.key, sizeof(mbtcp_key_s), ptr);
        
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


void test_single_add_find()
{
    BEGIN(enable_syslog);
    
    // important! initialize to NULL
    mbtcp_handle_s *servers = NULL;
    mbtcp_handle_s ff, *p, *h1, *h2, *tmp;
    
    // server #1
    h1 = (mbtcp_handle_s*)malloc(sizeof(mbtcp_handle_s));
    // let alignment bytes being set to zero-value.
    // ref: https://troydhanson.github.io/uthash/userguide.html#_structure_keys
    memset(h1, 0, sizeof(h1));
    h1->connected = false;
    strcpy(h1->key.ip, "192.168.10.1");
    h1->key.port = 555;
    h1->ctx = modbus_new_tcp(h1->key.ip, h1->key.port);
    HASH_ADD(hh, servers, key, sizeof(mbtcp_key_s), h1);
    
    // server #2
    h2 = (mbtcp_handle_s*)malloc(sizeof(mbtcp_handle_s));
    memset(h2, 0, sizeof(h2));
    h2->connected = false;
    strcpy(h2->key.ip, "192.168.10.2");
    h2->key.port = 556;
    h2->ctx = modbus_new_tcp(h2->key.ip, h2->key.port);
    HASH_ADD(hh, servers, key, sizeof(mbtcp_key_s), h2);
    
    // find #1
    memset(&ff, 0, sizeof(mbtcp_handle_s));
    strcpy(ff.key.ip, "192.168.10.1");
    ff.key.port = 555;
    HASH_FIND(hh, servers, &ff.key, sizeof(mbtcp_key_s), p);
    
    if (p)
    {
        LOG(enable_syslog, "Found %s, %d\n", p->key.ip, p->key.port);
    }
    else
    {
        LOG(enable_syslog, "Not found\n");
    }
    
    // find #2
    memset(&ff, 0, sizeof(mbtcp_handle_s));
    strcpy(ff.key.ip, "192.168.10.2");
    ff.key.port = 556;
    HASH_FIND(hh, servers, &ff.key, sizeof(mbtcp_key_s), p);
    
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
    
    test_json_decode();
    test_json_encode();
    test_init_tcp_handle_and_connect();
    test_multiple_add_find();
    test_single_add_find();
    
    exit(EXIT_SUCCESS);
}