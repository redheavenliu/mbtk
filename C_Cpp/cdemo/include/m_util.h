#ifndef M_UTIL_H
#define M_UTIL_H

#include "m_stdio.h"

/**
 * Get OS is big or little endian.
 *
 * Return
 *  1  :  Big-endian
 *  0  :  Little-endian
 *  -1 :  Unknown
 */
int util_big_endian();

bool cmd_line
(
    const char *cmd,
    char *buf,
    const int buf_size
);

int pidof(const char *name);

#endif /* M_UTIL_H */

