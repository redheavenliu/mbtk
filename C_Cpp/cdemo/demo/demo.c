#include "m_stdio.h"
#include "m_util.h"

//typedef unsigned char bool
//typedef unsigned char bool

void*
util_test
(
    void
)
{
    //char *tmp = (char*)malloc(100);
    static char tmp[100];
    memset(tmp,0,100);
    memcpy(tmp,"123",2);
    return tmp;
}


typedef struct
{
    char *key;
    char *value;
}mbtk_http_header_item_s;

typedef struct{
    mbtk_http_header_item_s *header_item[20];
    size_t item_count;
} mbtk_http_request_s,*mbtk_http_request_s_p;

mbtk_http_request_s http_session;

static int
mbtk_http_add_header_item
(
    char* key,
    char* value
)
{

    mbtk_http_header_item_s **tmp_item = http_session.header_item;
    while(*tmp_item){
        if(strcmp((*tmp_item)->key,key) == 0){
            if(strcmp((*tmp_item)->value,value) == 0){
                // Key and value all the same,do nothing.
                return TRUE;
            }else{
                // Should change value.
                break;
            }
        }
        tmp_item++;
    }

    // Should add or change item.
    mbtk_http_header_item_s *item = calloc(1,sizeof(mbtk_http_header_item_s));
    item->key = calloc(strlen(key) + 1,sizeof(char));
    item->value = calloc(strlen(value) + 1,sizeof(char));
    memcpy(item->key,key,strlen(key));
    memcpy(item->value,value,strlen(value));

    if(*tmp_item){// Change item,should free in the first.
        free((*tmp_item)->key);
        free((*tmp_item)->value);
        (*tmp_item)->key = NULL;
        (*tmp_item)->value = NULL;
        free(*tmp_item);
        *tmp_item = NULL;
    }else{
        http_session.item_count++;
    }

    *tmp_item = item;

    return TRUE;
}

static char*
mbtk_http_get_request_head_item_string
(
    void
)
{
    log_i("start.[%d]",10);
    uint16 lenght = 0;
    mbtk_http_header_item_s** tmp = http_session.header_item;
    while(*tmp){
        lenght += (strlen((*tmp)->key) + strlen((*tmp)->value) + 1 + 2);
        tmp++;
    }
    char* result = calloc(lenght + 1,1);
    char* p = result;
    uint16 position = 0;
    tmp = http_session.header_item;
    while(*tmp){
        //lenght += strlen(*http_request_ptr->http_request_head_ptr);
        position += sprintf(p + position,
                            "%s:%s\r\n",
                            (*tmp)->key,
                            (*tmp)->value);
        tmp++;
    }
    return result;
}

void my_printf(void *format,...)
{
    va_list ap;
    va_start(ap,format);

    char *para = va_arg(ap, char*);
    while (para)
    {
        printf("para:%s\n",para);
        para = va_arg(ap, char*);
    }

    va_end(ap);
}

void test()
{
    int result = util_big_endian();

    log(LOG_D,"big endian:%d",result);

    log(LOG_D,"data = %s",(char*)util_test());

    //log_clear();

    bzero(&http_session,sizeof(mbtk_http_request_s));

    mbtk_http_add_header_item("abc1","1111");
    mbtk_http_add_header_item("abc2","2222");
    mbtk_http_add_header_item("abc3","33333");
    mbtk_http_add_header_item("abc4","44444444");
    mbtk_http_add_header_item("abc5","5555555555");
    mbtk_http_add_header_item("abc6","666666666666666666");

    log(LOG_E,"%s",mbtk_http_get_request_head_item_string());

    log(LOG_W,"Changed.");

    log_init(LOG_D, FALSE, "log");

    mbtk_http_add_header_item("abc1","11112222222");
    mbtk_http_add_header_item("abc6","6666111111");

    log(LOG_I,"%s",mbtk_http_get_request_head_item_string());

    log_init(LOG_D, FALSE, "log1");

    mbtk_http_header_item_s **tmp = http_session.header_item;
    while(*tmp){
        free((*tmp)->key);
        free((*tmp)->value);
        (*tmp)->key = NULL;
        (*tmp)->value = NULL;
        free(*tmp);
        *tmp = NULL;
        tmp++;
    }
    bzero(&http_session,sizeof(mbtk_http_request_s));

    mbtk_http_add_header_item("qqqq","3333");
    mbtk_http_add_header_item("aaaaa","44444");

    log(LOG_D,"%s",mbtk_http_get_request_head_item_string());


    my_printf("OK:%s:%s\n","a","b",NULL);
}

static int main_1(int argc,char **argv)
{
    log_init(LOG_D, FALSE, NULL);


    //test();

	log_d("pid = %d",getpid());

	int fd = open("/home/lb/test.h264",O_RDONLY);
	if(fd < 0){
		log_e("open fail[%d].",errno);
		return -1;
	}
	//char buf[2048];
	char *buf = NULL;
	int size = -1;
	int count = 0;

	while((size = read(fd,buf,2048)) > 0){
		if(size > 0){
			count += size;
		}
	}

	log_d("len = %d,%d",count,strlen(buf));

    return EXIT_SUCCESS;
}

#define HEXSTR_TO_INT(len,token) (int)((0xFF & (token)[0]) | (0xFF00 & ((len>1)?((token)[1] << 8):0)) |  (0xFF0000 & ((len>2)?((token)[2] << 16):0)) |  (0xFF000000 & ((len>3)?((token)[3] << 24):0)))

static int main_2(int argc,char **argv)
{
    int a = 460001,b = 460002,c = 460003;


//    int fd = open("/home/lb/data/x",O_RDWR | O_TRUNC | O_CREAT);
    int fd = open("/home/lb/data/x",O_RDONLY);

    if(fd > 0)
    {
//        write(fd,&a,4);
//        write(fd,&b,4);
//        write(fd,&c,4);


        char buffer[20];
        int len = read(fd,buffer,20);
        if(len > 0 && len % 4 == 0)
        {
            int index = 0;
            while(len - index > 0)
            {
                printf("data : %d\n",HEXSTR_TO_INT(4,buffer + index));
                index += 4;
            }
        } else {
            printf("Error data:len - %d\n",len);
        }

        close(fd);
    } else {
        printf("Open fail.[%d]\n",errno);
    }
    return EXIT_SUCCESS;
}

void sock_read_cb_func(int fd, void *buf, size_t nbytes)
{
    printf("Read data[%d]:%s\n",nbytes,buf);
}


#include "mbtk_sock.h"
int main_3(int argc,char **argv)
{
    if(argc != 3) {
        printf("ARG error.\n");
        return 0;
    }

    int fd = mbtk_sock_open(MBTK_SOCK_TYPE_TCP, FALSE, NULL,NULL);
    if(fd > 0){
        printf("open success.\n");
        if(!mbtk_sock_conn(fd, argv[1], atoi(argv[2]))){
            printf("conn success.\n");
        } else {
            printf("conn fail.\n");
        }

        if(write(fd,"test",4) == 4) {
            printf("write success.\n");
        } else {
            printf("write fail.\n");
        }

        if(!mbtk_sock_close(fd)) {
            printf("close success.\n");
        } else {
            printf("close fail.\n");
        }
    } else {
        printf("open fail.\n");
    }
    return 0;
}

int main(int argc,char **argv)
{
//    return main_1(argc,argv);
//    return main_2(argc,argv);
    return main_3(argc,argv);
}

