#include <stdio.h>
#include <stdlib.h>

// yuv_test i420/yu12/nv12/nv21 yuv_file width height
int main(int argc,char *argv[]) {
    FILE *file = fopen("~/test.yuv","r");
    if(file == NULL) {
        printf("Open file fail.\n");
        return EXIT_FAILURE;
    }



    return EXIT_SUCCESS;
}

