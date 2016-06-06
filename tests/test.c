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


void test_zhashx()
{
    zhashx_t *hash = zhashx_new ();
    assert (hash);
    assert (zhashx_size (hash) == 0);
    assert (zhashx_first (hash) == NULL);
    assert (zhashx_cursor (hash) == NULL);

    // Insert some items
    int rc;
    rc = zhashx_insert (hash, "DEADBEEF", "dead beef");
    char *item = (char *) zhashx_first (hash);
    assert (streq ((char *) zhashx_cursor (hash), "DEADBEEF"));
    assert (streq (item, "dead beef"));
    assert (rc == 0);
    rc = zhashx_insert (hash, "ABADCAFE", "a bad cafe");
    assert (rc == 0);
    rc = zhashx_insert (hash, "C0DEDBAD", "coded bad");
    assert (rc == 0);
    rc = zhashx_insert (hash, "DEADF00D", "dead food");
    assert (rc == 0);
    assert (zhashx_size (hash) == 4);

    // Look for existing items
    item = (char *) zhashx_lookup (hash, "DEADBEEF");
    assert (streq (item, "dead beef"));
    item = (char *) zhashx_lookup (hash, "ABADCAFE");
    assert (streq (item, "a bad cafe"));
    item = (char *) zhashx_lookup (hash, "C0DEDBAD");
    assert (streq (item, "coded bad"));
    item = (char *) zhashx_lookup (hash, "DEADF00D");
    assert (streq (item, "dead food"));

    // Look for non-existent items
    item = (char *) zhashx_lookup (hash, "foo");
    assert (item == NULL);

    // Try to insert duplicate items
    rc = zhashx_insert (hash, "DEADBEEF", "foo");
    assert (rc == -1);
    item = (char *) zhashx_lookup (hash, "DEADBEEF");
    assert (streq (item, "dead beef"));

    // Some rename tests

    // Valid rename, key is now LIVEBEEF
    rc = zhashx_rename (hash, "DEADBEEF", "LIVEBEEF");
    assert (rc == 0);
    item = (char *) zhashx_lookup (hash, "LIVEBEEF");
    assert (streq (item, "dead beef"));

    // Trying to rename an unknown item to a non-existent key
    rc = zhashx_rename (hash, "WHATBEEF", "NONESUCH");
    assert (rc == -1);

    // Trying to rename an unknown item to an existing key
    rc = zhashx_rename (hash, "WHATBEEF", "LIVEBEEF");
    assert (rc == -1);
    item = (char *) zhashx_lookup (hash, "LIVEBEEF");
    assert (streq (item, "dead beef"));

    // Trying to rename an existing item to another existing item
    rc = zhashx_rename (hash, "LIVEBEEF", "ABADCAFE");
    assert (rc == -1);
    item = (char *) zhashx_lookup (hash, "LIVEBEEF");
    assert (streq (item, "dead beef"));
    item = (char *) zhashx_lookup (hash, "ABADCAFE");
    assert (streq (item, "a bad cafe"));

    // Test keys method
    zlistx_t *keys = zhashx_keys (hash);
    assert (zlistx_size (keys) == 4);
    zlistx_destroy (&keys);

    zlistx_t *values = zhashx_values(hash);
    assert (zlistx_size (values) == 4);
    zlistx_destroy (&values);

    // Test dup method
    zhashx_t *copy = zhashx_dup (hash);
    assert (zhashx_size (copy) == 4);
    item = (char *) zhashx_lookup (copy, "LIVEBEEF");
    assert (item);
    assert (streq (item, "dead beef"));
    zhashx_destroy (&copy);

    // Test pack/unpack methods
    zframe_t *frame = zhashx_pack (hash);
    copy = zhashx_unpack (frame);
    zframe_destroy (&frame);
    assert (zhashx_size (copy) == 4);
    item = (char *) zhashx_lookup (copy, "LIVEBEEF");
    assert (item);
    assert (streq (item, "dead beef"));
    zhashx_destroy (&copy);

    // Test save and load
    zhashx_comment (hash, "This is a test file");
    zhashx_comment (hash, "Created by %s", "czmq_selftest");
    zhashx_save (hash, ".cache");
    copy = zhashx_new ();
    assert (copy);
    zhashx_load (copy, ".cache");
    item = (char *) zhashx_lookup (copy, "LIVEBEEF");
    assert (item);
    assert (streq (item, "dead beef"));
    zhashx_destroy (&copy);
    zsys_file_delete (".cache");

    // Delete a item
    zhashx_delete (hash, "LIVEBEEF");
    item = (char *) zhashx_lookup (hash, "LIVEBEEF");
    assert (item == NULL);
    assert (zhashx_size (hash) == 3);

    // Check that the queue is robust against random usage
    struct {
    char name [100];
    bool exists;
    } testset [200];
    memset (testset, 0, sizeof (testset));
    int testmax = 200, testnbr, iteration;

    srandom ((unsigned) time (NULL));
    for (iteration = 0; iteration < 25000; iteration++) {
    testnbr = randof (testmax);
    if (testset [testnbr].exists) {
    item = (char *) zhashx_lookup (hash, testset [testnbr].name);
    assert (item);
    zhashx_delete (hash, testset [testnbr].name);
    testset [testnbr].exists = false;
    }
    else {
    sprintf (testset [testnbr].name, "%x-%x", rand (), rand ());
    if (zhashx_insert (hash, testset [testnbr].name, "") == 0)
    testset [testnbr].exists = true;
    }
    }
    // Test 10K lookups
    for (iteration = 0; iteration < 10000; iteration++)
    item = (char *) zhashx_lookup (hash, "DEADBEEFABADCAFE");

    // Destructor should be safe to call twice
    zhashx_destroy (&hash);
    zhashx_destroy (&hash);
    assert (hash == NULL);

    // Test autofree; automatically copies and frees string values
    hash = zhashx_new ();
    assert (hash);
    zhashx_autofree (hash);
    char value [255];
    strcpy (value, "This is a string");
    rc = zhashx_insert (hash, "key1", value);
    assert (rc == 0);
    strcpy (value, "Ring a ding ding");
    rc = zhashx_insert (hash, "key2", value);
    assert (rc == 0);
    assert (streq ((char *) zhashx_lookup (hash, "key1"), "This is a string"));
    assert (streq ((char *) zhashx_lookup (hash, "key2"), "Ring a ding ding")); zhashx_destroy (&hash);
}

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
    //char * reason = NULL;
    //mbtcp_do_connect(handle, &reason);

    LOG(enable_syslog, "Get TCP handle");
    handle = NULL;
    mbtcp_get_handle (&handle, "172.16.9.170", 502);
    
    LOG(enable_syslog, "Try to connect to slave");
    char * reason = NULL;
    mbtcp_do_connect(handle, &reason);
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
int main()
{
    BEGIN(enable_syslog);
    
    test_zhashx();
    test_json_decode();
    test_json_encode();
    test_init_tcp_handle_and_connect();
    test_multiple_add_find();
    test_single_add_find();

}