#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include <poll.h>
#include <pthread.h>
#include <sys/syscall.h>
#include "gv_dma_api.h"

int gv_dma_device_init()
{
	int fd = -1;
	
	fd = open(DRIVER_NAME, O_RDWR);
	if(fd < 0){
		printf("open %s failed\n", DRIVER_NAME);
		return -1;
	}
	
	return fd;
}

void gv_dma_device_deinit(int fd)
{
	if(fd > 0)
		close(fd);
}

int gv_dma_start(int fd, struct dma_chncfg *chncfg)
{
    int ret;
    
    if( fd < 0 || chncfg == NULL)
        return -1;
    
    if(chncfg->src_addr >= 0xEFFFFFFF || (chncfg->src_addr + chncfg->len) > 0xEFFFFFFF ||
       chncfg->dst_addr >= 0xEFFFFFFF || (chncfg->dst_addr + chncfg->len) > 0xEFFFFFFF)
        return -1;
	
    ret = ioctl(fd, AXIDMA_IOCCFGANDSTART_AHB, chncfg);
    if(ret){
        printf("ioctl: gv_dma_start failed\n");
    }
    return ret;
}

int gv_hp_dma_start(int fd, struct dma_chncfg_sg *chncfg_sg)
{
    int ret;
	
    if( fd < 0 || chncfg_sg == NULL)
        return -1;

    if(chncfg_sg->src_addr >= 0xEFFFFFFF || (chncfg_sg->src_addr + chncfg_sg->len) > 0xEFFFFFFF ||
       chncfg_sg->dst_addr >= 0xEFFFFFFF || (chncfg_sg->dst_addr + chncfg_sg->len) > 0xEFFFFFFF ||
       chncfg_sg->high == 0)
        return -1;
	
    ret = ioctl(fd, AXIDMA_IOCCFGANDSTART_AXI, chncfg_sg);
    if(ret){
        printf("ioctl: gv_hp_dma_start failed\n");
    }
	
    return ret;
}

int gv_hp_multi_dma_start(int fd, GV_DMA_CHN_CFG_MULTI_SG_S *chncfg_multi_sg)
{
    int i;
    if( fd < 0 || chncfg_multi_sg == NULL || chncfg_multi_sg->cnt == 0 || chncfg_multi_sg->cnt > DMA_MAX_SG)
        return -1;
	
    for(i = 0; i < chncfg_multi_sg->cnt; i++){
        if(gv_hp_dma_start(fd, &chncfg_multi_sg->chncfg_sg[i]) < 0 ){
            return -1;
        }
    }

    return 0;
}

/******************************************************************************************
api test by DMA_API_TEST
******************************************************************************************/

//#define DMA_API_TEST
#ifdef DMA_API_TEST


#define SRC_ADDR        0x60000000
#define DST_ADDR        0x80000000
#define DMA_MEMCPY_LEN  0x3000000


#define DMA_TEST_SG_HIGH  5
#define DMA_TEST_SG_LEN 1024
#define DMA_TEST_SG_SRC_STRIDE 4096
#define DMA_TEST_SG_DST_STRIDE 4096

#define DMA_TEST_AXI_THREAD_NUM 4

void *src_buf = NULL;
void *dst_buf = NULL;

void init_mem_data(void)
{
    int ret;
    int mem_fd;


    /* for mmap */
    mem_fd = open("/dev/mem", O_RDWR | O_SYNC, 00777);
    if (-1 == mem_fd) {
        printf("open /dev/mem failed!\n");
        return;
    }

    src_buf = mmap(NULL, DMA_MEMCPY_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, SRC_ADDR);
    if (NULL == src_buf) {
        printf("mmap for src_addr failed!\n");
    }
    memset(src_buf, 0x55, DMA_MEMCPY_LEN);

    dst_buf = mmap(NULL, DMA_MEMCPY_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, DST_ADDR);
    if (NULL == dst_buf) {
        printf("mmap for dst_addr failed!\n");
    }
    memset(dst_buf, 0x0, DMA_MEMCPY_LEN);
    system("sync");


    munmap(src_buf, DMA_MEMCPY_LEN);
    munmap(dst_buf, DMA_MEMCPY_LEN);
    

    close(mem_fd);

}

void verify_mem_data(void)
{
    int mem_fd;

    mem_fd = open("/dev/mem", O_RDWR | O_SYNC, 00777);
    if (-1 == mem_fd) {
        printf("open /dev/mem failed!\n");
        return;
    }
    src_buf = mmap(NULL, DMA_MEMCPY_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, SRC_ADDR);
    if (NULL == src_buf) {
        printf("mmap for src_addr failed!\n");
    }

    dst_buf = mmap(NULL, DMA_MEMCPY_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, DST_ADDR);
    if (NULL == dst_buf) {
        printf("mmap for dst_addr failed!\n");
    }

    if(memcmp(src_buf, dst_buf, 0x300000 * DMA_TEST_AXI_THREAD_NUM) == 0)
    {
        printf("dma verify_mem_data success\n");
    }
    else
    {
        printf("dma verify_mem_data faile\n");
    }

    munmap(src_buf, DMA_MEMCPY_LEN);
    munmap(dst_buf, DMA_MEMCPY_LEN);
    close(mem_fd);
}

void verify_mem_sg_data(void)
{
    int mem_fd;
    int len,high,src_stride, dst_stride;
    int i;
    
    mem_fd = open("/dev/mem", O_RDWR | O_SYNC, 00777);
    if (-1 == mem_fd) {
        printf("open /dev/mem failed!\n");
        return;
    }
    src_buf = mmap(NULL, DMA_MEMCPY_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, SRC_ADDR);
    if (NULL == src_buf) {
        printf("mmap for src_addr failed!\n");
    }

    dst_buf = mmap(NULL, DMA_MEMCPY_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, DST_ADDR);
    if (NULL == dst_buf) {
        printf("mmap for dst_addr failed!\n");
    }

    len = DMA_TEST_SG_LEN;
	high = DMA_TEST_SG_HIGH;
	src_stride = DMA_TEST_SG_SRC_STRIDE;
	dst_stride = DMA_TEST_SG_DST_STRIDE;
    for(i=0; i<high; i++){
        if(memcmp(src_buf + i*src_stride, dst_buf + i*dst_stride, len) != 0)
            break;
    }

    if(i == high)
    {
        printf("dma block verify_mem_data success\n");
    }
    else
    {
        printf("dma block verify_mem_data faile\n");
    }

    munmap(src_buf, DMA_MEMCPY_LEN);
    munmap(dst_buf, DMA_MEMCPY_LEN);
    close(mem_fd);
}

void verify_mem_multi_sg_data(void)
{
    int mem_fd;
    int len,high, src_stride ,dst_stride;
    int i,j;
    
    mem_fd = open("/dev/mem", O_RDWR | O_SYNC, 00777);
    if (-1 == mem_fd) {
        printf("open /dev/mem failed!\n");
        return;
    }
    src_buf = mmap(NULL, DMA_MEMCPY_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, SRC_ADDR);
    if (NULL == src_buf) {
        printf("mmap for src_addr failed!\n");
    }

    dst_buf = mmap(NULL, DMA_MEMCPY_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, DST_ADDR);
    if (NULL == dst_buf) {
        printf("mmap for dst_addr failed!\n");
    }

    len = DMA_TEST_SG_LEN;
	high = DMA_TEST_SG_HIGH;
	src_stride = DMA_TEST_SG_SRC_STRIDE;
	dst_stride = DMA_TEST_SG_DST_STRIDE;
    for(i=0; i<high; i++){
        if(memcmp(src_buf + i*src_stride, dst_buf + i*dst_stride, len) != 0)
            break;
    }
    if(i != high)
    {
        printf("dma block verify_mem_data faile\n");
        goto verify_mem_multi_sg_data_err;
    }

    for(i=0; i<high; i++){
        if(memcmp(src_buf  +2000000 + i*src_stride, dst_buf + 2000000 + i*dst_stride, len) != 0)
            break;
    }
    if(i == high)
    {
        printf("dma block verify_mem_data success\n");
    }
    else
    {
        printf("dma block verify_mem_data faile\n");
    }

    
verify_mem_multi_sg_data_err:
    munmap(src_buf, DMA_MEMCPY_LEN);
    munmap(dst_buf, DMA_MEMCPY_LEN);
    close(mem_fd);
}

int fd_test = -1;

void * dma_a(void *a){
    
    int ret = 0;
    struct dma_chncfg chncfg;

    /* config addr */
    chncfg.src_addr = SRC_ADDR;
    chncfg.dst_addr = DST_ADDR;
    chncfg.len = 0x300000;
    ret = gv_dma_start(fd_test, &chncfg);
    if(ret){
        printf("ioctl: config and start dma failed\n");
    }
    return NULL;

}

void * dma_b(void *a){
    int ret = 0;
    struct dma_chncfg chncfg;
    /* config addr */
    chncfg.src_addr = SRC_ADDR+0x300000;
    chncfg.dst_addr = DST_ADDR+0x300000;
    chncfg.len = 0x300000;
    ret = gv_dma_start(fd_test, &chncfg);
    if(ret){
        printf("ioctl: config and start dma failed\n");
    }
    return NULL;

}

void * dma_c(void *a){
    int ret = 0;

    struct dma_chncfg chncfg;
    /* config addr */
    chncfg.src_addr = SRC_ADDR+0x600000;
    chncfg.dst_addr = DST_ADDR+0x600000;
    chncfg.len = 0x300000;
    ret = gv_dma_start(fd_test, &chncfg);
    if(ret){
        printf("ioctl: config and start dma failed\n");
    }
    return NULL;

}

void * dma_d(void *a){
    int ret = 0;

    struct dma_chncfg chncfg;
    /* config addr */
    chncfg.src_addr = SRC_ADDR+0x900000;
    chncfg.dst_addr = DST_ADDR+0x900000;
    chncfg.len = 0x300000;
    ret = gv_dma_start(fd_test, &chncfg);
    if(ret){
        printf("ioctl: config and start dma failed\n");
    }
    return NULL;

}

void * dma_e(void *a){
    struct dma_chncfg chncfg;
    int ret = 0;
    
    /* config addr */
    chncfg.src_addr = SRC_ADDR+0xc00000;
    chncfg.dst_addr = DST_ADDR+0xc00000;
    chncfg.len = 0x300000;
#ifdef AHB_TEST
    ret = gv_dma_start(fd_test, &chncfg);
#endif
    if(ret){
        printf("ioctl: config and start dma failed\n");
    }
    
    return NULL;

}


void * dma_f(void *a){
    struct dma_chncfg chncfg;
    int ret = 0;
    
    /* config addr */
    chncfg.src_addr = SRC_ADDR+0xf00000;
    chncfg.dst_addr = DST_ADDR+0xf00000;
    chncfg.len = 0x300000;
#ifdef AHB_TEST
    ret = gv_dma_start(fd_test, &chncfg);
#endif
    if(ret){
        printf("ioctl: config and start dma failed\n");
    }
    
    return NULL;

}

void * dma_g(void *a){
    struct dma_chncfg chncfg;
    int ret = 0;
    
    /* config addr */
    chncfg.src_addr = SRC_ADDR+0x1200000;
    chncfg.dst_addr = DST_ADDR+0x1200000;
    chncfg.len = 0x300000;
#ifdef AHB_TEST
        ret = gv_dma_start(fd_test, &chncfg);
#endif
    if(ret){
        printf("ioctl: config and start dma failed\n");
    }
    
    return NULL;

}

void * dma_h(void *a){
    struct dma_chncfg chncfg;
    int ret = 0;
    
    /* config addr */
    chncfg.src_addr = SRC_ADDR+0x1500000;
    chncfg.dst_addr = DST_ADDR+0x1500000;
    chncfg.len = 0x300000;
#ifdef AHB_TEST
        ret = gv_dma_start(fd_test, &chncfg);
#endif
    if(ret){
        printf("ioctl: config and start dma failed\n");
    }
    
    return NULL;

}

void * dma_i(void *a){
    struct dma_chncfg chncfg;
    int ret = 0;
    
    /* config addr */
    chncfg.src_addr = SRC_ADDR+0x1800000;
    chncfg.dst_addr = DST_ADDR+0x1800000;
    chncfg.len = 0x300000;
#ifdef AHB_TEST
        ret = gv_dma_start(fd_test, &chncfg);
#endif
    if(ret){
        printf("ioctl: config and start dma failed\n");
    }
    
    return NULL;

}

void * dma_j(void *a){
    struct dma_chncfg chncfg;
    int ret = 0;
    
    /* config addr */
    chncfg.src_addr = SRC_ADDR+0x1b00000;
    chncfg.dst_addr = DST_ADDR+0x1b00000;
    chncfg.len = 0x300000;
#ifdef AHB_TEST
        ret = gv_dma_start(fd_test, &chncfg);
#endif
    if(ret){
        printf("ioctl: config and start dma failed\n");
    }
    
    return NULL;

}

void * dma_k(void *a){
    struct dma_chncfg chncfg;
    int ret = 0;
    
    /* config addr */
    chncfg.src_addr = SRC_ADDR+0x1e00000;
    chncfg.dst_addr = DST_ADDR+0x1e00000;
    chncfg.len = 0x300000;
#ifdef AHB_TEST
        ret = gv_dma_start(fd_test, &chncfg);
#endif
    if(ret){
        printf("ioctl: config and start dma failed\n");
    }
    
    return NULL;

}

void * dma_l(void *a){
    struct dma_chncfg chncfg;
    int ret = 0;
    
    /* config addr */
    chncfg.src_addr = SRC_ADDR+0x2100000;
    chncfg.dst_addr = DST_ADDR+0x2100000;
    chncfg.len = 0x300000;
#ifdef AHB_TEST
        ret = gv_dma_start(fd_test, &chncfg);
#endif
    if(ret){
        printf("ioctl: config and start dma failed\n");
    }
    
    return NULL;

}

void * dma_m(void *a){
    struct dma_chncfg chncfg;
    int ret = 0;
    
    /* config addr */
    chncfg.src_addr = SRC_ADDR+0x2400000;
    chncfg.dst_addr = DST_ADDR+0x2400000;
    chncfg.len = 0x300000;
#ifdef AHB_TEST
        ret = gv_dma_start(fd_test, &chncfg);
#endif
    if(ret){
        printf("ioctl: config and start dma failed\n");
    }
    
    return NULL;

}

void * dma_n(void *a){
    struct dma_chncfg chncfg;
    int ret = 0;
    
    /* config addr */
    chncfg.src_addr = SRC_ADDR+0x2700000;
    chncfg.dst_addr = DST_ADDR+0x2700000;
    chncfg.len = 0x300000;
#ifdef AHB_TEST
        ret = gv_dma_start(fd_test, &chncfg);
#endif
    if(ret){
        printf("ioctl: config and start dma failed\n");
    }
    
    return NULL;

}

void * dma_o(void *a){
    struct dma_chncfg chncfg;
    int ret = 0;
    
    /* config addr */
    chncfg.src_addr = SRC_ADDR+0x2a00000;
    chncfg.dst_addr = DST_ADDR+0x2a00000;
    chncfg.len = 0x300000;
#ifdef AHB_TEST
        ret = gv_dma_start(fd_test, &chncfg);
#endif
    if(ret){
        printf("ioctl: config and start dma failed\n");
    }
    
    return NULL;

}

void * dma_p(void *a){
    struct dma_chncfg chncfg;
    int ret = 0;
    
    /* config addr */
    chncfg.src_addr = SRC_ADDR+0x2d00000;
    chncfg.dst_addr = DST_ADDR+0x2d00000;
    chncfg.len = 0x300000;
#ifdef AHB_TEST
        ret = gv_dma_start(fd_test, &chncfg);
#endif
    if(ret){
        printf("ioctl: config and start dma failed\n");
    }
    
    return NULL;

}




void  dma_sg( ){
    struct dma_chncfg_sg chncfg_sg;
    int ret;
    
    /* config addr */
    chncfg_sg.src_addr = SRC_ADDR;
    chncfg_sg.dst_addr = DST_ADDR;
    chncfg_sg.len = DMA_TEST_SG_LEN;
    chncfg_sg.src_stride = DMA_TEST_SG_SRC_STRIDE;
	chncfg_sg.dst_stride = DMA_TEST_SG_DST_STRIDE;
    chncfg_sg.high = DMA_TEST_SG_HIGH;
    
    ret = gv_hp_dma_start(fd_test, &chncfg_sg);
    if(ret){
        printf("ioctl: config and start dma failed\n");
    }
    
    return;

}

void  dma_multi_sg( ){
    struct dma_chncfg_multi_sg chncfg_multi_sg;
    int ret;
    
    /* config addr */
    chncfg_multi_sg.chncfg_sg[0].src_addr = SRC_ADDR;
    chncfg_multi_sg.chncfg_sg[0].dst_addr = DST_ADDR;
    chncfg_multi_sg.chncfg_sg[0].len = DMA_TEST_SG_LEN;
    chncfg_multi_sg.chncfg_sg[0].src_stride = DMA_TEST_SG_SRC_STRIDE;
	chncfg_multi_sg.chncfg_sg[0].dst_stride = DMA_TEST_SG_DST_STRIDE;
    chncfg_multi_sg.chncfg_sg[0].high = DMA_TEST_SG_HIGH;

    chncfg_multi_sg.chncfg_sg[1].src_addr = SRC_ADDR+2000000;
    chncfg_multi_sg.chncfg_sg[1].dst_addr = DST_ADDR+2000000;
    chncfg_multi_sg.chncfg_sg[1].len = DMA_TEST_SG_LEN;
    chncfg_multi_sg.chncfg_sg[1].src_stride = DMA_TEST_SG_SRC_STRIDE;
	chncfg_multi_sg.chncfg_sg[1].dst_stride = DMA_TEST_SG_DST_STRIDE;
    chncfg_multi_sg.chncfg_sg[1].high = DMA_TEST_SG_HIGH;

    chncfg_multi_sg.cnt = 2;

    ret = gv_hp_multi_dma_start(fd_test, &chncfg_multi_sg);
    if(ret){
        printf("ioctl: config and start dma failed\n");
    }
    
    return;

}

int main(void)
{
    int ret;
    pthread_t t0;
    pthread_t t1;
    pthread_t t2;
    pthread_t t3;
    pthread_t t4;
    pthread_t t5;
    pthread_t t6;
    pthread_t t7;
    pthread_t t8;
    pthread_t t9;
    pthread_t t10;
    pthread_t t11;
    pthread_t t12;
    pthread_t t13;
    pthread_t t14;
    pthread_t t15;
    void * result;
	
    printf("current pid:%d\n", getpid());
    printf("dma test, only support mem to mem: copy 0x60000000 to 0x70000000, size:48M\n");

    /* open dev */
	fd_test = gv_dma_device_init();
    if(fd_test < 0){
        printf("open %s failed\n", DRIVER_NAME);
        return -1;
    }
    //多线程测试
    init_mem_data();
    if(pthread_create(&t0, NULL, dma_a, NULL) == -1){
        printf("fail to create pthread t0");
        exit(1);
    }
    if(pthread_create(&t1, NULL, dma_b, NULL) == -1){
        printf("fail to create pthread t1");
        exit(1);
    }
    if(pthread_create(&t2, NULL, dma_c, NULL) == -1){
        printf("fail to create pthread t2");
        exit(1);
    }
    if(pthread_create(&t3, NULL, dma_d, NULL) == -1){
        printf("fail to create pthread t3");
        exit(1);
    }
    if(pthread_create(&t4, NULL, dma_e, NULL) == -1){
        printf("fail to create pthread t4");
        exit(1);
    }    
    if(pthread_create(&t5, NULL, dma_f, NULL) == -1){
        printf("fail to create pthread t0");
        exit(1);
    }
    if(pthread_create(&t6, NULL, dma_g, NULL) == -1){
        printf("fail to create pthread t1");
        exit(1);
    }
    if(pthread_create(&t7, NULL, dma_h, NULL) == -1){
        printf("fail to create pthread t2");
        exit(1);
    }
    if(pthread_create(&t8, NULL, dma_i, NULL) == -1){
        printf("fail to create pthread t3");
        exit(1);
    }        
    if(pthread_create(&t9, NULL, dma_j, NULL) == -1){
        printf("fail to create pthread t4");
        exit(1);
    }
    if(pthread_create(&t10, NULL, dma_k, NULL) == -1){
        printf("fail to create pthread t0");
        exit(1);
    }
    if(pthread_create(&t11, NULL, dma_l, NULL) == -1){
        printf("fail to create pthread t1");
        exit(1);
    }
    if(pthread_create(&t12, NULL, dma_m, NULL) == -1){
        printf("fail to create pthread t2");
        exit(1);
    }
    if(pthread_create(&t13, NULL, dma_n, NULL) == -1){
        printf("fail to create pthread t3");
        exit(1);
    }
    if(pthread_create(&t14, NULL, dma_o, NULL) == -1){
        printf("fail to create pthread t4");
        exit(1);
    }    
    if(pthread_create(&t15, NULL, dma_p, NULL) == -1){
        printf("fail to create pthread t0");
        exit(1);
    }
    
    if(pthread_join(t0, &result) == -1){
        printf("fail to recollect t0");
        exit(1);
    }
    if(pthread_join(t1, &result) == -1){
        printf("fail to recollect t1");
        exit(1);
    }
    if(pthread_join(t2, &result) == -1){
        printf("fail to recollect t2");
        exit(1);
    }
    if(pthread_join(t3, &result) == -1){
        printf("fail to recollect t3");
        exit(1);
    }
    if(pthread_join(t4, &result) == -1){
        printf("fail to recollect t4");
        exit(1);
    }
    if(pthread_join(t5, &result) == -1){
        printf("fail to recollect t0");
        exit(1);
    }
    if(pthread_join(t6, &result) == -1){
        printf("fail to recollect t1");
        exit(1);
    }
    if(pthread_join(t7, &result) == -1){
        printf("fail to recollect t2");
        exit(1);
    }
    if(pthread_join(t8, &result) == -1){
        printf("fail to recollect t3");
        exit(1);
    }
    if(pthread_join(t9, &result) == -1){
        printf("fail to recollect t3");
        exit(1);
    }
    if(pthread_join(t10, &result) == -1){
        printf("fail to recollect t4");
        exit(1);
    }
    if(pthread_join(t11, &result) == -1){
        printf("fail to recollect t0");
        exit(1);
    }
    if(pthread_join(t12, &result) == -1){
        printf("fail to recollect t1");
        exit(1);
    }
    if(pthread_join(t13, &result) == -1){
        printf("fail to recollect t2");
        exit(1);
    }
    if(pthread_join(t14, &result) == -1){
        printf("fail to recollect t3");
        exit(1);
    }
    if(pthread_join(t15, &result) == -1){
        printf("fail to recollect t4");
        exit(1);
    }
	printf("before verify_mem_data\n");
    verify_mem_data();

    //高性能    DMA 测试
    init_mem_data();
    dma_sg();
    verify_mem_sg_data();



    //高性能 非连续 DMA 测试
    init_mem_data();
    dma_multi_sg();
    verify_mem_multi_sg_data();

    gv_dma_device_deinit(fd_test);
    return 0;

}
#endif

