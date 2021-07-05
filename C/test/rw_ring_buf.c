/*#############################################################
 *     File Name	: rw_ring_buf.c
 *     Author		: winddoing
 *     Created Time	: 2021年06月30日 星期三 09时30分24秒
 *     Description	:
 *############################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

#define RW_BUF_LEN  8

struct rw_buf {
    int data[RW_BUF_LEN];
#define WRITABLE    0
#define READABLE    1
    int data_status[RW_BUF_LEN];
    int write_pos;
    int read_pos;
};


static void* test_read(void* args)
{
    struct rw_buf* buf = (struct rw_buf*)args;
    int i = 0;

    for (i = 0; i < 100; i++) {
        int pos = buf->write_pos % RW_BUF_LEN;
        while (buf->data_status[pos] != WRITABLE){
            printf("==========%s %d: wait...\n", __func__, __LINE__);
            usleep(100);
        }
        buf->data[pos] = i;
        buf->data_status[pos] = READABLE;
        pos++;
        buf->write_pos = pos;
    }

    return NULL;
}

static void* test_write(void* args)
{
    struct rw_buf* buf = (struct rw_buf*)args;
    int i = 0;

    for (i = 0; i < 100; i++) {
        int pos = buf->read_pos % RW_BUF_LEN;
        while (buf->data_status[pos] != READABLE){
            usleep(100);
            printf("----------%s %d: wait...\n", __func__, __LINE__);
        }
        printf("%s %d: data=%d, pos=%d\n", __func__, __LINE__, buf->data[pos], pos);
        //usleep(1000);
        buf->data_status[pos] = WRITABLE;
        pos++;
        buf->read_pos = pos;
    }

    return NULL;
}

int main(int argc, const char *argv[])
{
    pthread_t tid_r, tid_w;
    struct rw_buf buffer;
    
    memset(&buffer, 0, sizeof(struct rw_buf));

    if(pthread_create(&tid_r, NULL, test_read, &buffer) != 0) {
        printf("create read thread fail, error %s\n", strerror(errno));
        return -1;
    }

    if(pthread_create(&tid_w, NULL, test_write, &buffer) != 0) {
        printf("create write thrad fail, error %s\n", strerror(errno));
        return -1;
    }

    pthread_join(tid_r, NULL);
    pthread_join(tid_w, NULL);
    
    return 0;
}
