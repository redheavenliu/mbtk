#include "m_queue.h"
#include "m_type.h"
#include "m_list.h"

#include <pthread.h>
#include <errno.h>

typedef struct {
    int session_id;
    int *client_req;
    int msg_id;
    int err;
    int len;

    void *data;
} msg_t;


static bool running = TRUE;
static struct queue_T queue;

static list_T list;


static void * thr_fn_1(void * arg)
{
    msg_t *data;
    while(q_get(&queue,&data) != NULL) {
        printf("%d\n",data->client_req);
        free(data);
        data = NULL;
    }

    running = 0;
}

static int int_temp = 123;

static void list_free_func(msg_t *msg)
{
    if(msg != NULL)
    {
        if(msg->data != NULL)
        {
            free(msg->data);
            printf("Free data : %p\n",msg->data);
        }
    }
}

int main_queue(int argc,char *argv[])
{
    q_init(&queue);



    msg_t *msg = (msg_t *)malloc(sizeof(msg_t));
    memset(msg,0,sizeof(msg_t));
    msg->session_id = 0;
    msg->client_req = &int_temp;
    msg->msg_id = 0;
    msg->err = 0;
    msg->len = 0;
    msg->data = NULL;

    printf("client:%d\n",msg->client_req);

    q_put(&queue,msg);

    pthread_t thread_1;
    if (0 != pthread_create(&thread_1, NULL, thr_fn_1, NULL))
    {
        printf("error when create pthread1,%d\n", errno);
        return 1;
    }


    while(running)
        ;

    printf("end.\n");

    return 0;
}

int main_list(int argc,char *argv[])
{
    list_init(&list,list_free_func);



    msg_t *msg = (msg_t *)malloc(sizeof(msg_t));
    memset(msg,0,sizeof(msg_t));
    msg->data = (char*)malloc(100);
    memcpy(msg->data,"abc",3);
    list_add(&list,msg);

    msg = (msg_t *)malloc(sizeof(msg_t));
    memset(msg,0,sizeof(msg_t));
    msg->data = (char*)malloc(100);
    memcpy(msg->data,"123",3);
    list_add(&list,msg);

    msg = (msg_t *)malloc(sizeof(msg_t));
    memset(msg,0,sizeof(msg_t));
    msg->data = (char*)malloc(100);
    memcpy(msg->data,"ABC",3);
    list_add(&list,msg);

    list_for_each(&list, msg_t, data)
    {
        printf("name = %s\n",data->data);
    }

    list_free(&list);

    printf("end.\n");

    return 0;
}


int main(int argc,char *argv[])
{
    return main_list(argc,argv);
}


