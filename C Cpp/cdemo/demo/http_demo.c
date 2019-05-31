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
    CURL *curl = curl_easy_init();//对curl进行初始化
    if(!curl) {
        printf("curl_easy_init() fail.\n");
        return CURLE_FAILED_INIT;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url); //设置下载地址
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);//设置超时时间
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_get_cb);//设置写数据的函数
//  curl_easy_setopt(curl, CURLOPT_WRITEDATA, str);//设置写数据的变量

    CURLcode result = curl_easy_perform(curl);//执行下载

    curl_easy_cleanup(curl); //释放curl资源

    return result;//执行下载
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

