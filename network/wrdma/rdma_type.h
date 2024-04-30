#ifndef __RDMA_TYPE_H
#define __RDMA_TYPE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>
#include <endian.h>
#include <getopt.h>

#include <sys/time.h>
#include <arpa/inet.h>
#include <infiniband/verbs.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "list.h"
#define MAX_POLL_CQ_TIMEOUT 2000

#define RECV_MAX_WR_NUM  2000
#define SEND_MAX_WR_NUM  5000

#define SEND_BUF_LEN        (500*1024*1024)
#define WRITE_BUF_LEN       (500*1024*1024)
#define WRITE_BUF_RESERVE   32

#define RDMA_START 1
#define RDMA_STOP  0

#define ERR_LEVEL 0
#define DBG_LEVEL 3

#define ADDR_REMOTE_RECV_WR_NUM    0
#define ADDR_REMOTE_RECV_REPLY_CNT 0
#define ADDR_LOCAL_RECV_WR_NUM   8

#define IF_SEND_WR(id)      ((id)>10000 && (id)<(10000+SEND_BUF_NUM-1))
#define SEND_BUFF2WR_ID(id) ((id)+10000)
#define SEND_WR2BUFF_ID(id) ((id)-10000)

#define RECV_NUM2WR_ID(id) ((id)+20000)
#define RECV_WR2NUM_ID(id) ((id)-20000)

#define RDMADBG(msg, arg...) \
    do{\
        if(dlevel >= DBG_LEVEL) printf(msg, ##arg);\
    }while(0)
//#define RDMAW(msg, arg...) printf("%s %d "msg, __func__, __LINE__, ##arg);
#define RDMAERR(msg, arg...)  \
    do{\
        if(dlevel >= ERR_LEVEL) printf(msg, ##arg);\
    }while(0)


struct buffer_mgr {
	void *start_addr;
	unsigned long  slide_head;//curr_w;
	unsigned long  slide_tail;//slide_tail;
	unsigned long  size;
	unsigned long  left;
	unsigned long  tail_hole;
	uint64_t       alloc_cnt;
	uint64_t       free_cnt;
	struct dl_list buflist;
	pthread_mutex_t mtx;
	pthread_cond_t  cond;
};

struct buffer_entry {
	int offset;
	int len;
	int wr_id;
	struct dl_list list;
};

struct buf_hdr {
	int len;
	int offset;
	uint64_t alloc_cnt;
	char data[0];
};


struct remote_buf {
	void *start_addr;
	unsigned long  curr_w;
	unsigned long  curr_r;
	unsigned long  size;
	unsigned long  left;
	unsigned long tail_hole;
};

typedef enum {
	IN_USE,
	FREE
} STATUS;

typedef enum {
	BUF_TYPE_SEND,
	BUF_TYPE_RECV,
	BUF_TYPE_WRITE
} BTYPE;

struct recv_wr {
	struct ibv_recv_wr rr;
	struct ibv_recv_wr *bad_wr;
	struct ibv_sge sge;
};

#define BIT_REMOTE_BUF_FREED 1
#define BIT_LOCAL_BUF_FREED  2


struct send_wr {
	struct ibv_send_wr sr;
	struct ibv_sge sge;
	struct ibv_send_wr *bad_wr;
	uint64_t remote_addr;
	uint64_t local_addr;
	uint64_t la_cnt;
	uint64_t wr_id;
	struct dl_list list;
	uint32_t freedMask;
};

struct buf_info {
	int           len;
	int           id;
	BTYPE         type;
	STATUS        sta;
	struct ibv_mr *mr;
	union {
		struct recv_wr rwr;
		struct send_wr swr;
	} wrs;
} __attribute__((packed));

#define PKG_TYPE_CTRL 0
#define PKG_TYPE_DATA 1

struct pkg_hdr {
	int type: 2;
	int len: 30;
} __attribute__((packed));

struct rdma_buf {
	struct buf_info info;
	char            buf[0];
} __attribute__((packed));

#define GET_BUFF_HDR(A) ((void*)(A)-sizeof(struct buf_info))

struct cm_con_data_t {
	uint64_t addr;   /* Buffer address */
	uint32_t rkey;   /* Remote key */
	uint32_t qp_num; /* QP number */
	uint16_t lid;   /* LID of the IB port */
	uint8_t gid[16]; /* gid */
} __attribute__((packed));

struct rdma_cb {
	int (*recv_cb)(void *priv, char *data, int len);
	int (*error_cb)(void *priv, int errno);
	void *cb_priv;
};

struct resources {
	struct ibv_device_attr
		device_attr;
	/* Device attributes */
	struct ibv_port_attr port_attr; /* IB port attributes */
	struct cm_con_data_t remote_props; /* values to connect to remote side */
	struct ibv_context *ib_ctx;        /* device handle */
	struct ibv_pd *pd;                 /* PD handle */
	struct ibv_cq *send_cq;                /* send CQ handle */
	struct ibv_cq *recv_cq;                /* recv CQ handle */
	struct ibv_cq *cq;                 /* recv CQ handle */
	struct ibv_comp_channel *cc;
	struct ibv_qp *qp;                 /* QP handle */
	int free_sd_buf;
	int free_rv_buf;
	int sock;                          /* TCP socket file descriptor */
	struct rdma_cb *cb_handle;

	pthread_mutex_t sd_mut;
	pthread_cond_t  sd_cond;
	pthread_mutex_t rv_mut;
	pthread_cond_t  rv_cond;
	pthread_t       event_tid;

	pthread_mutex_t stop_mut;
	pthread_cond_t  stop_cond;

	int status;
	uint64_t send_cnt;
	void *local_send;
	void *local_write;
	uint64_t remote_write;
	uint32_t remote_wtkey;
	struct ibv_mr *mr_lsd;
	struct ibv_mr *mr_lwt;
	struct ibv_mr *mr_rwt;
	struct buffer_mgr bm_lsd;
	struct buffer_mgr bm_rwt;
	struct dl_list swrlist;
	struct recv_wr rwr[RECV_MAX_WR_NUM];
	struct send_wr replywr;
	int consume_swr;
	int reply_pending;
};

extern struct resources *gRdma_handle;

int sock_srv_init(const char *servername, int port);
int sock_srv_accpt(int sockfd);
int sock_connect(const char *servername, int port);
void* rdma_alloc_send_buf(struct resources *res, int len);
int rdma_send_data(struct resources *res, char *data, int len);
/*struct resources*  rdma_srv_init(int sockfd, char *dev_name, int ib_port, int gid_idx); */
/*struct resources*  rdma_cli_init(char *dev_name, int sockfd, int ib_port, int gid_idx); */
void rdma_regist_cb(struct resources *res, struct rdma_cb *handle);
int rdma_start_event_thread(struct resources *res);
void rdma_deinit(struct resources *res);

#endif

