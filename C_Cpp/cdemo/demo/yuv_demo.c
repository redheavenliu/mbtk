#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "m_log.h"
#include "m_type.h"

typedef enum {
    YUV_TYPE_I420 = 0,  // Default
    YUV_TYPE_YU12,
    YUV_TYPE_NV12,
    YUV_TYPE_NV21,
    YUV_TYPE_420_MAX,
    YUV_TYPE_422_MAX
} yuv_type_enum;


static int pic_bmp_save(const uint8 *pic_path,uint32 width, uint32 height,
    const uint8 *data, uint32 data_len)
{
    if(data_len != width * height * 3) {
        LOGE("Data lenght error[width-%d,height-%d,len-%d].",width,height,data_len);
        return -1;
    }

    unsigned char header[54] = {
        0x42, 0x4d,      //WORD  bfType----------- [0,1]
        0, 0, 0, 0,      //DWORD bfSize----------- [2,3,4,5]
        0, 0,            //WORD  bfReserved1------ [6,7]
        0, 0,            //WORD  bfReserved2------ [8,9]
        54, 0, 0, 0,     //WORD  bfOffBits-------- [10,11,12,13]

        40, 0, 0, 0,     //DWORD biSize----------- [14,15,16,17]
        0, 0, 0, 0,      //LONG  biWidth---------- [18,19,20,21]
        0, 0, 0, 0,      //LONG  biHeight--------- [22,23,24,25]
        1, 0,            //WORD  biplanes--------- [26,27]
        24, 0,           //WORD  biCount---------- [28,29]
        0, 0, 0, 0,      //DWORD biCompression---- [30,31,32,33]
        0, 0, 0, 0,      //DWORD biSizeImage------ [34,35,36,37]
        0, 0, 0, 0,      //LONG  biXPelsPerMeter-- [38,39,40,41]
        0, 0, 0, 0,      //LONG  biYPelsPerMeter-- [42,43,44,45]
        0, 0, 0, 0,      //DWORD biClrUsed-------- [46,47,48,49]
        0, 0, 0, 0       //DWORD biClrImportant--- [50,51,52,53]
    };

    long file_size = (long)width * (long)height * 3 + 54;
    header[2] = (unsigned char)(file_size & 0x000000ff);
    header[3] = (file_size >> 8) & 0x000000ff;
    header[4] = (file_size >> 16) & 0x000000ff;
    header[5] = (file_size >> 24) & 0x000000ff;

    header[18] = width & 0x000000ff;
    header[19] = (width >> 8) & 0x000000ff;
    header[20] = (width >> 16) & 0x000000ff;
    header[21] = (width >> 24) & 0x000000ff;

    header[22] = height & 0x000000ff;
    header[23] = (height >> 8) & 0x000000ff;
    header[24] = (height >> 16) & 0x000000ff;
    header[25] = (height >> 24) & 0x000000ff;

    FILE *pic_file = NULL;
    if (!(pic_file = fopen(pic_path, "wb"))) {
        LOGE("fopen() fail[%d].",errno);
        return -1;
    }

    fwrite(header, sizeof(uint8), 54, pic_file);
    fwrite(data, sizeof(uint8), data_len, pic_file);
    fclose(pic_file);
    return 0;
}


static int yuv_data_parse(const uint8 *data,uint32 data_len,
    yuv_type_enum type,uint32 width,uint32 height,
    uint8 *y_data,uint32 *y_data_len,
    uint8 *u_data,uint32 *u_data_len,
    uint8 *v_data,uint32 *v_data_len)
{
    const uint32 width_height = width * height;
    if(type < YUV_TYPE_420_MAX) { // YUV420
        // Y
        memcpy(y_data,data,width_height);
        *y_data_len = width_height;

        switch(type) {
            case YUV_TYPE_I420:
            {
                // U
                memcpy(u_data,data + width_height,width_height / 4);
                *u_data_len = width_height / 4;

                // V
                memcpy(v_data,data + width_height * 5 / 4,width_height / 4);
                *v_data_len = width_height / 4;
                break;
            }
            case YUV_TYPE_YU12:
            {
                // U
                memcpy(u_data,data + width_height * 5 / 4,width_height / 4);
                *u_data_len = width_height / 4;

                // V
                memcpy(v_data,data + width_height,width_height / 4);
                *v_data_len = width_height / 4;
                break;
            }
            case YUV_TYPE_NV12:
            {
                const uint8 *data_ptr = data + width_height;
                uint32 index = 0;
                while(index < width_height / 2) {
                    *(u_data + index++) = *data_ptr++;
                    *(v_data + index++) = *data_ptr++;
                }
                *u_data_len = width_height / 4;
                *v_data_len = width_height / 4;
                break;
            }
            case YUV_TYPE_NV21:
            {
                const uint8 *data_ptr = data + width_height;
                uint32 index = 0;
                while(index < width_height / 2) {
                    *(v_data + index++) = *data_ptr++;
                    *(u_data + index++) = *data_ptr++;
                }
                *u_data_len = width_height / 4;
                *v_data_len = width_height / 4;
                break;
            }
            default:
                LOGW("No such yuv type.[type-%d]",type);
                return -1;
        }
    } else if(type < YUV_TYPE_422_MAX) { // YUV422

    } else {
        LOGE("Unknown type:%d",type);
        return -1;
    }
    return 0;
}

static void yuv420_to_rgb(yuv_type_enum type,uint32 width, uint32 height, uint8 *src,
                uint8 *dst) {
    uint32 frameSize = width * height;
    uint32 j = 0, yp = 0, i = 0;
    unsigned char *yuv420sp = src;
    uint32 uv_size = frameSize / 4;
    int r_temp,g_temp,b_temp;
    int uvp, u = 0, v = 0;

    for (j = 0, yp = 0; j < height; j++) {
        if(type == YUV_TYPE_I420 || type == YUV_TYPE_YU12) {
            uvp = frameSize + (j >> 1) * (width >> 1);
        } else if(type == YUV_TYPE_NV12 || type == YUV_TYPE_NV21) {
            uvp = frameSize + (j >> 1) * width;
        } else {
            LOGE("YUV type error.");
            return;
        }

        for (i = 0; i < width; i++, yp++) {
            int y = (0xff & ((int)yuv420sp[yp])) - 16;
            if (y < 0)
                y = 0;

            int y1192 = 1192 * y;

            if ((i & 1) == 0) {
                if(type == YUV_TYPE_I420 || type == YUV_TYPE_YU12) {
                    if(type == YUV_TYPE_I420) {
                        u = (0xff & yuv420sp[uvp]) - 128;
                        v = (0xff & yuv420sp[uvp + uv_size]) - 128;
                    } else {
                        v = (0xff & yuv420sp[uvp]) - 128;
                        u = (0xff & yuv420sp[uvp + uv_size]) - 128;
                    }

                    uvp++;
                } else if(type == YUV_TYPE_NV12 || type == YUV_TYPE_NV21) {
                    if(type == YUV_TYPE_NV12) {
                        u = (0xff & yuv420sp[uvp++]) - 128;
                        v = (0xff & yuv420sp[uvp++]) - 128;
                    } else {
                        v = (0xff & yuv420sp[uvp++]) - 128;
                        u = (0xff & yuv420sp[uvp++]) - 128;
                    }
                } else {
                    LOGE("YUV type error.");
                    return;
                }
                r_temp = 1634 * v;
                g_temp = 833 * v + 400 * u;
                b_temp = 2066 * u;
            }

            int r = (y1192 + r_temp);
            int g = (y1192 - g_temp);
            int b = (y1192 + b_temp);

            if (r < 0)
                r = 0;
            else if (r > 262143)
                r = 262143;
            if (g < 0)
                g = 0;
            else if (g > 262143)
                g = 262143;
            if (b < 0)
                b = 0;
            else if (b > 262143)
                b = 262143;

            dst[yp * 3] = ((r << 6) & 0xff0000) >> 16;
            dst[yp * 3 + 1] = ((g >> 2) & 0xff00) >> 8;
            dst[yp * 3 + 2] = ((b >> 10) & 0xff);
        }
    }
}

#if 0
static void yuv420p_to_rgb(uint32 width, uint32 height, uint8 *src,
                uint8 *dst) {
    uint32 frameSize = width * height;
    uint32 j = 0, yp = 0, i = 0;
    unsigned char *yuv420sp = src;
    uint32 uv_size = frameSize / 4;

    for (j = 0, yp = 0; j < height; j++) {
        int uvp = frameSize + (j >> 1) * (width >> 1), u = 0, v = 0;
        for (i = 0; i < width; i++, yp++) {
            int y = (0xff & ((int)yuv420sp[yp])) - 16;
            if (y < 0)
                y = 0;
            if ((i & 1) == 0) {
                u = (0xff & yuv420sp[uvp]) - 128;
                v = (0xff & yuv420sp[uvp + uv_size]) - 128;

                uvp++;
            }

            int y1192 = 1192 * y;
            int r = (y1192 + 1634 * v);
            int g = (y1192 - 833 * v - 400 * u);
            int b = (y1192 + 2066 * u);

            if (r < 0)
                r = 0;
            else if (r > 262143)
                r = 262143;
            if (g < 0)
                g = 0;
            else if (g > 262143)
                g = 262143;
            if (b < 0)
                b = 0;
            else if (b > 262143)
                b = 262143;

            dst[yp * 3] = ((r << 6) & 0xff0000) >> 16;
            dst[yp * 3 + 1] = ((g >> 2) & 0xff00) >> 8;
            dst[yp * 3 + 2] = ((b >> 10) & 0xff);
        }
    }
}
#endif

static int yuv_2_rgb(yuv_type_enum type,uint32 width,uint32 height,
    const uint8 *y_data,const uint8 *u_data,const uint8 *v_data,uint8 *rgb)
{
    if(type < YUV_TYPE_420_MAX) { // YUV420
        int32 h,w;
        uint32 point_index = 0;
        for(h = 0; h < height; h += 2) {
            for(w = 0; w < width; w += 2) {
                int32 u = (0xff & u_data[h / 2*width + w / 2]) - 128;
                int32 v = (0xff & v_data[h / 2*width + w / 2]) - 128;

                int32 i,j;
                for(i = 0; i < 2; i++) {
                    for(j = 0; j < 2; j++) {
                        int32 y = (0xff & ((int)y_data[(h + i)*width + w + j])) - 16;
                        if (y < 0)
                            y = 0;

                        int32 y1192 = 1192 * y;
                        int32 r = (y1192 + 1634 * v);
                        int32 g = (y1192 - 833 * v - 400 * u);
                        int32 b = (y1192 + 2066 * u);

                        if (r < 0)
                            r = 0;
                        else if (r > 262143)
                            r = 262143;
                        if (g < 0)
                            g = 0;
                        else if (g > 262143)
                            g = 262143;
                        if (b < 0)
                            b = 0;
                        else if (b > 262143)
                            b = 262143;

                        /*
                        dst[yp] = ((((r << 6) & 0xff0000) >> 16) << 16) |
                                  (((((g >> 2) & 0xff00) >> 8)) << 8) |
                                  ((((b >> 10) & 0xff)) << 0);
                                  */

                        LOGI("point:%d",(h + i)*width + w + j);
                        rgb[point_index * 3] = ((r << 6) & 0xff0000) >> 16;
                        rgb[point_index * 3 + 1] = ((g >> 2) & 0xff00) >> 8;
                        rgb[point_index * 3 + 2] = (b >> 10) & 0xff;

                        point_index++;
                    }
                }
            }
        }
    } else if(type < YUV_TYPE_422_MAX) { // YUV422

    } else {
        LOGE("Unknown YUV type:%d",type);
        return -1;
    }
    return 0;
}


static bool yuv_lenght_check(yuv_type_enum type,
    uint32 len,uint32 width,uint32 height)
{
    switch(type) {
        case YUV_TYPE_I420:
        case YUV_TYPE_YU12:
        case YUV_TYPE_NV12:
        case YUV_TYPE_NV21:
            if(len == width * height * 3 / 2)
                return true;
            break;
        default:
            LOGW("No such yuv type.[type-%d]",type);
            break;
    }
    return false;
}

static void yuv_help()
{
    LOGI("\nUsage : yuv_test type yuv_file width height\n\ttype:\n\t    0-I420\n\t    1-YU12\n\t    2-NV12\n\t    3-NV21");
}

// yuv_test i420/yu12/nv12/nv21 yuv_file width height
int main(int argc,char *argv[]) {
    if(argc != 5) {
        yuv_help();
        return EXIT_FAILURE;
    }

    int result = EXIT_SUCCESS;
    FILE *file = NULL;
    long file_len = 0L;
    uint8 *yuv_data = NULL;
    uint8 *y_data = NULL;
    uint8 *u_data = NULL;
    uint8 *v_data = NULL;
    uint8 *rgb_data = NULL;
    uint32 y_data_len = 0,u_data_len = 0,v_data_len = 0;
    uint32 len_temp = 0;
    uint32 read_count = 0;
    yuv_type_enum yuv_type = (yuv_type_enum)atoi(argv[1]);
    uint32 width = (uint32)atoi(argv[3]);
    uint32 height = (uint32)atoi(argv[4]);
    if(width <= 0 || height <= 0) {
        LOGE("width or height error[%s x %s].",argv[3],argv[4]);
        return EXIT_FAILURE;
    }

    file = fopen(argv[2],"r");
    if(file == NULL) {
        LOGE("fopen() fail[%d].",errno);
        return EXIT_FAILURE;
    }

    if(fseek(file,0,SEEK_END)) {
        LOGE("fseek() fail[%d].",errno);
        result = EXIT_FAILURE;
        goto exit_free;
    }

    file_len = ftell(file);
    if(file_len < 0) {
        LOGE("ftell() fail[%d].",errno);
        result = EXIT_FAILURE;
        goto exit_free;
    }

    if(fseek(file,0,SEEK_SET)) {
        LOGE("fseek() fail[%d].",errno);
        result = EXIT_FAILURE;
        goto exit_free;
    }

    LOGI("YUV file lenght:%d",file_len);

    if(!yuv_lenght_check(yuv_type,file_len,width,height)) {
        LOGE("YUV file lenght error.");
        result = EXIT_FAILURE;
        goto exit_free;
    }

    yuv_data = (uint8*)malloc(file_len);
    y_data = (uint8*)malloc(width * height);
    u_data = (uint8*)malloc(width * height);
    v_data = (uint8*)malloc(width * height);
    if(yuv_data == NULL || y_data == NULL
        || u_data == NULL || v_data == NULL) {
        LOGE("malloc() fail[%d].",errno);
        result = EXIT_FAILURE;
        goto exit_free;
    }

    while(read_count < file_len
        && (len_temp = fread(yuv_data + read_count, sizeof(uint8), file_len - read_count, file)) > 0) {
        LOGI("Read angin.");
        read_count += len_temp;
    }

    if(read_count != file_len) {
        LOGE("Read data lenght error[%d/%d].",read_count,file_len);
        result = EXIT_FAILURE;
        goto exit_free;
    }

    if(yuv_data_parse(yuv_data, file_len, yuv_type, width, height,
        y_data, &y_data_len, u_data, &u_data_len, v_data, &v_data_len)) {
        LOGW("yuv_data_parse() fail.");
        result = EXIT_FAILURE;
        goto exit_free;
    }

    LOGI("YUV parse success:y-%d,u-%d,v-%d",y_data_len,u_data_len,v_data_len);


    rgb_data = (uint8*)malloc(width * height * 3);
//    if(yuv_2_rgb(yuv_type,width,height,y_data,u_data,v_data,rgb_data)) {
//        LOGW("yuv_2_rgb() fail.");
//        result = EXIT_FAILURE;
//        goto exit_free;
//    }

    int count = 0;
    char pic_name[20];
    while(count < 4) {
        memset(pic_name,0x0,20);
        snprintf(pic_name,20,"/home/lb/test_%d.bmp",count);
        LOGI("Time - tart");
        yuv420_to_rgb(count,width,height,yuv_data,rgb_data);
        LOGI("Time - end");
        if(pic_bmp_save(pic_name, width, height, rgb_data, width * height * 3)) {
            LOGW("pic_bmp_save() fail.");
            result = EXIT_FAILURE;
            goto exit_free;
        }
        count++;
    }

    LOGI("Save BMP success.");

exit_free:
    if(yuv_data != NULL) {
        free(yuv_data);
        yuv_data = NULL;
    }
    if(y_data != NULL) {
        free(y_data);
        y_data = NULL;
    }
    if(u_data != NULL) {
        free(u_data);
        u_data = NULL;
    }
    if(v_data != NULL) {
        free(v_data);
        v_data = NULL;
    }
    if(rgb_data != NULL) {
        free(rgb_data);
        rgb_data = NULL;
    }
    if(file != NULL) {
        fclose(file);
        file = NULL;
    }

    return result;
}

