# coding:utf-8=True
import sys


file_mbtk_config = 'mbtk_at_config'
file_mbtk_at_info_h = 'inc/mbtk_at_info.h'
file_mbtk_at_info_c = 'server/mbtk_at_info.c'
file_at_process_c = 'client/mbtk_at_client.c'


file_info_h_head = """/* Python Automatically generate file. Don't edit. */
#ifndef _MBTK_AT_INFO_H
#define _MBTK_AT_INFO_H
#include "mbtk_at.h"

"""

file_info_h_tail = """
#endif /* _MBTK_AT_INFO_H */
"""

file_info_c_head = """/* Python Automatically generate file. Don't edit. */
#include "mbtk_at_def.h"
#include "mbtk_at.h"
#include "mbtk_at_info.h"
"""

file_info_c_tail = """"""

at_process_func_comment = """
/*
* AT{} : {}
* Support function:{}
*/
"""

at_process_func_body = """{{
    mbtk_at_state result = MBTKAT_OK;
    switch(req->type)
    {{{}
        default:
        {{
            LOGE("Unknow AT type.");
            result = MBTKAT_ERROR;
        }}
    }}
    return result;
}}
"""

at_process_func_body_case = """
        case MBTKAT_TYPE_{}:
        {{

            break;
        }}"""


def file_info_h_write(line_strs):
    print "Start write file:mbtk_at_info.h"
    file_info_h = open(file_mbtk_at_info_h, 'w')
    file_info_h.write(file_info_h_head)

    # mbtk_at_cmd_ids_enum
    file_info_h.write('/* Define AT command index */\n')
    file_info_h.write('typedef enum {\n')
    for line in line_strs:
        if line[0][0].isupper():
            file_info_h.write('    MBTK_AT_CMD_{},\n'.format(line[0]))
        else:
            file_info_h.write('    MBTK_AT_CMD_{},\n'.format(line[0][1:]))

    file_info_h.write('    MBTK_AT_CMD_NUM_MAX\n} mbtk_at_cmd_ids_enum;\n\n')

    # AT function
    file_info_h.write('/* Define AT cmd functions */\n')
    for line in line_strs:
        if line[0][0].isupper():
            file_info_h.write('mbtk_at_state mbtk_at_handle_{}(mbtk_at_req_t *req,mbtk_at_rsp_t *rsp);\n'
                              .format(line[0]))
        else:
            file_info_h.write('mbtk_at_state mbtk_at_handle_{}(mbtk_at_req_t *req,mbtk_at_rsp_t *rsp);\n'
                              .format(line[0][1:]))

    file_info_h.write(file_info_h_tail)
    file_info_h.close()


def file_info_c_write(line_strs):
    print "Start write file:mbtk_at_info.c"
    file_info_c = open(file_mbtk_at_info_c, 'w')
    file_info_c.write(file_info_c_head)

    for line in line_strs:
        # Write AT Name
        if line[0][0].isupper():
            at_name = line[0]
        else:
            at_name = line[0][1:]
        file_info_c.write("\n// Define for AT{}\n".format(line[0]))
        file_info_c.write('const MBTKAT_PARAM_STRING_T    mbtkat_cmd_name_{}[] =\n{{\n    '
                          '{{ {}, (const uint8 *)"{}" }}\n}};\n'
                          .format(at_name, len(line[0]), line[0]))
        # Write numeric/string params
        for arg_index in range(2, len(line)):
            arg_range = line[arg_index][1:-1].split(' ')
            # print arg_range
            if len(arg_range) == 1 and arg_range[0][0] == 's':  # string
                file_info_c.write('const MBTKAT_PARAM_STRING_T   mbtkat_param_string_{}_{}[] '
                                  '=\n{{\n    {{    {},   NULL }}\n}};\n'
                                  .format(at_name, arg_index - 1, arg_range[0][1:]))
            else:   # numeric
                file_info_c.write('const MBTKAT_PARAM_NUMERIC_T   mbtkat_param_numeric_{}_{}[] =\n{{\n'
                                  .format(at_name, arg_index - 1))
                for arg in arg_range:
                    ranges = str(arg).split('-')
                    # print ranges
                    if len(ranges) == 1:
                        if arg is arg_range[len(arg_range) - 1]:    # Is last
                            file_info_c.write('    {{    {},    {} }}\n'.format(ranges[0], ranges[0]))
                        else:
                            file_info_c.write('    {{    {},    {} }},\n'.format(ranges[0], ranges[0]))
                    elif len(ranges) == 2:
                        if arg is arg_range[len(arg_range) - 1]:    # Is last
                            file_info_c.write('    {{    {},    {} }}\n'.format(ranges[0], ranges[1]))
                        else:
                            file_info_c.write('    {{    {},    {} }},\n'.format(ranges[0], ranges[1]))
                    else:
                        print "ARG ranges error:{}".format(arg)
                file_info_c.write('};\n')

        # Write params info struct
        file_info_c.write('const MBTKAT_PARAM_INFO_T mbtkat_param_info_{}[] =\n{{\n'.format(at_name))
        for arg_index in range(2, len(line)):
            arg_range = line[arg_index][1:-1].split(' ')
            # print arg_range
            if len(arg_range) == 1 and arg_range[0][0] == 's':  # string
                if arg_index == len(line) - 1:  # Is last
                    file_info_c.write('    {{  2,  1, .param_info.str_array_ptr = mbtkat_param_string_{}_{} }}\n'
                                      .format(at_name, arg_index - 1))
                else:
                    file_info_c.write('    {{  2,  1, .param_info.str_array_ptr = mbtkat_param_string_{}_{} }},\n'
                                      .format(at_name, arg_index - 1))
            else:   # numeric
                if arg_index == len(line) - 1:  # Is last
                    file_info_c.write('    {{  1,  {}, .param_info.num_array_ptr = mbtkat_param_numeric_{}_{} }}\n'
                                      .format(len(arg_range), at_name, arg_index - 1))
                else:
                    file_info_c.write('    {{  1,  {}, .param_info.num_array_ptr = mbtkat_param_numeric_{}_{} }},\n'
                                      .format(len(arg_range), at_name, arg_index - 1))
        file_info_c.write('};\n')

    # Write mbtkat_info_tables
    file_info_c.write('\n\n// Define mbtkat_info_tables\n')
    file_info_c.write('const MBTKAT_INFO_T mbtkat_info_tables[] =\n{')
    for line in line_strs:
        # print line
        if line[0][0].isupper():
            at_name = line[0]
        else:
            at_name = line[0][1:]
        if line is line_strs[len(line_strs) - 1]:   # Is last
            file_info_c.write('    {{MBTK_AT_CMD_{}, {}, mbtkat_cmd_name_{}, {},'
                              ' mbtkat_param_info_{}, mbtk_at_handle_{}}}\n'
                              .format(at_name, line[1], at_name, len(line) - 2, at_name, at_name))
        else:
            file_info_c.write('    {{MBTK_AT_CMD_{}, {}, mbtkat_cmd_name_{}, {},'
                              ' mbtkat_param_info_{}, mbtk_at_handle_{}}},\n'
                              .format(at_name, line[1], at_name, len(line) - 2, at_name, at_name))

    file_info_c.write('};\n')
    file_info_c.write(file_info_c_tail)
    file_info_c.close()


def at_arg_format(args):
    result = ''
    for arg in args:
        result += arg
    return result

def at_type_format(type):
    result = ''
    if type & 0x01:
        result += '/EXE'
    if type & 0x02:
        result += '/SET'
    if type & 0x04:
        result += '/READ'
    if type & 0x08:
        result += '/TEST'
    return result[1:]


def at_process_body_case_create(type):
    result = ''
    if type & 0x01:
        result += at_process_func_body_case.format('EXE')
    if type & 0x02:
        result += at_process_func_body_case.format('SET')
    if type & 0x04:
        result += at_process_func_body_case.format('READ')
    if type & 0x08:
        result += at_process_func_body_case.format('TEST')
    return result


def file_at_process_write(line_strs):
    print "Start write file:mbtk_at_client.c"
    file_at = open(file_at_process_c, 'r+')
    for line in line_strs:
        if line[0][0].isupper():
            at_name = line[0]
        else:
            at_name = line[0][1:]
        func_name = 'mbtk_at_state mbtk_at_handle_{}(mbtk_at_req_t *req,mbtk_at_rsp_t *rsp)\n'.format(at_name)
        file_at.seek(0)
        found_func = False
        for read_line in file_at:
            # print 'line:{}'.format(read_line)
            # print 'func:{}'.format(func_name)
            if read_line == func_name:
                found_func = True
                break

        if not found_func:
            file_at.write(at_process_func_comment.format(line[0], at_arg_format(line[2:]), at_type_format(int(line[1]))))
            file_at.write(func_name)
            file_at.write(at_process_func_body.format(at_process_body_case_create(int(line[1]))))


def is_int(s):
    try:
        int(s)
        return True
    except ValueError:
        pass
    return False


def at_define_check(at_array):
    # print at_array
    at_sets = set()
    for at in at_array:
        # print at
        if len(at) < 2:
            print 'ERROR:{}'.format(at)
            return False
        if at[0][0] != '+' and not at[0][0].isupper():
            print 'ERROR:{}'.format(at)
            return False
        if at[0][0] == '+':
            if len(at[0]) <= 1 or not at[0][1].isupper():
                print 'ERROR:{}'.format(at)
                return False
        for ch in at[0][1:]:
            if not ch.isupper() and not ch.isdigit():
                print 'ERROR:{}'.format(at)
                return False
        if int(at[1]) < 1 or int(at[1]) > 15:
            print 'ERROR:{}'.format(at)
            return False
        if int(at[1]) & 0x02:   # Support set cmd
            if len(at) <= 2:
                print 'ERROR:{}'.format(at)
                return False
            else:
                for arg in at[2:]:
                    if len(arg) <= 2:
                        print 'ERROR:{}'.format(at)
                        return False
                    if arg[0] != '(' or arg[-1] != ')':
                        print 'ERROR:{}'.format(at)
                        return False
                    if arg[1] == 's':   # String
                        if ' ' in arg or not is_int(arg[2:-1]):
                            print 'ERROR:{}'.format(at)
                            return False
                    else:   # Numeric
                        for range in arg[1:-1].split(' '):
                            if ' ' in range:
                                print 'ERROR:{}'.format(at)
                                return False
                            else:
                                range_items = range.split('-')
                                if len(range_items) != 1 and len(range_items) != 2:
                                    print 'ERROR:{}'.format(at)
                                    return False
                                for range_item in range_items:
                                    if ' ' in range_item or not is_int(range_item):
                                        print 'ERROR:{}'.format(at)
                                        return False
                                if len(range_items) == 2 and int(range_items[0]) > int(range_items[1]):
                                    print 'ERROR:{}'.format(at)
                                    return False
        else:
            if len(at) != 2:
                print 'ERROR:{}'.format(at)
                return False
        if at[0][0].isupper():
            at_sets.add(at[0])
        else:
            at_sets.add(at[0][1:])
    if len(at_sets) != len(at_array):
        print 'AT name same.'
        return False

    return True


if __name__ == '__main__':
    print "Start generate MBTK_AT files."

    file_lines = []
    f_config = open(file_mbtk_config)
    for line in f_config:
        if line[0] != '#' and line[0] != '\n':
            file_lines.append(line[0:-1].split(','))

    if at_define_check(file_lines):
        file_info_h_write(file_lines)
        file_info_c_write(file_lines)
        file_at_process_write(file_lines)
        print "Success generate files."
    else:
        print 'AT define error.'

    f_config.close()

