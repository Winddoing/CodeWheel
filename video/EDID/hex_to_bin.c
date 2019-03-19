/*#############################################################
 *     File Name    : hex2bin.c
 *     Author       : wqshao
 *     Created Time : 2018年07月16日 星期一 11时40分07秒
 *     Description  :
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <unistd.h>

static char trans(char * buffer)
{
    char temp = -1;
    int i = 0;

    for(i = 0; i < 2; i++) {
        temp = temp << 4;

        if(buffer[i] >= '0' && buffer[i] <= '9') {
            temp += buffer[i] - 0x30;

        } else if(buffer[i] >= 'a' && buffer[i] <= 'f') {
            temp += buffer[i] - 0x61 + 0x0a;

        } else if(buffer[i] >= 'A' && buffer[i] <= 'F') {
            temp += buffer[i] - 0x41 + 0x0a;

        } else {
            printf("get wrong value: %02X ,file: %s ,line: %d\n", buffer[i], __FILE__, __LINE__);
        }
    }

    return temp;
}


int main(int argc, char** argv)
{
    FILE * fd_in = 0;
    FILE * fd_out = 0;
    char buffer[1024] = {0};
    int size = 0;
    int i = 0;

    if(argc != 3) {
        printf("Please input filename.\n");
        printf("\tUsage: %s hex.file out.file\n", argv[0]);
        return 0;
    }

    fd_in = fopen(argv[1], "r");

    if(fd_in == 0) {
        printf("Please input correct filename ,%s.\n", argv[1]);
        return -1;
    }

    fd_out = fopen(argv[2], "wb");

    if(fd_out == 0) {
        printf("Please input correct filename ,%s.\n", argv[2]);
        fclose(fd_in);
        return -1;
    }

    while(!feof(fd_in)) {
        memset(buffer, 0, sizeof(buffer));
        fgets(buffer, 1024, fd_in);
        size = strlen(buffer);
        size -= 1; //最后的换行符

        for(i = 0; i < size; i += 2) {
            char temp = trans(&buffer[i]);
            fwrite(&temp, sizeof(char), 1, fd_out);
            //printf("temp :%02hhx buffer, i :%d , buffer[i]: %c, buffer [i+1]: %c,size : %d\n", temp, i, buffer[i], buffer[i + 1], size);
        }
    }

    fclose(fd_in);
    fclose(fd_out);

    return 0;
}

