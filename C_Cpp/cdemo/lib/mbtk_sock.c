#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <sys/time.h>
#include <fcntl.h>
#include <netdb.h>

#include "mbtk_sock.h"

#define SA struct sockaddr

static int epoll_fd = -1;
static mbtk_sock_s mbtk_socks[MBTK_SOCK_NUM_MAX] = {{0}};

static void
sig_alrm(int signo)
{
    LOGD("Timeout.\n");
}

int mbtk_sock_open(
    mbtk_sock_type_enum type,bool is_ssl,
    mbtk_sock_can_read_cb_func can_read_cb,
    mbtk_sock_read_cb_func read_cb)
{
    int index = 0;
    // Jump to first NULL struct.
    while(index < MBTK_SOCK_NUM_MAX && mbtk_socks[index].fd > 0) {
        index++;
    }
    if(index == MBTK_SOCK_NUM_MAX) {
        LOGE("Socket fd too more.");
        goto result_fail;
    }

    mbtk_socks[index].type = type;
    if(type == MBTK_SOCK_TYPE_UDP) { // UDP
        if((mbtk_socks[index].fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
            LOGE("socket() fail.[%d]",errno);
            goto result_fail;
        }
    } else { // TCP
        if((mbtk_socks[index].fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
            LOGE("socket() fail.[%d]",errno);
            goto result_fail;
        }
    }

    mbtk_socks[index].is_ssl = is_ssl;

    // Is async
    if(can_read_cb || read_cb) {
        mbtk_socks[index].is_sync = FALSE;
        mbtk_socks[index].can_read_cb = can_read_cb;
        mbtk_socks[index].read_cb = read_cb;
        // Set O_NONBLOCK
        int flags = fcntl(mbtk_socks[index].fd, F_GETFL, 0);
        if (flags < 0) {
            LOGE("Get flags error:%s\n", strerror(errno));
            goto result_fail_with_close;
        }
        flags |= O_NONBLOCK;
        if (fcntl(mbtk_socks[index].fd, F_SETFL, flags) < 0) {
            LOGE("Set flags error:%s\n", strerror(errno));
            goto result_fail_with_close;
        }
    } else {
        mbtk_socks[index].is_sync = TRUE;
        // Set O_NONBLOCK
        int flags = fcntl(mbtk_socks[index].fd, F_GETFL, 0);
        if (flags < 0) {
            LOGE("Get flags error:%s\n", strerror(errno));
            goto result_fail_with_close;
        }
        flags &= ~O_NONBLOCK;
        if (fcntl(mbtk_socks[index].fd, F_SETFL, flags) < 0) {
            LOGE("Set flags error:%s\n", strerror(errno));
            goto result_fail_with_close;
        }
    }

    return mbtk_socks[index].fd;

result_fail_with_close:
    close(mbtk_socks[index].fd);
    memset(&mbtk_socks[index],0x0,sizeof(mbtk_sock_s));

result_fail:
    LOGE("mbtk_sock() end:fail");
    return -1;
}

int mbtk_sock_conn(int fd,const void* host,int port)
{
    int index = 0;
    // Jump to first NULL struct.
    while(index < MBTK_SOCK_NUM_MAX && mbtk_socks[index].fd != fd) {
        index++;
    }
    if(fd <= 0 || index == MBTK_SOCK_NUM_MAX) {
        LOGE("No such socket fd.");
        goto result_fail;
    }

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);

    struct hostent *he = gethostbyname(host);
    if (he == NULL){
        LOGE("gethostbyname() fail.[%d]",errno);
        goto result_fail_with_close;
    } else {
        LOGD("Ip : %d",he->h_addr_list[0]);
    }
    memcpy(&servaddr.sin_addr, he->h_addr_list[0], sizeof(struct in_addr));

    if(!mbtk_socks[index].is_sync) { // No block.
        if(connect(mbtk_socks[index].fd, (SA *) &servaddr, sizeof(servaddr)) < 0){
            if(EINPROGRESS != errno){
                LOGE("connect() fail.[%d]",errno);
                goto result_fail_with_close;
            }
        }

        fd_set rset, wset;
        FD_ZERO(&rset);
        FD_ZERO(&wset);
        FD_SET(mbtk_socks[index].fd, &rset);
        FD_SET(mbtk_socks[index].fd, &wset);
        struct timeval time_out;
        time_out.tv_sec = 10; // 10 s
        time_out.tv_usec = 0;
        int nready = select(mbtk_socks[index].fd + 1, &rset, &wset, NULL, &time_out);
        LOGD("nready = %d",nready);
        if(nready == 0){// Timeout
            LOGE("Timeout.");
            goto result_fail_with_close;
        }else{
            if (FD_ISSET(mbtk_socks[index].fd, &rset)
                && FD_ISSET(mbtk_socks[index].fd, &wset)) {
                int error = -1;
                int len = sizeof(int);
                LOGE("Can read and write.");
                if(getsockopt(mbtk_socks[index].fd, SOL_SOCKET, SO_ERROR, &error, &len) < 0){
                    LOGE("getsockopt fail.[%d]",errno);
                    goto result_fail_with_close;
                }else{
                    LOGE("error = %d",error);
                    if(error == 0){ // Success
                        goto result_success;
                    }else{// Fail
                        goto result_fail_with_close;
                    }
                }
            }else if(FD_ISSET(mbtk_socks[index].fd, &wset)){
                LOGE("Can write.");
                goto result_success;
            }else{
                LOGE("Can read(Impossible).");
                goto result_fail_with_close;
            }
        }
    } else {
        /* Set interval timer to go off before 3WHS completes */
        signal(SIGALRM, sig_alrm);
        struct itimerval val;
        val.it_interval.tv_sec  = 0;
        val.it_interval.tv_usec = 0;
        val.it_value.tv_sec  = 10;
        val.it_value.tv_usec = 500000;  /* 500 ms */
        if (setitimer(ITIMER_REAL, &val, NULL) == -1) {
            LOGE("setitimer error");
            goto result_fail_with_close;
        }

        if(connect(mbtk_socks[index].fd, (SA *) &servaddr, sizeof(servaddr)) < 0){
            LOGE("connect() fail.[%d]",errno);
            goto result_fail_with_close;
        }

        struct itimerval value;
        value.it_value.tv_sec = 0;
        value.it_value.tv_usec = 0;
        value.it_interval = value.it_value;
        setitimer(ITIMER_REAL, &value, NULL);
    }

result_success:
{
    if(!mbtk_socks[index].is_sync) {
        struct epoll_event ev;
        ev.data.fd = mbtk_socks[index].fd;
        ev.events = EPOLLIN | EPOLLET;
        epoll_ctl(epoll_fd,EPOLL_CTL_ADD,mbtk_socks[index].fd,&ev);
    }
    LOGD("end:success");
    return 0;
}
result_fail_with_close:
    close(mbtk_socks[index].fd);
    memset(&mbtk_socks[index],0x0,sizeof(mbtk_sock_s));

result_fail:
    LOGE("mbtk_sock_conn() end:fail");
    return -1;
}

int mbtk_sock_close(int fd)
{
    int index = 0;
    // Jump to first NULL struct.
    while(index < MBTK_SOCK_NUM_MAX && mbtk_socks[index].fd != fd) {
        index++;
    }
    if(fd <= 0 || index == MBTK_SOCK_NUM_MAX) {
        LOGE("No such socket fd.");
        return -1;
    }

    close(mbtk_socks[index].fd);
    memset(&mbtk_socks[index],0x0,sizeof(mbtk_sock_s));

    return 0;
}

