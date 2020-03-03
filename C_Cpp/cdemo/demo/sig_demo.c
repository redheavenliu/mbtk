#include "m_stdio.h"

typedef struct
{
    int id;
    char name[20];
} person_struct;

void sig_process(int sig)
{
    printf("I got signal %d\n", sig);
    switch(sig)
    {
        case SIGINT: // Ctrl + C
        {
            printf("Exit by SIGINT.\n");
            exit(0);
        }
        case SIGQUIT: // Ctrl + \ (���� SIGINT ����Ҫ����core�ļ�)
        {
            printf("Exit by SIGQUIT.\n");
            exit(0);
        }
        case SIGTERM:// Ĭ��kill   (ͬ SIGKILL ���� SIGKILL ���ɲ���)
        {
            printf("Exit by SIGTERM.\n");
            exit(0);
        }
        case SIGTSTP:// Ctrl + Z (ͬ SIGSTOP ���� SIGSTOP ���ɲ���)
        {
            printf("Exit by SIGTSTP.\n");
            exit(0);
        }
        case SIGSEGV: // ���ָ��
        {
            printf("Exit by SIGSEGV.\n");
            exit(0);
        }
        default:
        {
            printf("Unknown sig:%d\n",sig);
        }
    }
}

int main(int argc,char *argv[])
{
    signal(SIGINT, sig_process);
    //signal(SIGQUIT, sig_process);
    signal(SIGTERM, sig_process);
    signal(SIGTSTP, sig_process);
    //signal(SIGSEGV, sig_process);

    person_struct *person = NULL;

    /*person = (person_struct*)malloc(sizeof(person_struct));
    memset(person,0x0,sizeof(person_struct));*/
    person->id = 1;
    memcpy(person->name,"XXX",3);
    printf("Person : id(%d),name(%s)\n",person->id,person->name);

    while(1)
    {
        printf("hello world...\n");
        sleep(1);
    }

    return 0;
}
