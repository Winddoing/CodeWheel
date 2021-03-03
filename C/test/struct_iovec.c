/*#############################################################
 *     File Name	: struct_iovec.c
 *     Author		: winddoing
 *     Created Time	: 2021年01月26日 星期二 10时01分00秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <sys/uio.h>
#include <string.h>

//struct iovec {
//    /* Starting address (内存起始地址）*/
//    void  *iov_base;   
//
//    /* Number of bytes to transfer（这块内存长度） */
//    size_t iov_len;    
//};

// struct iovec定义了一个向量元素。通常，这个结构用作一个多元素的数组。
// 对于每一个传输的元素，
// 指针成员iov_base指向一个缓冲区，这个缓冲区是存放的是readv所接收的数据或是writev将要发送的数据;
// 成员iov_len在各种情况下分别确定了接收的最大长度以及实际写入的长度;

int main(int argc,char **argv)
{
    struct iovec iov[4];
    char *p1 = "123";
    char *p2 = "abc";
    char *p3 = "XYZ";
    int p4 = 65;  //ASCII "A"

    iov[0].iov_base = p1;
    iov[0].iov_len = strlen(p1);

    iov[1].iov_base = p2;
    iov[1].iov_len = strlen(p2);

    iov[2].iov_base = p3;
    iov[2].iov_len = strlen(p3);

    iov[3].iov_base = &p4;
    iov[3].iov_len = sizeof(int);

    ssize_t ret = writev(1, iov, 4);
    if(ret < 0) {
        perror("writev");
        return -1;
    }

    return 0;
}
