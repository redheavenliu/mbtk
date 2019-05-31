#include <curl.h>
#include <stdio.h>
#include <string.h>

size_t http_get_cb(void *ptr, size_t size, size_t nmemb, void *stream)
{

    printf("HTTP GET:\n%s",ptr);

    return size * nmemb;
}

CURLcode http_get(const char *url)
{
    CURL *curl = curl_easy_init();//��curl���г�ʼ��
    if(!curl) {
        printf("curl_easy_init() fail.\n");
        return CURLE_FAILED_INIT;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url); //�������ص�ַ
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);//���ó�ʱʱ��
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_get_cb);//����д���ݵĺ���
//  curl_easy_setopt(curl, CURLOPT_WRITEDATA, str);//����д���ݵı���

    CURLcode result = curl_easy_perform(curl);//ִ������

    curl_easy_cleanup(curl); //�ͷ�curl��Դ

    return result;//ִ������
}

int main(int argc,char *argv[])
{
    if(argc != 2)
        printf("ARG error.\n");

    CURLcode result = http_get(argv[1]);
    if (CURLE_OK != result)
        printf("Get Fail[%d]!\n",result);

    return 0;
}

