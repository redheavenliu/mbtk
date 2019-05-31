#ifndef IMAGE_H
#define IMAGE_H
#include <stdio.h>

void image_info(FILE* file);
void image_save(FILE *file);
void image_gray();
void image_binarization();
void image_opposite();
void image_channel(); //��ȡRGBͨ��
void image_bright();//�ı�ͼ������
typedef struct BMP
{
//14�ֽ�
    unsigned short bfType; //�ļ���ʶ 2�ֽ� ����ΪBM
    unsigned int bfSize; //�ļ���С 4�ֽ�
    unsigned short bfReserved1; //������ÿ�ֽ���"00"��д 2�ֽ�
    unsigned short bfReserved2; //ͬ�� 2�ֽ�
    unsigned int bfOffBits; //��¼ͼ������������ʼλ��(ͼ������������ļ�ͷ�ֽڵ�ƫ����)�� 4�ֽ�
//40�ֽ�
    unsigned int biSize; //��ʾ���ṹ�Ĵ�С 4�ֽ�
    int biWidth; //λͼ�Ŀ�� 4�ֽ�
    int biHeight; //λͼ�ĸ߶� 4�ֽ�
    unsigned short biPlanes; //��ԶΪ1 , 2�ֽ�
    unsigned short biBitCount; //λͼ��λ�� ��Ϊ1 4 8 16 24 32 2�ֽ�
    unsigned int biCompression; //ѹ��˵�� 4�ֽ�
    unsigned int biSizeImage; //��ʾλͼ��������Ĵ�С���ֽ�Ϊ��λ 4�ֽ�
    int biXPelsPerMeter; //������/�ױ�ʾ��ˮƽ�ֱ��� 4�ֽ�
    int biYPelsPerMeter; //������/�ױ�ʾ�Ĵ�ֱ�ֱ��� 4�ֽ�
    unsigned int biClrUsed; //λͼʹ�õ���ɫ������ 4�ֽ�
    unsigned int biClrImportant; //��ͼ����ʾ����ҪӰ�����ɫ��������Ŀ 4�ֽ�
} BMP;
int line_byte;
unsigned char *imagedata;
extern BMP bmp;
extern int line_byte;
extern unsigned char *imagedata;
#endif
//image_rw.c�ļ�
#include<stdio.h>
#include<stdlib.h>
#include"image.h"
void image_info(FILE *file)
{
    int times=3; //�����ļ���������
    char bmp_name[10]; //�ļ���
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
        file=fopen(bmp_name,"rb+"); //��һ���ļ����ж�д������
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
//��ȡͼ����Ϣ
    fseek(file,0L,0); //��ȡͼ���ļ�����
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
    line_byte=(bmp.biWidth*bmp.biBitCount/8+3)/4*4; //���ͼ������ÿ�е����ݸ���
//printf("dfsa%u",bmp.line_byte);
//bmp.imagedata=NULL;
    imagedata=(unsigned char*)malloc(bmp.biSizeImage);
    fseek(file,(long)bmp.bfOffBits,0);
    fread(imagedata,sizeof(unsigned char),bmp.biSizeImage,file);
    fclose(file);
}
//����ͼ��
void image_save(FILE *file)
{
    int times=3; //�����ļ���������
    char bmp_name[10]; //�ļ���
//int i; //��¼����������
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
        file=fopen(bmp_name,"wb+"); //��һ���ļ����ж�д������
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
//д�ļ�ͷ
    printf("\n%s",bmp_name);
    fseek(file,0L,0); //ͼ���ļ�����
    fwrite(&(bmp.bfType),sizeof(short),1,file);
    printf("\n bmp tpye: %d",bmp.bfType);
    fseek(file,2L,0); //ͼ���ļ���С
    fwrite(&(bmp.bfSize),sizeof(int),1,file);
    printf("\n bmp size: %d",bmp.bfSize);
    fseek(file,6L,0); //ͼ���ļ�������1
    fwrite(&(bmp.bfReserved1),sizeof(short),1,file);
    printf("\n bmp reserved1: %d",bmp.bfReserved1);
    fseek(file,8L,0); //ͼ���ļ�������2
    fwrite(&(bmp.bfReserved2),sizeof(short),1,file);
    printf("\n bmp reserved2: %d",bmp.bfReserved2);
    fseek(file,10L,0);//��������ƫ����
    fwrite(&(bmp.bfOffBits),sizeof(short),1,file);
    printf("\n bmp offBits: %d",bmp.bfOffBits);
    fseek(file,14L,0);//�ļ�ͷ�ṹ��С
    fwrite(&(bmp.biSize),sizeof(int),1,file);
    printf("\n bmp bisize: %d",bmp.biSize);
    fseek(file,18L,0);//ͼ��Ŀ��
    fwrite(&(bmp.biWidth),sizeof(int),1,file);
    printf("\n bmp biWidth: %d",bmp.biWidth);
    fseek(file,22L,0);//ͼ��ĸ߶�
    fwrite(&(bmp.biHeight),sizeof(int),1,file);
    printf("\n bmp biHeight: %d",bmp.biHeight);
    fseek(file,24L,0);//ͼ�������
    fwrite(&(bmp.biPlanes),sizeof(short),1,file);
    printf("\n bmp biplans: %d",bmp.biPlanes);
    fseek(file,28L,0);//ͼ��һ�����ص��ֽ���
    fwrite(&(bmp.biBitCount),sizeof(short),1,file);
    printf("\n bmp biBitCount: %d",bmp.biBitCount);
    fseek(file,30L,0);//ͼ��ѹ����Ϣ
    fwrite(&(bmp.biCompression),sizeof(short),1,file);
    printf("\n bmp biCompression: %d",bmp.biCompression);
    fseek(file,34L,0);//ͼ���������Ĵ�С
    fwrite(&(bmp.biSizeImage),sizeof(int),1,file);
    printf("\n bmp biSizeImage: %d",bmp.biSizeImage);
    fseek(file,38L,0);//ˮƽ�ֱ���
    fwrite(&(bmp.biXPelsPerMeter),sizeof(int),1,file);
    printf("\n bmp biXPelsPerMeter: %d",bmp.biXPelsPerMeter);
    fseek(file,42L,0);//��ֱ�ֱ���
    fwrite(&(bmp.biYPelsPerMeter),sizeof(int),1,file);
    printf("\n bmp biYPelsPerMeter: %d",bmp.biYPelsPerMeter);
    fseek(file,46L,0);//��ɫ������
    fwrite(&(bmp.biClrUsed),sizeof(int),1,file);
    printf("\n bmp biClrUsed: %d",bmp.biClrUsed);
    fseek(file,50L,0);//��Ҫ��ɫ������
    fwrite(&(bmp.biClrImportant),sizeof(int),1,file);
    printf("\n bmp biClrImportant: %d\n",bmp.biClrImportant);
    fseek(file,(long)(bmp.bfOffBits),0);
    fwrite(imagedata,sizeof(unsigned char),bmp.biSizeImage,file);
    fclose(file);
}
//pixProcess.c�ļ�
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"image.h"
//�ҶȻ�
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
//��ֵ��
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
void image_opposite() //����
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
void image_channel() //��ȡRGBͨ��
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
void image_bright()//�ı�ͼ������
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
//void image_create() //����һ��24λBMPͼ���ļ���
//{
//main.c�ļ�
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
        image_info(file); //imagedata�Ѿ������˶�̬�ڴ棬����û���ͷ�
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
