#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <math.h>

#define TRUE 1
#define FALSE 0

typedef struct
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alah;
} color_s;

static color_s color;
static void info_print()
{
    int fp=0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    fp = open ("/dev/fb0",O_RDWR);

    if (fp < 0)
    {
        printf("Error : Can not open framebuffer device/n");
        return;
    }

    if (ioctl(fp,FBIOGET_FSCREENINFO,&finfo))
    {
        printf("Error reading fixed information/n");
        return;
    }

    if (ioctl(fp,FBIOGET_VSCREENINFO,&vinfo))
    {
        printf("Error reading variable information/n");
        return;
    }

    printf("The mem is :%d\n",finfo.smem_len);
    printf("The line_length is :%d\n",finfo.line_length);
    printf("The xres is :%d\n",vinfo.xres);
    printf("The yres is :%d\n",vinfo.yres);
    printf("bits_per_pixel is :%d\n",vinfo.bits_per_pixel);
    close (fp);
}

static void clear(color_s *color)
{
    int fp=0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long screensize=0;
    char *fbp = 0;
    long point = 0;
    fp = open ("/dev/fb0",O_RDWR);

    if (fp < 0)
    {
        printf("Error : Can not open framebuffer device/n");
        return;
    }

    if (ioctl(fp,FBIOGET_FSCREENINFO,&finfo))
    {
        printf("Error reading fixed information/n");
        return;
    }

    if (ioctl(fp,FBIOGET_VSCREENINFO,&vinfo))
    {
        printf("Error reading variable information/n");
        return;
    }

    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    /*这就是把fp所指的文件中从开始到screensize大小的内容给映射出来，得到一个指向这块空间的指针*/
    fbp =(char *) mmap (0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fp,0);
    if ((int) fbp == -1)
    {
        printf ("Error: failed to map framebuffer device to memory./n");
        return;
    }

    for(point = 0; point < screensize; point += 4)
    {
        *(fbp + point) = color->blue;  /* 蓝色的色深 */  /*直接赋值来改变屏幕上某点的颜色*/
        *(fbp + point + 1) = color->green; /* 绿色的色深*/
        *(fbp + point + 2) = color->red; /* 红色的色深*/
        *(fbp + point + 3) = color->alah;  /* 是否透明*/
    }

    munmap (fbp, screensize); /*解除映射*/
    close (fp);    /*关闭文件*/
}

static void draw_point(int x,int y,color_s *color)
{
    int fp=0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long screensize=0;
    char *fbp = 0;
    long location = 0;
    fp = open ("/dev/fb0",O_RDWR);

    if (fp < 0)
    {
        printf("Error : Can not open framebuffer device/n");
        return;
    }

    if (ioctl(fp,FBIOGET_FSCREENINFO,&finfo))
    {
        printf("Error reading fixed information/n");
        return;
    }

    if (ioctl(fp,FBIOGET_VSCREENINFO,&vinfo))
    {
        printf("Error reading variable information/n");
        return;
    }

    if(x <= 0 || x > vinfo.xres
       || y <= 0 || y > vinfo.yres)
    {
        printf("Point error.\n");
        return;
    }
    x--;
    y--;

    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    /*这就是把fp所指的文件中从开始到screensize大小的内容给映射出来，得到一个指向这块空间的指针*/
    fbp =(char *) mmap (0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fp,0);
    if ((int) fbp == -1)
    {
        printf ("Error: failed to map framebuffer device to memory./n");
        return;
    }
    /*这是你想画的点的位置坐标,(0，0)点在屏幕左上角*/
    location = x * (vinfo.bits_per_pixel / 8) + y  *  finfo.line_length;

    *(fbp + location) = color->blue;  /* 蓝色的色深 */  /*直接赋值来改变屏幕上某点的颜色*/
    *(fbp + location + 1) = color->green; /* 绿色的色深*/
    *(fbp + location + 2) = color->red; /* 红色的色深*/
    *(fbp + location + 3) = color->alah;  /* 是否透明*/
    munmap (fbp, screensize); /*解除映射*/
    close (fp);    /*关闭文件*/
}

static void draw_line(int start_x,int start_y,int end_x,int end_y,color_s *color)
{
    int fp=0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long screensize=0;
    char *fbp = 0;
    long location = 0;
    fp = open ("/dev/fb0",O_RDWR);

    if (fp < 0)
    {
        printf("Error : Can not open framebuffer device/n");
        return;
    }

    if (ioctl(fp,FBIOGET_FSCREENINFO,&finfo))
    {
        printf("Error reading fixed information/n");
        return;
    }

    if (ioctl(fp,FBIOGET_VSCREENINFO,&vinfo))
    {
        printf("Error reading variable information/n");
        return;
    }

    if(start_x <= 0 || start_x > vinfo.xres
       || start_y <= 0 || start_y > vinfo.yres
       || end_x <= 0 || end_x > vinfo.xres
       || end_y <= 0 || end_y > vinfo.yres)
    {
        printf("Point error.\n");
        return;
    }
    start_x--;
    start_y--;
    end_x--;
    end_y--;

    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    /*这就是把fp所指的文件中从开始到screensize大小的内容给映射出来，得到一个指向这块空间的指针*/
    fbp =(char *) mmap (0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fp,0);
    if ((int) fbp == -1)
    {
        printf ("Error: failed to map framebuffer device to memory./n");
        return;
    }

    if(start_x == end_x)
    {
        bool is_add;
        int y;
        if(start_y < end_y)
        {
            is_add = TRUE;
        }
        else
        {
            is_add = FALSE;
        }

        for(y = start_y; is_add?(y <= end_y):(y >= end_y); is_add?(y++):(y--))
        {
            location = start_x * (vinfo.bits_per_pixel / 8) + y * finfo.line_length;
            *(fbp + location) = color->blue;  /* 蓝色的色深 */  /*直接赋值来改变屏幕上某点的颜色*/
            *(fbp + location + 1) = color->green; /* 绿色的色深*/
            *(fbp + location + 2) = color->red; /* 红色的色深*/
            *(fbp + location + 3) = color->alah;  /* 是否透明*/
        }
    }
    else if(start_y == end_y)
    {
        bool is_add;
        int x;
        if(start_x < end_x)
        {
            is_add = TRUE;
        }
        else
        {
            is_add = FALSE;
        }

        for(x = start_x; is_add?(x <= end_x):(x >= end_x); is_add?(x++):(x--))
        {
            location = x * (vinfo.bits_per_pixel / 8) + start_y * finfo.line_length;
            *(fbp + location) = color->blue;  /* 蓝色的色深 */  /*直接赋值来改变屏幕上某点的颜色*/
            *(fbp + location + 1) = color->green; /* 绿色的色深*/
            *(fbp + location + 2) = color->red; /* 红色的色深*/
            *(fbp + location + 3) = color->alah;  /* 是否透明*/
        }
    }
    else
    {
        float a = 1.0f * (end_y - start_y)/(end_x - start_x);
        float b = -1.0f * a * start_x + start_y;
        bool is_add;
        int x,y;
        if(start_x < end_x)
        {
            is_add = TRUE;
        }
        else
        {
            is_add = FALSE;
        }

        for(x = start_x; is_add?(x <= end_x):(x >= end_x); is_add?(x++):(x--))
        {
            y = (long)(a * x + b + 0.5f);
            if(y < 0)
                y = 0;
            if(y > vinfo.yres)
                y = vinfo.yres;
            location = x * (vinfo.bits_per_pixel / 8) + y * finfo.line_length;
            *(fbp + location) = color->blue;  /* 蓝色的色深 */  /*直接赋值来改变屏幕上某点的颜色*/
            *(fbp + location + 1) = color->green; /* 绿色的色深*/
            *(fbp + location + 2) = color->red; /* 红色的色深*/
            *(fbp + location + 3) = color->alah;  /* 是否透明*/
        }
    }

    munmap (fbp, screensize); /*解除映射*/
    close (fp);    /*关闭文件*/
}

static void draw_circle(int point_x,int point_y,int r,color_s *color)
{
    int fp=0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long screensize=0;
    char *fbp = 0;
    long location = 0;
    fp = open ("/dev/fb0",O_RDWR);

    if (fp < 0)
    {
        printf("Error : Can not open framebuffer device/n");
        return;
    }

    if (ioctl(fp,FBIOGET_FSCREENINFO,&finfo))
    {
        printf("Error reading fixed information/n");
        return;
    }

    if (ioctl(fp,FBIOGET_VSCREENINFO,&vinfo))
    {
        printf("Error reading variable information/n");
        return;
    }

    if(point_x <= 0 || point_x > vinfo.xres
       || point_y <= 0 || point_y > vinfo.yres
       || r <= 0)
    {
        printf("Point error.\n");
        return;
    }
    //point_x--;
    //point_y--;

    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    /*这就是把fp所指的文件中从开始到screensize大小的内容给映射出来，得到一个指向这块空间的指针*/
    fbp =(char *) mmap (0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fp,0);
    if ((int) fbp == -1)
    {
        printf ("Error: failed to map framebuffer device to memory./n");
        return;
    }

    int start_x = point_x - r;
    int end_x = point_x + r;
    int start_y = point_y - r;
    int end_y = point_y + r;
    int x,y;
    for(x = start_x + r * 1.0f/4; x <= start_x + r * 7.0f/4; x++)
    {
        if(x < 0 || x >= vinfo.xres)
            continue;
        for(y = start_y; y <= point_y; y++)
        {
            float result = hypot(x - point_x,y - point_y);
            if(result < r) // First point in circle.
            {
                if(y > 0 && y <= vinfo.yres)
                {
                    y--;
                    location = x * (vinfo.bits_per_pixel / 8) + y * finfo.line_length;
                    *(fbp + location) = color->blue;  /* 蓝色的色深 */  /*直接赋值来改变屏幕上某点的颜色*/
                    *(fbp + location + 1) = color->green; /* 绿色的色深*/
                    *(fbp + location + 2) = color->red; /* 红色的色深*/
                    *(fbp + location + 3) = color->alah;  /* 是否透明*/

                    y += (point_y - y) * 2;
                    if(y >= 0 && y < vinfo.yres)
                    {
                        location = x * (vinfo.bits_per_pixel / 8) + y * finfo.line_length;
                        *(fbp + location) = color->blue;  /* 蓝色的色深 */  /*直接赋值来改变屏幕上某点的颜色*/
                        *(fbp + location + 1) = color->green; /* 绿色的色深*/
                        *(fbp + location + 2) = color->red; /* 红色的色深*/
                        *(fbp + location + 3) = color->alah;  /* 是否透明*/
                    }
                }
                else
                {
                    y += (point_y - y) * 2;
                    if(y >= 0 && y < vinfo.yres)
                    {
                        location = x * (vinfo.bits_per_pixel / 8) + y * finfo.line_length;
                        *(fbp + location) = color->blue;  /* 蓝色的色深 */  /*直接赋值来改变屏幕上某点的颜色*/
                        *(fbp + location + 1) = color->green; /* 绿色的色深*/
                        *(fbp + location + 2) = color->red; /* 红色的色深*/
                        *(fbp + location + 3) = color->alah;  /* 是否透明*/
                    }
                    break;
                }
            }
        }
    }

    for(y = start_y + r * 1.0f/4; y <= start_y + r * 7.0f/4; y++)
    {
        if(y < 0 || y >= vinfo.yres)
            continue;
        for(x = start_x; x <= point_x; x++)
        {
            float result = hypot(x - point_x,y - point_y);
            if(result < r) // First point in circle.
            {
                if(x > 0 && x <= vinfo.xres)
                {
                    x--;
                    location = x * (vinfo.bits_per_pixel / 8) + y * finfo.line_length;
                    *(fbp + location) = color->blue;  /* 蓝色的色深 */  /*直接赋值来改变屏幕上某点的颜色*/
                    *(fbp + location + 1) = color->green; /* 绿色的色深*/
                    *(fbp + location + 2) = color->red; /* 红色的色深*/
                    *(fbp + location + 3) = color->alah;  /* 是否透明*/

                    x += (point_x - x) * 2;
                    if(x >= 0 && x < vinfo.xres)
                    {
                        location = x * (vinfo.bits_per_pixel / 8) + y * finfo.line_length;
                        *(fbp + location) = color->blue;  /* 蓝色的色深 */  /*直接赋值来改变屏幕上某点的颜色*/
                        *(fbp + location + 1) = color->green; /* 绿色的色深*/
                        *(fbp + location + 2) = color->red; /* 红色的色深*/
                        *(fbp + location + 3) = color->alah;  /* 是否透明*/
                    }
                }
                else
                {
                    x += (point_x - x) * 2;
                    if(x >= 0 && x < vinfo.xres)
                    {
                        location = x * (vinfo.bits_per_pixel / 8) + y * finfo.line_length;
                        *(fbp + location) = color->blue;  /* 蓝色的色深 */  /*直接赋值来改变屏幕上某点的颜色*/
                        *(fbp + location + 1) = color->green; /* 绿色的色深*/
                        *(fbp + location + 2) = color->red; /* 红色的色深*/
                        *(fbp + location + 3) = color->alah;  /* 是否透明*/
                    }
                    break;
                }
            }
        }
    }

    munmap (fbp, screensize); /*解除映射*/
    close (fp);    /*关闭文件*/

}

static void draw_pic(int x,int y,int width,int height,char *pic)
{
    int fp=0;
    int fd;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long screensize=0;
    char *fbp = 0;
    long location = 0;
    fp = open ("/dev/fb0",O_RDWR);

    if (fp < 0)
    {
        printf("Error : Can not open framebuffer device/n");
        return;
    }

    if (ioctl(fp,FBIOGET_FSCREENINFO,&finfo))
    {
        printf("Error reading fixed information/n");
        return;
    }

    if (ioctl(fp,FBIOGET_VSCREENINFO,&vinfo))
    {
        printf("Error reading variable information/n");
        return;
    }

    if(x <= 0 || x > vinfo.xres
       || y <= 0 || y > vinfo.yres)
    {
        printf("Point error.\n");
        return;
    }
    x--;
    x--;

    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    /*这就是把fp所指的文件中从开始到screensize大小的内容给映射出来，得到一个指向这块空间的指针*/
    fbp =(char *) mmap (0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fp,0);
    if ((int) fbp == -1)
    {
        printf ("Error: failed to map framebuffer device to memory./n");
        return;
    }

    fd = open(pic,O_RDONLY);
    if(fd > 0)
    {
        char rbg[4];
        int i,j;
        for(i = 0;i < height; i++)
        {
            //printf("line:%d\n",y + i);
            for(j = 0;j < width;j++)
            {
                if(read(fd,rbg,4) == 4)
                {
                    location = (x + j) * (vinfo.bits_per_pixel / 8)
                        + (y + i) * finfo.line_length;
                    *(fbp + location) = rbg[0];  /* 蓝色的色深 */  /*直接赋值来改变屏幕上某点的颜色*/
                    *(fbp + location + 1) = rbg[1]; /* 绿色的色深*/
                    *(fbp + location + 2) = rbg[2]; /* 红色的色深*/
                    *(fbp + location + 3) = rbg[3];  /* 是否透明*/
                }
            }
        }
        close(fd);
    }

    munmap (fbp, screensize); /*解除映射*/
    close (fp);    /*关闭文件*/
}

static void draw_bmp(int x,int y,int width,int height,char *pic)
{
    int fp=0;
    int fd;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long screensize=0;
    char *fbp = 0;
    long location = 0;
    fp = open ("/dev/fb0",O_RDWR);

    if (fp < 0)
    {
        printf("Error : Can not open framebuffer device/n");
        return;
    }

    if (ioctl(fp,FBIOGET_FSCREENINFO,&finfo))
    {
        printf("Error reading fixed information/n");
        return;
    }

    if (ioctl(fp,FBIOGET_VSCREENINFO,&vinfo))
    {
        printf("Error reading variable information/n");
        return;
    }

    if(x <= 0 || x > vinfo.xres
       || y <= 0 || y > vinfo.yres)
    {
        printf("Point error.\n");
        return;
    }
    x--;
    x--;

    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    /*这就是把fp所指的文件中从开始到screensize大小的内容给映射出来，得到一个指向这块空间的指针*/
    fbp =(char *) mmap (0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fp,0);
    if ((int) fbp == -1)
    {
        printf ("Error: failed to map framebuffer device to memory./n");
        return;
    }

    fd = open(pic,O_RDONLY);
    if(fd > 0)
    {
        char rbg[3];
        int i,j;
        for(i = height;i >= 0; i--)
        {
            //printf("line:%d\n",y + i);
            for(j = 0;j < width;j++)
            {
                if(read(fd,rbg,3) == 3)
                {
                    location = (x + j) * (vinfo.bits_per_pixel / 8)
                        + (y + i) * finfo.line_length;
                    *(fbp + location) = rbg[0];  /* 蓝色的色深 */  /*直接赋值来改变屏幕上某点的颜色*/
                    *(fbp + location + 1) = rbg[1]; /* 绿色的色深*/
                    *(fbp + location + 2) = rbg[2]; /* 红色的色深*/
                    *(fbp + location + 3) = 0x00;  /* 是否透明*/
                }
            }
        }
        close(fd);
    }

    munmap (fbp, screensize); /*解除映射*/
    close (fp);    /*关闭文件*/
}


static void draw_circle_1(int point_x,int point_y,int r,color_s *color)
{
    int fp=0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long screensize=0;
    char *fbp = 0;
    long location = 0;
    fp = open ("/dev/fb0",O_RDWR);

    if (fp < 0)
    {
        printf("Error : Can not open framebuffer device/n");
        return;
    }

    if (ioctl(fp,FBIOGET_FSCREENINFO,&finfo))
    {
        printf("Error reading fixed information/n");
        return;
    }

    if (ioctl(fp,FBIOGET_VSCREENINFO,&vinfo))
    {
        printf("Error reading variable information/n");
        return;
    }

    if(point_x <= 0 || point_x > vinfo.xres
       || point_y <= 0 || point_y > vinfo.yres
       || r <= 0)
    {
        printf("Point error.\n");
        return;
    }
    point_x--;
    point_y--;

    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
    /*这就是把fp所指的文件中从开始到screensize大小的内容给映射出来，得到一个指向这块空间的指针*/
    fbp =(char *) mmap (0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fp,0);
    if ((int) fbp == -1)
    {
        printf ("Error: failed to map framebuffer device to memory./n");
        return;
    }

    int start_x = point_x - r;
    int end_x = point_x + r;
    int x,y,c;
    for(x = start_x; x <= end_x; x++)
    {
        c = sqrt(pow(r,2) - pow(x - point_x,2));
        y = point_y + c;
        if(x < 0 || x >= vinfo.xres
           || y < 0 || y >= vinfo.yres)
            continue;

        location = x * (vinfo.bits_per_pixel / 8) + y * finfo.line_length;
        *(fbp + location) = color->blue;  /* 蓝色的色深 */  /*直接赋值来改变屏幕上某点的颜色*/
        *(fbp + location + 1) = color->green; /* 绿色的色深*/
        *(fbp + location + 2) = color->red; /* 红色的色深*/
        *(fbp + location + 3) = color->alah;  /* 是否透明*/

        y = point_y - c;
        if(x < 0 || x >= vinfo.xres
           || y < 0 || y >= vinfo.yres)
            continue;

        location = x * (vinfo.bits_per_pixel / 8) + y * finfo.line_length;
        *(fbp + location) = color->blue;  /* 蓝色的色深 */  /*直接赋值来改变屏幕上某点的颜色*/
        *(fbp + location + 1) = color->green; /* 绿色的色深*/
        *(fbp + location + 2) = color->red; /* 红色的色深*/
        *(fbp + location + 3) = color->alah;  /* 是否透明*/
    }

    munmap (fbp, screensize); /*解除映射*/
    close (fp);    /*关闭文件*/
}


int main (int argc,char *argv[])
{
    info_print();

    color_s color_clear;
    memset(&color_clear,0x0,sizeof(color_s));
    color_clear.red = 0x00;
    color_clear.green = 0x00;
    color_clear.blue = 0x00;
    color_clear.alah = 0x00;

    memset(&color,0x0,sizeof(color_s));
    color.red = 0xFF;
    color.green = 0xFF;
    color.blue = 0xFF;
    color.alah = 0x00;

    while(TRUE)
    {
        char cmd[100];
        bzero(cmd,100);
        printf(">>");
        fgets(cmd,100,stdin);
        if(!memcmp(cmd,"color",5))
        {
            memset(&color,0x0,sizeof(color_s));
            sscanf(cmd + 6,"%d %d %d",&(color.red),&(color.green),
                   &(color.blue));
            printf("Color[%d,%d,%d]\n",color.red,color.green,color.blue);
        }
        else if(!memcmp(cmd,"clear",5))
        {
            printf("Clear[%d,%d,%d,%d]\n",color_clear.red,color_clear.green,color_clear.blue,color_clear.alah);
            clear(&color_clear);
        }
        else if(!memcmp(cmd,"point",5))
        {
            int x,y;
            sscanf(cmd + 6,"%d %d",&x,&y);

            //clear(&color_clear);

            printf("Point[%d,%d,%d,%d] [%d,%d]\n",color.red,color.green,color.blue,color.alah,x,y);
            draw_point(x,y,&color);
        }
        else if(!memcmp(cmd,"line",4))
        {
            int x1,y1,x2,y2;
            sscanf(cmd + 5,"%d %d %d %d",&x1,&y1,&x2,&y2);

            //clear(&color_clear);

            printf("Line[%d,%d,%d,%d] [%d,%d] - [%d,%d]\n",color.red,color.green,color.blue,color.alah,x1,y1,x2,y2);
            draw_line(x1,y1,x2,y2,&color);
        }
        else if(!memcmp(cmd,"circle",6))
        {
            int x1,y1,r;
            sscanf(cmd + 7,"%d %d %d",&x1,&y1,&r);

            //clear(&color_clear);

            printf("Line[%d,%d,%d,%d] [%d,%d] - [%d]\n",color.red,color.green,color.blue,color.alah,x1,y1,r);
            draw_circle(x1,y1,r,&color);
        }
        else if(!memcmp(cmd,"pic",3))
        {
            char *pic = "/home/lb/data/pic_data";
            int x,y,width,height;
            sscanf(cmd + 4,"%d %d %d %d",&x,&y,&width,&height);

            clear(&color_clear);

            printf("Pic[%d,%d,%d,%d] [%s]\n",x,y,width,height,pic);
            draw_pic(x,y,width,height,pic);
        }
        else if(!memcmp(cmd,"bmp",3))
        {
            char *pic = "/home/lb/data/pic.bmp";
            int x,y,width,height;
            sscanf(cmd + 4,"%d %d %d %d",&x,&y,&width,&height);

            clear(&color_clear);

            printf("BMP[%d,%d,%d,%d] [%s]\n",x,y,width,height,pic);
            draw_bmp(x,y,width,height,pic);
        }
        else if(!memcmp(cmd,"exit",4)
                || !memcmp(cmd,"q",1))
        {
            break;
        }
        else
        {
            char *ptr = cmd + strlen(cmd) - 1;
            while(ptr >= cmd && (*ptr == '\r' || *ptr == '\n'))
            {
                *ptr-- = '\0';
            }
            printf("CMD error.[%s]\n",cmd);
        }
    }

    printf("EXIT.\n");
    return 0;
}
