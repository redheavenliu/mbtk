#include "m_stdio.h"
#include <pthread.h>

static pid_t lynq_pppd_pid = 0;

bool kill_process(const char *process,int sig)
{
    if(process == NULL || strlen(process) == 0)
        return FALSE;

    int count = 0;
    kill(pidof(process),sig);
    while(pidof(process) > 0 && count < 10)
    {
        usleep(300000);
        count++;
    }

    if(count == 10)
    {
        printf("Kill %s fail.\n",process);
        return FALSE;
    }
    else
    {
        printf("Kill %s success.\n",process);
        return TRUE;
    }
}

void sig_process(int sig)
{
    printf("I got signal %d\n", sig);
    switch(sig)
    {
        case SIGINT: // Ctrl + C
        {
            printf("Exit by SIGINT.\n");
            kill_process("pppd",SIGINT);
            sleep(3);
            //exit(0);
        }
        case SIGQUIT: // Ctrl + \ (类似 SIGINT ，但要产生core文件)
        {
            printf("Exit by SIGQUIT.\n");
            kill_process("pppd",SIGQUIT);
            exit(0);
        }
        case SIGTERM:// 默认kill   (同 SIGKILL ，但 SIGKILL 不可捕获)
        {
            printf("Exit by SIGTERM.\n");
            kill_process("pppd",SIGTERM);
            exit(0);
        }
        default:
        {
            printf("Unknown sig:%d\n",sig);
        }
    }
}


static void* pppd_thread_function(void*  arg) {
    char **argvv = (char **)arg;
    while (TRUE) {
        pid_t child_pid;
        char serialdevname[32];
        memset(serialdevname,0,32);
        snprintf(serialdevname,32,"/dev/ttyUSB%s",argvv[1]);
        printf("%s\n",serialdevname);

        child_pid = fork();
        if (0 == child_pid) { //this is the child_process
//            if (system("pppd call myppp")) {
//                printf("cannot execve: %s\n", strerror(errno));
//                exit(errno);
//            }
//            exit(0);

            int argc = 0;
            const char *argv[40] = {"pppd", "115200", "nodetach", "nolock", "debug", "dump", "nocrtscts", "modem", "hide-password",
                "usepeerdns", "noipdefault", "novj", "novjccomp", "noccp", "defaultroute", "ipcp-accept-local", "ipcp-accept-remote", "ipcp-max-failure", "30",
                NULL
            };

            while (argv[argc]) argc++;
            argv[argc++] = serialdevname;

            argv[argc++] = "local";
            argv[argc++] = "connect";
            argv[argc++] = "''/usr/sbin/chat -s -v ABORT BUSY ABORT \"NO CARRIER\" ABORT \"NO DIALTONE\" ABORT ERROR ABORT \"NO ANSWER\" TIMEOUT 20 \"\" ATD*99# CONNECT''";
            argv[argc] = NULL;

            if (execv("/usr/sbin/pppd", (char**) argv)) {
                printf("cannot execve('%s'): %s\n", argv[0], strerror(errno));
                exit(errno);
            }
            exit(0);
        } else if (child_pid < 0) {
            printf("failed to start ('%s'): %s\n", "pppd", strerror(errno));
            break;
        } else {
            int status, retval = 0;
            lynq_pppd_pid = child_pid;
            printf("waitpid() : %d\n",child_pid);
            waitpid(child_pid, &status, 0);
            printf("waitpid() end.[status = %d]\n",status);
            lynq_pppd_pid = 0;
            if (WIFSIGNALED(status)) {
                retval = WTERMSIG(status);
                printf("*** %s: Killed by signal %d retval = %d\n", "pppd", WTERMSIG(status), retval);
            } else if (WIFEXITED(status) && WEXITSTATUS(status) > 0) {
                retval = WEXITSTATUS(status);
                printf("*** %s: Exit code %d retval = %d\n", "pppd", WEXITSTATUS(status), retval);
            }
            break;
        }
    }

    lynq_pppd_pid = 0;
    printf("%s exit\n", __func__);
    pthread_exit(NULL);
    return NULL;
}

int main(int argc,char *argv[])
{

    signal(SIGINT, sig_process);
    signal(SIGQUIT, sig_process);
    signal(SIGTERM, sig_process);

    static pthread_attr_t thread_attr;
    pthread_attr_init(&thread_attr);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
    if (pthread_create(&lynq_pppd_pid, &thread_attr, pppd_thread_function, argv)!=0) {
        printf("%s %s errno: %d (%s)", __FILE__, __func__, errno, strerror(errno));
        return -1;
    }
    pthread_attr_destroy(&thread_attr); /* Not strictly necessary */

    sleep(3);

    while(lynq_pppd_pid)
    {
        sleep(1);
    }

    printf("End.\n");
}


