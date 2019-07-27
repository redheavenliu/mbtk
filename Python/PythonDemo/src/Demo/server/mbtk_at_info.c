/* Python Automatically generate file. Don't edit. */
#include "mbtk_at_def.h"
#include "mbtk_at.h"
#include "mbtk_at_info.h"

// Define for ATI
const MBTKAT_PARAM_STRING_T    mbtkat_cmd_name_I[] =
{
    { 1, (const uint8 *)"I" }
};
const MBTKAT_PARAM_INFO_T mbtkat_param_info_I[] =
{
};

// Define for AT+S1
const MBTKAT_PARAM_STRING_T    mbtkat_cmd_name_S1[] =
{
    { 3, (const uint8 *)"+S1" }
};
const MBTKAT_PARAM_NUMERIC_T   mbtkat_param_numeric_S1_1[] =
{
    {    0,    0 },
    {    1,    1 }
};
const MBTKAT_PARAM_NUMERIC_T   mbtkat_param_numeric_S1_2[] =
{
    {    1,    10 }
};
const MBTKAT_PARAM_STRING_T   mbtkat_param_string_S1_3[] =
{
    {    10,   NULL }
};
const MBTKAT_PARAM_INFO_T mbtkat_param_info_S1[] =
{
    {  1,  2, .param_info.num_array_ptr = mbtkat_param_numeric_S1_1 },
    {  1,  1, .param_info.num_array_ptr = mbtkat_param_numeric_S1_2 },
    {  2,  1, .param_info.str_array_ptr = mbtkat_param_string_S1_3 }
};

// Define for AT+MTEST2
const MBTKAT_PARAM_STRING_T    mbtkat_cmd_name_MTEST2[] =
{
    { 7, (const uint8 *)"+MTEST2" }
};
const MBTKAT_PARAM_NUMERIC_T   mbtkat_param_numeric_MTEST2_1[] =
{
    {    0,    0 },
    {    1,    1 },
    {    5,    5 }
};
const MBTKAT_PARAM_STRING_T   mbtkat_param_string_MTEST2_2[] =
{
    {    100,   NULL }
};
const MBTKAT_PARAM_STRING_T   mbtkat_param_string_MTEST2_3[] =
{
    {    10,   NULL }
};
const MBTKAT_PARAM_NUMERIC_T   mbtkat_param_numeric_MTEST2_4[] =
{
    {    1,    10 },
    {    100,    200 },
    {    300,    300 }
};
const MBTKAT_PARAM_INFO_T mbtkat_param_info_MTEST2[] =
{
    {  1,  3, .param_info.num_array_ptr = mbtkat_param_numeric_MTEST2_1 },
    {  2,  1, .param_info.str_array_ptr = mbtkat_param_string_MTEST2_2 },
    {  2,  1, .param_info.str_array_ptr = mbtkat_param_string_MTEST2_3 },
    {  1,  3, .param_info.num_array_ptr = mbtkat_param_numeric_MTEST2_4 }
};

// Define for AT+MTEST3
const MBTKAT_PARAM_STRING_T    mbtkat_cmd_name_MTEST3[] =
{
    { 7, (const uint8 *)"+MTEST3" }
};
const MBTKAT_PARAM_NUMERIC_T   mbtkat_param_numeric_MTEST3_1[] =
{
    {    123,    123 }
};
const MBTKAT_PARAM_INFO_T mbtkat_param_info_MTEST3[] =
{
    {  1,  1, .param_info.num_array_ptr = mbtkat_param_numeric_MTEST3_1 }
};

// Define for AT+MTEST4
const MBTKAT_PARAM_STRING_T    mbtkat_cmd_name_MTEST4[] =
{
    { 7, (const uint8 *)"+MTEST4" }
};
const MBTKAT_PARAM_NUMERIC_T   mbtkat_param_numeric_MTEST4_1[] =
{
    {    1,    1 },
    {    3,    3 },
    {    5,    5 }
};
const MBTKAT_PARAM_STRING_T   mbtkat_param_string_MTEST4_2[] =
{
    {    5,   NULL }
};
const MBTKAT_PARAM_NUMERIC_T   mbtkat_param_numeric_MTEST4_3[] =
{
    {    4,    20 }
};
const MBTKAT_PARAM_INFO_T mbtkat_param_info_MTEST4[] =
{
    {  1,  3, .param_info.num_array_ptr = mbtkat_param_numeric_MTEST4_1 },
    {  2,  1, .param_info.str_array_ptr = mbtkat_param_string_MTEST4_2 },
    {  1,  1, .param_info.num_array_ptr = mbtkat_param_numeric_MTEST4_3 }
};

// Define for AT+MTEST5
const MBTKAT_PARAM_STRING_T    mbtkat_cmd_name_MTEST5[] =
{
    { 7, (const uint8 *)"+MTEST5" }
};
const MBTKAT_PARAM_NUMERIC_T   mbtkat_param_numeric_MTEST5_1[] =
{
    {    1,    1 },
    {    3,    3 },
    {    5,    5 }
};
const MBTKAT_PARAM_STRING_T   mbtkat_param_string_MTEST5_2[] =
{
    {    5,   NULL }
};
const MBTKAT_PARAM_NUMERIC_T   mbtkat_param_numeric_MTEST5_3[] =
{
    {    4,    20 }
};
const MBTKAT_PARAM_INFO_T mbtkat_param_info_MTEST5[] =
{
    {  1,  3, .param_info.num_array_ptr = mbtkat_param_numeric_MTEST5_1 },
    {  2,  1, .param_info.str_array_ptr = mbtkat_param_string_MTEST5_2 },
    {  1,  1, .param_info.num_array_ptr = mbtkat_param_numeric_MTEST5_3 }
};


// Define mbtkat_info_tables
const MBTKAT_INFO_T mbtkat_info_tables[] =
    {MBTK_AT_CMD_I, 1, mbtkat_cmd_name_I, 0, mbtkat_param_info_I, mbtk_at_handle_I},
    {MBTK_AT_CMD_S1, 15, mbtkat_cmd_name_S1, 3, mbtkat_param_info_S1, mbtk_at_handle_S1},
    {MBTK_AT_CMD_MTEST2, 2, mbtkat_cmd_name_MTEST2, 4, mbtkat_param_info_MTEST2, mbtk_at_handle_MTEST2},
    {MBTK_AT_CMD_MTEST3, 10, mbtkat_cmd_name_MTEST3, 1, mbtkat_param_info_MTEST3, mbtk_at_handle_MTEST3},
    {MBTK_AT_CMD_MTEST4, 6, mbtkat_cmd_name_MTEST4, 3, mbtkat_param_info_MTEST4, mbtk_at_handle_MTEST4},
    {MBTK_AT_CMD_MTEST5, 6, mbtkat_cmd_name_MTEST5, 3, mbtkat_param_info_MTEST5, mbtk_at_handle_MTEST5}
};
