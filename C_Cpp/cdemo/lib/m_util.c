//#include "m_stdio.h"
#include "m_util.h"
#include "m_stdio.h"

/**
 * Get OS is big or little endian.
 *
 * Return
 *  1  :  Big-endian
 *  0  :  Little-endian
 *  -1 :  Unknown
 */
int util_big_endian()
{
    union
    {
        short s;
        char c[sizeof(short)];
    } un;
    un.s = 0x0102;
    if (sizeof(short) == 2) {
        if (un.c[0] == 1 && un.c[1] == 2)
            return 1;
        else if (un.c[0] == 2 && un.c[1] == 1)
            return 0;
        else
            return -1;
    } else
        return -1;
}

bool cmd_line
(
    const char *cmd,
    char *buf,
    const int buf_size
)
{
    FILE *fcmd;
    fcmd = popen(cmd, "r");
    memset(buf, 0, buf_size);
    if(fcmd)
    {
        fgets(buf, buf_size, fcmd);
        pclose(fcmd);
        return TRUE;
    }
    return FALSE;
}

int pidof(const char *name)
{
    char cmd[100];
    char buffer[20];
    if(name == NULL || strlen(name) == 0)
        return -1;

    int pid = -1;
    memset(buffer,0x0,buffer);
    int size = snprintf(cmd,100,"pidof %s",name);
    cmd[size] = '\0';
    if(cmd_line(cmd,buffer,20))
    {
        if(strlen(buffer) > 0)
            pid = atoi(buffer);
    }

    return pid;
}
