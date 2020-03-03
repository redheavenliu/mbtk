#ifndef M_FILE_H
#define M_FILE_H

#include <sys/stat.h>
#include "m_type.h"

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
					/* default file access permissions for new files */
#define	DIR_MODE	(FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)

/* Little endian
*  1024 -> 0x00040000
*/
int read_int32_l(int fd);

/* Big endian
*  1024 -> 0x00000400
*/
int read_int32_b(int fd);

/* Little endian
*  1024 -> 0x0004
*/
int16 read_int16_l(int fd);

/* Big endian
*  1024 -> 0x0400
*/
int16 read_int16_b(int fd);

#endif /* M_FILE_H */