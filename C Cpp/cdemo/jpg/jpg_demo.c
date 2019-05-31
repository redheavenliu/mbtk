#ifndef IMAGE_H
#define IMAGE_H
#include <stdio.h>

void image_info(FILE* file);
void image_save(FILE *file);
void image_gray();
void image_binarization();
void image_opposite();
void image_channel(); //抽取RGB通道
void image_bright();//改变图像亮度
typedef struct BMP
{
//14字节
    unsigned short bfType; //文件标识 2字节 必须为BM
    unsigned int bfSize; //文件大小 4字节
    unsigned short bfReserved1; //保留，每字节以"00"填写 2字节
    unsigned short bfReserved2; //同上 2字节
    unsigned int bfOffBits; //记录图像数据区的起始位置(图象数据相对于文件头字节的偏移量)。 4字节
//40字节
    unsigned int biSize; //表示本结构的大小 4字节
    int biWidth; //位图的宽度 4字节
    int biHeight; //位图的高度 4字节
    unsigned short biPlanes; //永远为1 , 2字节
    unsigned short biBitCount; //位图的位数 分为1 4 8 16 24 32 2字节
    unsigned int biCompression; //压缩说明 4字节
    unsigned int biSizeImage; //表示位图数据区域的大小以字节为单位 4字节
    int biXPelsPerMeter; //用象素/米表示的水平分辨率 4字节
    int biYPelsPerMeter; //用象素/米表示的垂直分辨率 4字节
    unsigned int biClrUsed; //位图使用的颜色索引数 4字节
    unsigned int biClrImportant; //对图象显示有重要影响的颜色索引的数目 4字节
} BMP;
int line_byte;
unsigned char *imagedata;
extern BMP bmp;
extern int line_byte;
extern unsigned char *imagedata;
#endif
//image_rw.c文件
#include<stdio.h>
#include<stdlib.h>
#include"image.h"
void image_info(FILE *file)
{
    int times=3; //输入文件名次数。
    char bmp_name[10]; //文件名
    printf("\nplease enter a file name for reading:");
    do
    {
        if (times<3)
        {
            printf("\nplease enter a file name for reading again:");
        }
        fflush(stdin);
        gets(bmp_name);
//printf("\n%s",bmp_name);
        file=fopen(bmp_name,"rb+"); //打开一个文件进行读写操作。
        --times;
        if (file==NULL)
        {
            printf("\nerror opening %s for reading! ",bmp_name);
        }
        else
        {
            break;
        }
    }
    while(times!=0);
    if (times==0)
    {
        printf("\nsorry, shutdown!");
        exit(1);
    }
//读取图像信息
    fseek(file,0L,0); //读取图像文件类型
    fread(&bmp,sizeof(BMP),1,file);
    printf("\n bmp tpye: %u",bmp.bfType);
    printf("\n bmp size: %u",bmp.bfSize);
    printf("\n bmp reserved1: %u",bmp.bfReserved1);
    printf("\n bmp reserved2: %u",bmp.bfReserved2);
    printf("\n bmp offBits: %u",bmp.bfOffBits);
    printf("\n bmp bisize: %u",bmp.biSize);
    printf("\n bmp biWidth: %d",bmp.biWidth);
    printf("\n bmp biHeight: %d",bmp.biHeight);
    printf("\n bmp biplans: %u",bmp.biPlanes);
    printf("\n bmp biBitCount: %u",bmp.biBitCount);
    printf("\n bmp biCompression: %u",bmp.biCompression);
    printf("\n bmp biSizeImage: %u",bmp.biSizeImage);
    printf("\n bmp biXPelsPerMeter: %d",bmp.biXPelsPerMeter);
    printf("\n bmp biYPelsPerMeter: %d",bmp.biYPelsPerMeter);
    printf("\n bmp biClrUsed: %u",bmp.biClrUsed);
    printf("\n bmp biClrImportant: %u\n",bmp.biClrImportant);
    line_byte=(bmp.biWidth*bmp.biBitCount/8+3)/4*4; //获得图像数据每行的数据个数
//printf("dfsa%u",bmp.line_byte);
//bmp.imagedata=NULL;
    imagedata=(unsigned char*)malloc(bmp.biSizeImage);
    fseek(file,(long)bmp.bfOffBits,0);
    fread(imagedata,sizeof(unsigned char),bmp.biSizeImage,file);
    fclose(file);
}
//保存图像
void image_save(FILE *file)
{
    int times=3; //输入文件名次数。
    char bmp_name[10]; //文件名
//int i; //记录数据区个数
    printf("\nplease enter a file name for writeing:");
    do
    {
        if (times<3)
        {
            printf("\nplease enter a file name for writeing again:");
        }
        fflush(stdin);
        gets(bmp_name);
        printf("\n%s",bmp_name);
        file=fopen(bmp_name,"wb+"); //打开一个文件进行读写操作。
        --times;
        if (file==NULL)
        {
            printf("\nerror opening %s for writing",bmp_name);
        }
        else
        {
            break;
        }
    }
    while(times!=0);
    if (times==0)
    {
        printf("\nsorry, shutdown!");
        exit(1);
    }
//写文件头
    printf("\n%s",bmp_name);
    fseek(file,0L,0); //图像文件类型
    fwrite(&(bmp.bfType),sizeof(short),1,file);
    printf("\n bmp tpye: %d",bmp.bfType);
    fseek(file,2L,0); //图像文件大小
    fwrite(&(bmp.bfSize),sizeof(int),1,file);
    printf("\n bmp size: %d",bmp.bfSize);
    fseek(file,6L,0); //图像文件保留字1
    fwrite(&(bmp.bfReserved1),sizeof(short),1,file);
    printf("\n bmp reserved1: %d",bmp.bfReserved1);
    fseek(file,8L,0); //图像文件保留字2
    fwrite(&(bmp.bfReserved2),sizeof(short),1,file);
    printf("\n bmp reserved2: %d",bmp.bfReserved2);
    fseek(file,10L,0);//数据区的偏移量
    fwrite(&(bmp.bfOffBits),sizeof(short),1,file);
    printf("\n bmp offBits: %d",bmp.bfOffBits);
    fseek(file,14L,0);//文件头结构大小
    fwrite(&(bmp.biSize),sizeof(int),1,file);
    printf("\n bmp bisize: %d",bmp.biSize);
    fseek(file,18L,0);//图像的宽度
    fwrite(&(bmp.biWidth),sizeof(int),1,file);
    printf("\n bmp biWidth: %d",bmp.biWidth);
    fseek(file,22L,0);//图像的高度
    fwrite(&(bmp.biHeight),sizeof(int),1,file);
    printf("\n bmp biHeight: %d",bmp.biHeight);
    fseek(file,24L,0);//图像的面数
    fwrite(&(bmp.biPlanes),sizeof(short),1,file);
    printf("\n bmp biplans: %d",bmp.biPlanes);
    fseek(file,28L,0);//图像一个像素的字节数
    fwrite(&(bmp.biBitCount),sizeof(short),1,file);
    printf("\n bmp biBitCount: %d",bmp.biBitCount);
    fseek(file,30L,0);//图像压缩信息
    fwrite(&(bmp.biCompression),sizeof(short),1,file);
    printf("\n bmp biCompression: %d",bmp.biCompression);
    fseek(file,34L,0);//图像数据区的大小
    fwrite(&(bmp.biSizeImage),sizeof(int),1,file);
    printf("\n bmp biSizeImage: %d",bmp.biSizeImage);
    fseek(file,38L,0);//水平分辨率
    fwrite(&(bmp.biXPelsPerMeter),sizeof(int),1,file);
    printf("\n bmp biXPelsPerMeter: %d",bmp.biXPelsPerMeter);
    fseek(file,42L,0);//垂直分辨率
    fwrite(&(bmp.biYPelsPerMeter),sizeof(int),1,file);
    printf("\n bmp biYPelsPerMeter: %d",bmp.biYPelsPerMeter);
    fseek(file,46L,0);//颜色索引数
    fwrite(&(bmp.biClrUsed),sizeof(int),1,file);
    printf("\n bmp biClrUsed: %d",bmp.biClrUsed);
    fseek(file,50L,0);//重要颜色索引数
    fwrite(&(bmp.biClrImportant),sizeof(int),1,file);
    printf("\n bmp biClrImportant: %d\n",bmp.biClrImportant);
    fseek(file,(long)(bmp.bfOffBits),0);
    fwrite(imagedata,sizeof(unsigned char),bmp.biSizeImage,file);
    fclose(file);
}
//pixProcess.c文件
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"image.h"
//灰度化
void image_gray()
{
    int i,j;
    unsigned char tmp;
    for (i=0; i<bmp.biHeight; i++)
    {
        for (j=0; j<line_byte/3; j++)
        {
            tmp=0.11*(*(imagedata+i*line_byte+j*3+0))+0.59*(*(imagedata+i*line_byte+j*3+1))+0.3*(*(imagedata+i*line_byte+j*3+2));
            imagedata[i*line_byte+j*3+0]=tmp;
            imagedata[i*line_byte+j*3+1]=tmp;
            imagedata[i*line_byte+j*3+2]=tmp;
//printf("\nnidsfh%d %d",i,j);
        }
    }
}
//二值化
void image_binarization()
{
    int i,j;
    for (i=0; i<bmp.biHeight; i++)
    {
        for (j=0; j<line_byte; j++)
        {
            if ((*(imagedata+i*line_byte+j))<128)
            {
                imagedata[i*line_byte+j]=0;
            }
            else
            {
                imagedata[i*line_byte+j]=255;
            }
        }
    }
}
void image_opposite() //反相
{
    int i,j;
    for (i=0; i<bmp.biHeight; i++)
    {
        for (j=0; j<line_byte; j++)
        {
            imagedata[i*line_byte+j]=abs(255-imagedata[i*line_byte+j]);
        }
    }
}
void image_channel() //抽取RGB通道
{
    int i,j;
    char rgb;
    printf("\nplease enter a char(r/g/b): ");
    fflush(stdin);
    scanf("%c",&rgb);
    if (rgb=='b')
    {
        for (i=0; i<bmp.biHeight; i++)
        {
            for (j=0; j<line_byte/3; j++)
            {
                imagedata[i*line_byte+3*j+1]=0;
                imagedata[i*line_byte+3*j+2]=0;
            }
        }
    }
    else if(rgb=='g')
    {
        for (i=0; i<bmp.biHeight; i++)
        {
            for (j=0; j<line_byte/3; j++)
            {
                imagedata[i*line_byte+3*j]=0;
                imagedata[i*line_byte+3*j+2]=0;
            }
        }
    }
    else
    {
        for (i=0; i<bmp.biHeight; i++)
        {
            for (j=0; j<line_byte/3; j++)
            {
                imagedata[i*line_byte+3*j]=0;
                imagedata[i*line_byte+3*j+1]=0;
            }
        }
    }
}
void image_bright()//改变图像亮度
{
    int level;
    int i,j;
    printf("\n please enter the level of brightness[-255 to 255] :");
    fflush(stdin);
    scanf("%d",&level);
    for (i=0; i<bmp.biHeight; i++)
    {
        for (j=0; j<line_byte; j++)
        {
            if (level>=0)
            {
                if ((imagedata[i*line_byte+j]+level)>255)
                    imagedata[i*line_byte+j]=255;
                else
                    imagedata[i*line_byte+j]+=level;
            }
            else
            {
                if ((imagedata[i*line_byte+j]-abs(level))<0)
                    imagedata[i*line_byte+j]=0;
                else
                    imagedata[i*line_byte+j]+=level;
            }
        }
    }
}
//void image_create() //创建一幅24位BMP图像文件。
//{
//main.c文件
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
#include"image.h"
BMP bmp;
int main(int argc,char *argv[])
{
    FILE *file=NULL;
    int choose;
    char gono;
    do
    {
        image_info(file); //imagedata已经分配了动态内存，但是没有释放
        printf("\n 1.image_opposite");
        printf("\n 2.image_gray");
        printf("\n 3.image_binarization");
        printf("\n 4.image_channel");
        printf("\n 5.image_brightness");
//printf("6.image_opposite");
//printf("7.image_opposite");
        printf("\nchoose your options:");
        fflush(stdin);
        scanf("%d",&choose);
        switch(choose)
        {
            case 1:
                image_opposite();
                image_save(file);
                free(imagedata);
                break;
            case 2:
                image_gray();
                image_save(file);
                free(imagedata);
                break;
            case 3:
                image_binarization();
                image_save(file);
                free(imagedata);
                break;
            case 4:
                image_channel();
                image_save(file);
                free(imagedata);
                break;
            case 5:
                image_bright();
                image_save(file);
                free(imagedata);
                break;
            default:
                printf("\n wrong choose!");
        }
        printf("\nlet's go on?(y/n):");
        fflush(stdin);
        scanf("%c",&gono);
        if (gono=='n')
        {
            printf("\nbye bye!");
            break;
        }
    }
    while(1);
    return 0;
}
