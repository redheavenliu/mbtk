#include "m_file.h"
#include "m_type.h"

/* Little endian
*  1024 -> 0x00040000
*/
int read_int32_l(int fd)
{
    char buff[4];
    if(read(fd,buff,4) == 4)
    {
        return (int)((0xFF & (buff)[0]) | (0xFF00 & ((buff)[1] << 8)) |  (0xFF0000 & ((buff)[2] << 16)) | (0xFF000000 & ((buff)[3] << 24)));
    } else {
        return -1;
    }
}

/* Big endian
*  1024 -> 0x00000400
*/
int read_int32_b(int fd)
{
    char buff[4];
    if(read(fd,buff,4) == 4)
    {
        return (int)((0xFF & (buff)[3]) | (0xFF00 & ((buff)[2] << 8)) |  (0xFF0000 & ((buff)[1] << 16)) | (0xFF000000 & ((buff)[0] << 24)));
    } else {
        return -1;
    }
}

/* Little endian
*  1024 -> 0x0004
*/
int16 read_int16_l(int fd)
{
    char buff[2];
    if(read(fd,buff,2) == 2)
    {
        return (int16)((0xFF & (buff)[0]) | (0xFF00 & ((buff)[1] << 8)));
    } else {
        return -1;
    }
}

/* Big endian
*  1024 -> 0x0400
*/
int16 read_int16_b(int fd)
{
    char buff[2];
    if(read(fd,buff,2) == 2)
    {
        return (int16)((0xFF & (buff)[1]) | (0xFF00 & ((buff)[0] << 8)));
    } else {
        return -1;
    }
}
