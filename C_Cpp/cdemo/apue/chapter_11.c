#include "m_stdio.h"
#include <pthread.h>

typedef struct
{
	int data1;
	int data2;
} data_demo_2;


static void
demo_3_clean_fun
(
	void *arg
)
{
	log_d("Thread clean : %s",(char*)arg);
}


static void*
demo_3_fun
(
	void *arg
)
{
	log_d("Thread %ld start.",pthread_self());
	pthread_cleanup_push(demo_3_clean_fun, "clean 1");
	pthread_cleanup_push(demo_3_clean_fun, "clean 2");

	// return ((void*)100); // ������ô�����

	pthread_exit((void*)100); // ����ô�����

	// ������ã�������벻��
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);
}

static void
demo_3
(
	void
)
{
	log_d("start");
	pthread_t tid1;
	int err = pthread_create(&tid1,NULL,demo_3_fun,NULL);
	if(err != 0){
		log_e("pthread_creat fail(%d)",err);
		return;
	}

	void *data;
	err = pthread_join(tid1,&data); // ������ tid1 ����
	if(err != 0){
		log_e("pthrad_join fail(%d)",err);
		return;
	}
	log_d("Thread %ld end.",tid1);
	log_d("data = %d",(long)data);

	log_d("end");
}


static void*
demo_2_fun
(
	void *arg
)
{
	log_d("Thread %ld start.",pthread_self());
// �߳��еľֲ��������߳̽��������Ч
//	data_demo_2 data = {10,20};
//	log_d("data = {%d,%d}",data.data1,data.data2);
//	sleep(1);
//	log_d("Thread %ld end.",pthread_self());
//	return ((void*)&data);

	data_demo_2 *data = (data_demo_2*)malloc(sizeof(data_demo_2));
	data->data1 = 10;
	data->data2 = 20;
	log_d("data = {%d,%d}",data->data1,data->data2);
	sleep(1);
	log_d("Thread %ld end.",pthread_self());
	return ((void*)data);
}


static void
demo_2
(
	void
)
{
	log_d("start");
	pthread_t tid1;
	int err = pthread_create(&tid1,NULL,demo_2_fun,NULL);
	if(err != 0){
		log_e("pthread_creat fail(%d)",err);
		return;
	}

	data_demo_2 *data;
	err = pthread_join(tid1,&data); // ������ tid1 ����
	if(err != 0){
		log_e("pthrad_join fail(%d)",err);
		return;
	}
	log_d("Thread %ld end.",tid1);
	log_d("data = [%d,%d]",data->data1,data->data2);

	log_d("end");
}


static void*
demo_1_fun
(
	void *arg
)
{
	log_d("Thread %ld start.",pthread_self());
	sleep(1);
	return ((void*)100);
}


static void
demo_1
(
	void
)
{
	log_d("start");
	pthread_t tid1;
	int err = pthread_create(&tid1,NULL,demo_1_fun,NULL);
	if(err != 0){
		log_e("pthread_creat fail(%d)",err);
		return;
	}

	void *vret;
	err = pthread_join(tid1,&vret); // ������ tid1 ����
	if(err != 0){
		log_e("pthrad_join fail(%d)",err);
		return;
	}
	log_d("Thread %ld end.Code is %ld",tid1,(long)vret);

	log_d("end");
}


int main(int argc,char **argv)
{
	log_init(LOG_D,FALSE,NULL);
    //demo_1();
    //demo_2();
    demo_3();
    return 0;
}

