#ifndef MBTK_SOCK_INCLUDE
#define MBTK_SOCK_INCLUDE
#include <stdbool.h>
#include <stdio.h>

#include "m_type.h"

#ifdef MBTK_ANDROID_LOG
#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "mbtk_sock"
#define LOGI ALOGI
#define LOGD ALOGD
#define LOGW ALOGW
#define LOGE ALOGE
#else
#define LOGI(fmt, arg...) printf("%s(): " fmt "\n", __FUNCTION__, ##arg)
#define LOGD(fmt, arg...) printf("%s(): " fmt "\n", __FUNCTION__, ##arg)
#define LOGW(fmt, arg...) printf("%s(): " fmt "\n", __FUNCTION__, ##arg)
#define LOGE(fmt, arg...) printf("%s(): " fmt "\n", __FUNCTION__, ##arg)
#endif


#define MBTK_SOCK_HOST_LEN_MAX 50
#define MBTK_SOCK_NUM_MAX 30

typedef void (*mbtk_sock_read_cb_func)(int fd, void *buf, size_t nbytes);
typedef void (*mbtk_sock_can_read_cb_func)(int fd);

typedef enum {
    MBTK_SOCK_TYPE_TCP,
    MBTK_SOCK_TYPE_UDP
} mbtk_sock_type_enum;

typedef struct {
    int fd;
    mbtk_sock_type_enum type;
    bool is_sync;   // Will block
    bool is_ssl;    // Support SSL/TLS
    char host[MBTK_SOCK_HOST_LEN_MAX];
    int port;
    // This callback only for async socket.
    mbtk_sock_read_cb_func read_cb;
    mbtk_sock_can_read_cb_func can_read_cb;
} mbtk_sock_s;

int mbtk_sock_open(
    mbtk_sock_type_enum type,bool is_ssl,
    mbtk_sock_can_read_cb_func can_read_cb,
    mbtk_sock_read_cb_func read_cb);
int mbtk_sock_conn(int fd,const void* host,int port);
int mbtk_sock_close(int fd);


#endif /* MBTK_SOCK_INCLUDE */
