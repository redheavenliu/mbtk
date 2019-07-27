/* Python Automatically generate file. Don't edit. */
#ifndef _MBTK_AT_INFO_H
#define _MBTK_AT_INFO_H
#include "mbtk_at.h"

/* Define AT command index */
typedef enum {
    MBTK_AT_CMD_I,
    MBTK_AT_CMD_S1,
    MBTK_AT_CMD_MTEST2,
    MBTK_AT_CMD_MTEST3,
    MBTK_AT_CMD_MTEST4,
    MBTK_AT_CMD_MTEST5,
    MBTK_AT_CMD_NUM_MAX
} mbtk_at_cmd_ids_enum;

/* Define AT cmd functions */
mbtk_at_state mbtk_at_handle_I(mbtk_at_req_t *req,mbtk_at_rsp_t *rsp);
mbtk_at_state mbtk_at_handle_S1(mbtk_at_req_t *req,mbtk_at_rsp_t *rsp);
mbtk_at_state mbtk_at_handle_MTEST2(mbtk_at_req_t *req,mbtk_at_rsp_t *rsp);
mbtk_at_state mbtk_at_handle_MTEST3(mbtk_at_req_t *req,mbtk_at_rsp_t *rsp);
mbtk_at_state mbtk_at_handle_MTEST4(mbtk_at_req_t *req,mbtk_at_rsp_t *rsp);
mbtk_at_state mbtk_at_handle_MTEST5(mbtk_at_req_t *req,mbtk_at_rsp_t *rsp);

#endif /* _MBTK_AT_INFO_H */
