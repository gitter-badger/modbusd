//
// log.h
// taka-wang
//
#ifndef _LOG_H_
#define _LOG_H_

#include <syslog.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*===================================================================
              Logging functions
===================================================================*/

/* MAX ERROR MESSAGE LENGTH */
#define MSG_LEN 256
static char cstr[MSG_LEN];

/* Error Message */
#define ERR(flag, fmt, ...) \
    do { \
        snprintf(cstr, sizeof(cstr), "[%s:%d]: " fmt, __func__, __LINE__, ##__VA_ARGS__); \
        switch (flag)    \
        { \
            case 0:  { break;} \
            case 1:  { printf("[ERROR]%s\n", cstr); } \
            default: { syslog(LOG_ERR, "%s", cstr); break; } \
        } \
    } while (0)

/* Normal Message */
#define LOG(flag, fmt, ...) \
    do { \
        snprintf(cstr, sizeof(cstr), "[%s:%d]: " fmt, __func__, __LINE__, ##__VA_ARGS__); \
        switch (flag)    \
        { \
            case 0:  { break;} \
            case 1:  { printf("[LOG]%s\n", cstr); } \
            default: { syslog(LOG_INFO, "%s", cstr); break; } \
        } \
    } while (0)

/* FUNCTION BEGIN */
#define BEGIN(flag) \
    do { \
        snprintf(cstr, sizeof(cstr), "[BEGIN][%s:%d]", __func__, __LINE__); \
        switch (flag)    \
        { \
            case 0:  { break;} \
            case 1:  { printf("%s\n", cstr); } \
            default: { syslog(LOG_DEBUG, "%s", cstr); break; } \
        } \
    } while (0)

/* FUNCTION END */
#define END(flag) \
    do { \
        snprintf(cstr, sizeof(cstr), "[END][%s:%d]", __func__, __LINE__); \
        switch (flag)    \
        { \
            case 0:  { break;} \
            case 1:  { printf("%s\n", cstr); } \
            default: { syslog(LOG_DEBUG, "%s", cstr); break; } \
        } \
    } while (0)

#endif /* END OF _LOG_H_ */