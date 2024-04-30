#include <stdio.h>
#include <stdlib.h>
#include <byteswap.h>

#include "rdma_type.h"

struct resources *gRdma_handle;
static int srv_sock = -1;
int dlevel = 3;

#if __BYTE_ORDER == __LITTLE_ENDIAN
static inline uint64_t htonll(uint64_t x) {	return bswap_64(x); }
static inline uint64_t ntohll(uint64_t x) {	return bswap_64(x); }
#elif __BYTE_ORDER == __BIG_ENDIAN
static inline uint64_t htonll(uint64_t x) {	return x; }
static inline uint64_t ntohll(uint64_t x) {	return x; }
#else
#error __BYTE_ORDER is neither __LITTLE_ENDIAN nor __BIG_ENDIAN
#endif

void dump_bm_info(struct buffer_mgr *bm)
{

	RDMADBG("start addr:%p\n", bm->start_addr);
	RDMADBG("      size:%ld\n", bm->size);
	RDMADBG("      left:%ld\n", bm->left);
	RDMADBG("      head:%ld\n", bm->slide_head);
	RDMADBG("      tail:%ld\n", bm->slide_tail);
	RDMADBG(" tail_hole:%ld\n", bm->tail_hole);

}

struct buffer_entry* new_buffer_entry(struct buffer_mgr *bm, int offset, int len, int wr_id)
{
	struct buffer_entry *entry = malloc(sizeof(struct buffer_entry));

	if(!entry) {
		return NULL;
	}

	entry->offset = offset;
	entry->len = len;
	entry->wr_id = wr_id;
	dl_list_add_tail(&bm->buflist, &entry->list);
	return entry;
}

struct buffer_entry* get_front_buffer_entry(struct buffer_mgr *rb)
{
	return dl_list_first(&rb->buflist, struct buffer_entry, list);
}


void del_buffer_entry(struct buffer_entry *be)
{
	dl_list_del(&be->list);
	free(be);
}

int buffer_mgr_init(struct buffer_mgr *bm, void *startaddr, int buff_size)
{
	bm->start_addr = startaddr;

	bm->size = buff_size;
	bm->left = buff_size;
	bm->slide_tail = bm->slide_head = 0;
	bm->tail_hole = 0;
	bm->alloc_cnt = 0;
	bm->free_cnt = 0;
	pthread_mutex_init(&bm->mtx, NULL);
	pthread_cond_init(&bm->cond, NULL);

	dl_list_init(&bm->buflist);
	return 0;
}

unsigned int bm_addr_to_offset(struct buffer_mgr *bm, void *addr)
{
	return addr - bm->start_addr;
}

void *bm_offset_to_addr(struct buffer_mgr *bm, unsigned long offset)
{
	return bm->start_addr + offset;
}

void* bm_buffer_alloc(struct buffer_mgr *bm, int size, int block, int wr_id)
{
	unsigned long cur_offset;

	RDMADBG("bm_buffer_alloc len:%d\n", size);

	pthread_mutex_lock(&bm->mtx);
	dump_bm_info(bm);

realloc:

	if(bm->left < (unsigned long)size) {
		RDMAERR("have no enough memory!\n");
		dump_bm_info(bm);

		if(block) {
			RDMADBG("waiting for free memory...\n");
			pthread_cond_wait(&bm->cond, &bm->mtx);
			goto realloc;

		} else {
			pthread_mutex_unlock(&bm->mtx);
			return NULL;
		}
	}

	if(bm->slide_tail <= bm->slide_head) { /* |----tail-------head-------|*/
		if((bm->size - bm->slide_head) >= (unsigned long)size) {
			//memcpy(bp->pbuf+bp->slide_head, data, len);
			cur_offset = bm->slide_head;

			if((bm->slide_head += size) > (bm->size - 1)) {
				bm->slide_head = 0;
			}

			bm->left -= size;

		} else {                  /* |head-----tail-------|---hole--|*/
			if(bm->slide_tail < (unsigned long)size) {
				RDMADBG("have no enough memory! because of hole.\n");
				dump_bm_info(bm);

				if(block) {
					RDMADBG("waiting for free memory...\n");
					pthread_cond_wait(&bm->cond, &bm->mtx);
					goto realloc;

				} else {
					pthread_mutex_unlock(&bm->mtx);
					return NULL;
				}
			}

			bm->left -= (bm->size - bm->slide_head);
			cur_offset = 0;
			bm->tail_hole = bm->slide_head;
			bm->slide_head = size;
			bm->left -= size;
		}

	} else { /* |----head-------tail--------|*/
		if((bm->slide_tail - bm->slide_head) < (unsigned long)size) {
			RDMADBG("have no enough memory! because of hole.\n");
			dump_bm_info(bm);

			if(block) {
				RDMADBG("waiting for free memory...\n");
				pthread_cond_wait(&bm->cond, &bm->mtx);
				goto realloc;

			} else {
				pthread_mutex_unlock(&bm->mtx);
				return NULL;
			}
		}

		//memcpy(bm->pbuf+bm->slide_head, data, len);
		cur_offset = bm->slide_head;
		bm->slide_head += size;
		bm->left -= size;
	}

	/*    if(bm->left <0){
	        RDMAERR("left <0,why?\n");
	        dump_bm_info(bm);
	        exit(0);
	    }*/

	if(!new_buffer_entry(bm, cur_offset, size, wr_id)) {
		RDMAERR("add new buffer entry error!\n");
		pthread_mutex_unlock(&bm->mtx);
		exit(0);
	}

	dump_bm_info(bm);
	RDMADBG("bm_buffer_alloc completed.\n");

	pthread_mutex_unlock(&bm->mtx);

	return bm_offset_to_addr(bm, cur_offset);

}

void bm_clear_hole(struct buffer_mgr *bm)
{
	bm->left += (bm->size - bm->tail_hole);
	//bm->slide_tail = bm->tail_hole;
	bm->tail_hole = 0;
}


int bm_buff_free_entry(struct buffer_mgr *bm, struct buffer_entry *entry)
{
	unsigned int old_tail = bm->slide_tail;

	RDMADBG("bm_buff_free_entry:(%d), len:%d\n", entry->offset, entry->len);
	dump_bm_info(bm);

	bm->left += entry->len;
	del_buffer_entry(entry);

	entry = get_front_buffer_entry(bm);

	if(!entry) {
		bm->slide_tail = bm->slide_head;

	} else {
		bm->slide_tail = entry->offset;
	}

	if(bm->slide_tail < old_tail && bm->tail_hole > 0) {
		bm_clear_hole(bm);
	}

	dump_bm_info(bm);

	RDMADBG("bm_buff_free_entry completed.\n");
	return 0;
}

static void bm_free_unused_entry(struct buffer_mgr *bm, int max_wrid)
{
	struct buffer_entry *be, *n;

	pthread_mutex_lock(&bm->mtx);
	dl_list_for_each_safe(be, n, &bm->buflist, struct buffer_entry, list) {
		if(!be) {
			break;
		}

		if(be->wr_id > max_wrid) {
			break;
		}

		RDMADBG("rdma_free_remote_buf wr_id:%d, remote addr:%d\n", be->wr_id, be->offset);
		bm_buff_free_entry(bm, be);
	}

	pthread_cond_broadcast(&bm->cond);
	pthread_mutex_unlock(&bm->mtx);
}


int bm_buffer_free(struct buffer_mgr *bm, void *addr)
{
	struct buffer_entry *entry;
	unsigned int old_tail;

	pthread_mutex_lock(&bm->mtx);
	entry = get_front_buffer_entry(bm);

	if(!entry) {
		RDMAERR("empty buffer list\n");
		dump_bm_info(bm);
		pthread_mutex_unlock(&bm->mtx);
		exit(0);
	}

	RDMADBG("bm_buffer_free:%p(%d), len:%d\n", addr, entry->offset, entry->len);
	dump_bm_info(bm);
	old_tail = bm->slide_tail;

	if(bm_offset_to_addr(bm, entry->offset) != addr) {
		RDMAERR("err addr:%p, tail entry:%p(%d)\n", addr, (void*)(bm->start_addr + entry->offset), entry->offset);
		dump_bm_info(bm);
		pthread_mutex_unlock(&bm->mtx);
		exit(0);
	}

	if(bm->left == bm->size) {
		RDMAERR("empty!!!\n");
		dump_bm_info(bm);
		pthread_mutex_unlock(&bm->mtx);
		return 0;
	}

	bm->left += entry->len;
	del_buffer_entry(entry);

	entry = get_front_buffer_entry(bm);

	if(!entry) {
		bm->slide_tail = bm->slide_head;

	} else {
		bm->slide_tail = entry->offset;
	}

	if(bm->slide_tail < old_tail && bm->tail_hole > 0) {
		bm_clear_hole(bm);
	}

	dump_bm_info(bm);
	RDMADBG("bm_buffer_free completed.\n");
	pthread_cond_broadcast(&bm->cond);
	pthread_mutex_unlock(&bm->mtx);
	return 0;
}

static void* get_local_write_entry(struct resources *res, int offset)
{
	if(offset >= WRITE_BUF_LEN) {
		return NULL;
	}

	return (void*)(res->local_write + offset);
}

static void* get_remote_write_entry(struct resources *res, int offset)
{
	if(offset >= WRITE_BUF_LEN) {
		return NULL;
	}

	return (void*)(res->remote_write + offset);
}

static int check_remote_recv(struct resources *res)
{
	uint64_t *remote_recv = get_local_write_entry(res, ADDR_REMOTE_RECV_REPLY_CNT);
	uint64_t wait_cnt = 0;

	__sync_add_and_fetch(&res->consume_swr, 1);

	RDMADBG("now remote recv wr NUM:%ld, send_cnt:%ld, consume:%d\n", (uint64_t)*remote_recv, res->send_cnt, res->consume_swr);

	while((uint64_t)(__sync_add_and_fetch(remote_recv, 0) + RECV_MAX_WR_NUM) <= res->send_cnt \
		  || __sync_add_and_fetch(&res->consume_swr, 0) > SEND_MAX_WR_NUM) {
		usleep(1000);
		wait_cnt++;
	}

	if(wait_cnt > 0) {
		RDMAERR("check_remote_recv waiting count:%ld\n", wait_cnt);
	}

	return 0;
}

static int recv_reply(struct resources *res, int cur_cnt)
{
	struct send_wr *swr = &res->replywr;
	int rc = 0;
	RDMADBG("recv_reply.\n");

	__sync_add_and_fetch(&res->consume_swr, 1);

	if(res->consume_swr > SEND_MAX_WR_NUM) {
		RDMAERR("cannot reply when consuem is:%d\n", res->consume_swr);
		__sync_sub_and_fetch(&res->consume_swr, 1);
		res->reply_pending += cur_cnt;
		return 0;
	}

	/* prepare the scatter/gather entry */
	memset(&swr->sge, 0, sizeof(swr->sge));
	swr->sge.addr = (uintptr_t)get_local_write_entry(res, ADDR_LOCAL_RECV_WR_NUM);
	swr->sge.length = 8;
	swr->sge.lkey = res->mr_lwt->lkey;
	/* prepare the send work request */
	memset(&swr->sr, 0, sizeof(swr->sr));
	swr->sr.next = NULL;
	swr->sr.wr_id = 0;
	swr->sr.sg_list = &swr->sge;
	swr->sr.num_sge = 1;
	swr->sr.opcode = IBV_WR_ATOMIC_FETCH_AND_ADD;
	swr->sr.send_flags = IBV_SEND_SIGNALED;
	swr->sr.wr.atomic.remote_addr = (uintptr_t)get_remote_write_entry(res, ADDR_REMOTE_RECV_REPLY_CNT);
	swr->sr.wr.atomic.rkey = res->remote_wtkey;
	swr->sr.wr.atomic.compare_add = (cur_cnt + res->reply_pending);

	/* there is a Receive Request in the responder side, so we won't get any into RNR flow */
	rc = ibv_post_send(res->qp, &swr->sr, &swr->bad_wr);

	if(rc) {
		RDMAERR("failed to post SR\n");
		res->reply_pending += cur_cnt;
		return -1;
	}

	res->reply_pending = 0;

	return rc;
}

static void rdma_free_remote_buf(struct resources *res)
{
	struct buffer_entry *be, *n;
	uint64_t *remote_recv_reply = get_local_write_entry(res, ADDR_REMOTE_RECV_REPLY_CNT);
	RDMADBG("rdma_free_remote_buf, now remote reply:%ld\n", (*remote_recv_reply));

	bm_free_unused_entry(&res->bm_rwt, (int)*remote_recv_reply);

	return;
}

static void rdma_free_send_buf(struct resources *res, int wr_id)
{
	struct send_wr *swr, *n;

	RDMADBG("rdma_free_send_buf:%d\n", wr_id);

	pthread_mutex_lock(&res->sd_mut);
	dl_list_for_each_safe(swr, n, &res->swrlist, struct send_wr, list) {
		if(swr->wr_id <= (uint64_t)wr_id) {
			RDMADBG("find wr id:%d alloc_cnt:%ld when free_cnt is:%ld\n", wr_id, swr->la_cnt, res->bm_lsd.free_cnt);

			if(swr->la_cnt == res->bm_lsd.free_cnt) {
				bm_buffer_free(&res->bm_lsd, (void*)swr->local_addr);
				res->bm_lsd.free_cnt++;
				dl_list_del(&swr->list);
				free(swr);
			}

		} else {
			break;
		}
	}

	pthread_mutex_unlock(&res->sd_mut);
	return;
}

static int post_recv_wr(struct resources *res, int rwr_num)
{

	struct recv_wr *rwr;

	if(rwr_num >= RECV_MAX_WR_NUM) {
		return -1;
	}

	rwr = &res->rwr[rwr_num];

	RDMADBG("post_recv_wr %p rwr_num:%d\n", res, rwr_num);

	/* prepare the receive work request */
	memset(&rwr->rr, 0, sizeof(rwr->rr));
	rwr->rr.next = NULL;
	rwr->rr.wr_id = rwr_num;
	rwr->rr.sg_list = NULL;
	rwr->rr.num_sge = 0;

	/* post the Receive Request to the RQ */
	if(ibv_post_recv(res->qp, &rwr->rr, &rwr->bad_wr)) {
		RDMAERR("failed to post RR\n");
		return -1;
	}

	RDMADBG("Receive Request was posted\n");
	return 0;
}

/*only for wake up event loop*/
static int post_err_wr(struct resources *res)
{
	struct ibv_send_wr sr;
	struct ibv_sge sge;
	struct ibv_send_wr *bad_wr = NULL;
	int rc;
	static unsigned ck = 1;

	memset(&sge, 0, sizeof(sge));
	sge.addr = (uintptr_t)res->local_send;
	sge.length = 1;
	sge.lkey = 0;

	memset(&sr, 0, sizeof(sr));
	sr.next = NULL;
	sr.wr_id = 0;
	sr.sg_list = &sge;
	sr.num_sge = 1;
	sr.opcode = IBV_WR_SEND;
	sr.send_flags = IBV_SEND_SIGNALED;

	rc = ibv_post_send(res->qp, &sr, &bad_wr);

	if(rc)
		RDMAERR("failed to post SR\n");

	return rc;

}
static int post_write_wr(struct resources *res, uint64_t addr, int len)
{
	int rc = 0;
	struct send_wr *swr;
	struct buf_hdr *bh = (struct buf_hdr*)addr;

	rdma_free_remote_buf(res);

	if(check_remote_recv(res)) {
		return -1;
	}

	swr = malloc(sizeof(struct send_wr));

	if(!swr) {
		return -1;
	}

	swr->la_cnt = bh->alloc_cnt;
	swr->local_addr = addr;
	swr->wr_id = res->send_cnt;

	while(!(swr->remote_addr = (uint64_t)bm_buffer_alloc(&res->bm_rwt, len, 0, swr->wr_id))) {
		usleep(500);
		rdma_free_remote_buf(res);
	}

	swr->freedMask = 0;

	if(!swr->remote_addr) {
		free(swr);
		return -1;
	}

	/* prepare the scatter/gather entry */
	memset(&swr->sge, 0, sizeof(swr->sge));
	swr->sge.addr = (uintptr_t)addr;
	swr->sge.length = len;
	swr->sge.lkey = res->mr_lsd->lkey;
	/* prepare the send work request */
	memset(&swr->sr, 0, sizeof(swr->sr));
	swr->sr.next = NULL;
	swr->sr.wr_id = swr->wr_id;
	swr->sr.sg_list = &swr->sge;
	swr->sr.num_sge = 1;
	swr->sr.opcode = IBV_WR_RDMA_WRITE_WITH_IMM;
	swr->sr.send_flags = IBV_SEND_SIGNALED;

	swr->sr.imm_data = bm_addr_to_offset(&res->bm_rwt, (void*)swr->remote_addr);
	swr->sr.wr.rdma.remote_addr = swr->remote_addr;
	swr->sr.wr.rdma.rkey = res->remote_wtkey;
	RDMADBG(" send info:\n"\
			"     wr_id:%ld\n"\
			" local addr:%p(%d)\n"\
			"remote addr:%p(%d)\n"\
			"        len:%d\n",
			swr->wr_id, (void*)swr->local_addr, \
			bm_addr_to_offset(&res->bm_lsd, (void*)swr->local_addr), (void*)swr->remote_addr, swr->sr.imm_data, len);

	pthread_mutex_lock(&res->sd_mut);
	dl_list_add_tail(&res->swrlist, &swr->list);
	pthread_mutex_unlock(&res->sd_mut);

	/* there is a Receive Request in the responder side, so we won't get any into RNR flow */
	rc = ibv_post_send(res->qp, &swr->sr, &swr->bad_wr);

	if(rc) {
		RDMAERR("failed to post SR\n");
		free(swr);
		exit(0);
	}

	return rc;

}


static void proc_send_cq(struct resources *res, struct ibv_wc *wc)
{

	RDMADBG("-------------------------------------------------proc_send_cq id:%ld consume:%d\n", wc->wr_id, res->consume_swr);
	__sync_sub_and_fetch(&res->consume_swr, 1);

	if(res->reply_pending > 0) {
		RDMADBG("----------------------------pending:%d\n", res->reply_pending);
		recv_reply(res, 0);
	}

	rdma_free_send_buf(res, wc->wr_id);
}

static void proc_recv_cq(struct resources *res, struct ibv_wc *wc)
{
	struct rdma_cb *cb = res->cb_handle;

	int offset = wc->imm_data;
	struct buf_hdr *bh = res->local_write + WRITE_BUF_RESERVE + offset;

	RDMADBG("----------------------------------------------------proc_recv_cq id:%ld, imm:%d %p\n", wc->wr_id, wc->imm_data, res->local_write);

	if(cb) {
		cb->recv_cb(cb->cb_priv, bh->data, bh->len);
	}

	post_recv_wr(res, wc->wr_id);
	recv_reply(res, 1);
}

static void proc_reply_cq(struct resources *res, struct ibv_wc *wc)
{
	RDMADBG("--------------------------------------------------proc_reply_cq, consume:%d\n", res->consume_swr);
	__sync_sub_and_fetch(&res->consume_swr, 1);

	if(res->reply_pending > 0) {
		RDMADBG("----------------------------pending:%d\n", res->reply_pending);
		recv_reply(res, 0);
	}

}

int get_completion_wc(struct ibv_cq *cq, struct ibv_wc *wc)
{
	unsigned long start_time_msec;
	unsigned long cur_time_msec;
	struct timeval cur_time;
	int poll_result;
	int rc = 1;
	/* poll the completion for a while before giving up of doing it .. */
	gettimeofday(&cur_time, NULL);
	start_time_msec = (cur_time.tv_sec * 1000) + (cur_time.tv_usec / 1000);

	do {
		poll_result = ibv_poll_cq(cq, 1, wc);
		gettimeofday(&cur_time, NULL);
		cur_time_msec = (cur_time.tv_sec * 1000) + (cur_time.tv_usec / 1000);
	} while(0);

	//} while ((poll_result == 0) && ((cur_time_msec - start_time_msec) < MAX_POLL_CQ_TIMEOUT));
	if(poll_result < 0) {
		/* poll CQ failed */
		RDMAERR("--------------------------------------------poll CQ failed\n");
		rc = 0;

	} else if(poll_result == 0) {
		/* the CQ is empty */
		RDMADBG("-------------------------------------------completion wasn't found in the CQ after timeout\n");
		rc = 0;

	} else {
		/* CQE found */
		//RDMADBG("-------------------------------------------completion was found in CQ with status 0x%x\n", wc->status);
		/* check the completion status (here we don't care about the completion opcode */
	}

	return rc;
}

static void* event_loop(void *ptr)
{
	int ret;
	struct resources *res = (struct resources*)ptr;
	struct ibv_cq *cq;
	void *ev_ctx;
	struct ibv_wc wc;
	int i;
	struct rdma_cb *cb;

	RDMADBG("start send all recv wr\n");


	cb = res->cb_handle;

	/*post all receive wr*/
	for(i = 0; i < RECV_MAX_WR_NUM; i++) {
		post_recv_wr(res, i);
	}

	RDMADBG("end send all recv wr\n");

	/*start receive data loop*/
	while(res->status == RDMA_START) {
		ret = ibv_get_cq_event(res->cc, &cq, &ev_ctx);

		if(ret) {
			RDMAERR("------------------------------------------Failed to get CQ event\n");
			break;
		}

		RDMADBG("---------------------------------------------get event\n");

		/* Ack the event */
		ibv_ack_cq_events(cq, 1);

		ret = ibv_req_notify_cq(res->cq, 0);

		if(ret) {
			RDMAERR("-------------------------------------Failed to ibv_req_notify_cq\n");
			break;
		}

		while(get_completion_wc(res->cq, &wc)) { // send and recv all need wait here and need lock
			if(wc.status != IBV_WC_SUCCESS) {
				RDMAERR("----------------------------got bad cq (%ld) with status: 0x%x, vendor syndrome: 0x%x\n", wc.wr_id, wc.status,
						wc.vendor_err);
				//cb->error_cb(cb->cb_priv, 0);
				//return NULL;
				break;
			}

			if(wc.opcode == IBV_WC_RECV_RDMA_WITH_IMM) {
				proc_recv_cq(res, &wc);

			} else if(wc.opcode ==  IBV_WC_RDMA_WRITE) {
				proc_send_cq(res, &wc);

			} else if(wc.opcode == IBV_WC_FETCH_ADD) {
				proc_reply_cq(res, &wc);

			} else {
				RDMAERR("-------------------------------------err opcode:%d\n", wc.opcode);
			}

			rdma_free_remote_buf(res);
		}

	}

	pthread_mutex_lock(&res->stop_mut);
	pthread_cond_broadcast(&res->stop_cond);
	pthread_mutex_unlock(&res->stop_mut);

	RDMAERR("event loop exit!!!\n");

	cb->error_cb(cb->cb_priv, 0);

	return NULL;
}

int sock_sync_data(int sock, int xfer_size, char *local_data, char *remote_data)
{
	int rc;
	int read_bytes = 0;
	int total_read_bytes = 0;
	int retry_cnt = 0;

	rc = write(sock, local_data, xfer_size);

	if(rc < xfer_size)
		RDMAERR("Failed writing data during sock_sync_data\n");

	else
		rc = 0;

	while(!rc && total_read_bytes < xfer_size && retry_cnt < 10) {
		read_bytes = read(sock, remote_data, xfer_size);

		if(read_bytes > 0)
			total_read_bytes += read_bytes;

		else
			rc = read_bytes;

		if(rc == 0)
			retry_cnt++;
	}

	if(rc == 0 && retry_cnt > 1)
		RDMAERR("Socket Read data failed. retry_cnt=%d\n", retry_cnt);

	return rc;
}


void rdma_stop_event_thread(struct resources *res)
{
	RDMAERR("rdma_stop_event_thread...\n");

	res->status = RDMA_STOP;
}

int rdma_start_event_thread(struct resources *res)
{
	char temp_char;
	int i;
	uint64_t *remote_recv = get_local_write_entry(res, ADDR_REMOTE_RECV_REPLY_CNT);

	res->status = RDMA_START;


	if(pthread_create(&res->event_tid, 0, event_loop, (void*)res)) {
		RDMAERR("Failed to create event loop thread.\n");
		res->status = RDMA_STOP;
		return -1;
	}

	sleep(1);
	*remote_recv = 0;
	res->send_cnt = 1;
	res->consume_swr = 0;
	res->reply_pending = 0;

	if(sock_sync_data(res->sock, 1, "R", &temp_char)) { /* just send a dummy char back and forth */
		RDMAERR("sync error after start event loop.\n");
	}

	return 0;
}

void rdma_destroy_event_thread(struct resources *res)
{
	RDMAERR("destroy event thread ...\n");

	int e;
	struct timespec abstime;
	struct timeval now;
	long timeout_ms = 500; // wait time 500ms
	gettimeofday(&now, NULL);
	long nsec = now.tv_usec * 1000 + (timeout_ms % 1000) * 1000000;
	abstime.tv_sec = now.tv_sec + nsec / 1000000000 + timeout_ms / 1000;
	abstime.tv_nsec = nsec % 1000000000;

	pthread_mutex_lock(&res->stop_mut);
	e = pthread_cond_timedwait(&res->stop_cond, &res->stop_mut, &abstime);
	pthread_mutex_unlock(&res->stop_mut);

	if(e == ETIMEDOUT) {
		RDMAERR("Timed out waiting for thread to exit.\n");
		pthread_cancel(res->event_tid);

	} else {
		pthread_join(res->event_tid, NULL);
	}

}

void* rdma_alloc_send_buf(struct resources *res, int len)
{
	void *addr;
	struct buf_hdr *bh;

	RDMADBG("rdma_alloc_send_buf:%d\n", len);

	if(len > SEND_BUF_LEN) {
		RDMADBG("cannot alloc memory bigger than %d.\n", SEND_BUF_LEN);
		return NULL;
	}

	if(!res) {
		return NULL;
	}

	bh = bm_buffer_alloc(&res->bm_lsd, len + sizeof(struct buf_hdr), 1, 0);

	if(!bh) {
		return NULL;
	}

	bh->len = len;
	bh->offset = bm_addr_to_offset(&res->bm_lsd, (void*)bh);
	bh->alloc_cnt = res->bm_lsd.alloc_cnt;

	res->bm_lsd.alloc_cnt++;

	RDMADBG("rdma_alloc_send_buf alloc addr:%x, len:%d\n", bh->offset, bh->len);
	return bh->data;
}

int rdma_send_data(struct resources *res, char *data, int len)
{
	struct buf_hdr *bh = (struct buf_hdr*)(data - sizeof(struct buf_hdr));

	if(len > SEND_BUF_LEN) {
		RDMAERR("send too long size:%d.(addr:%p)\n", len, data);
		return -1;
	}

	if(bh->len != len || (unsigned int)bh->offset != bm_addr_to_offset(&res->bm_lsd, bh)) {
		RDMAERR("err addr:%p,len:%d local send bm infor:\n", data, len);
		dump_bm_info(&res->bm_lsd);
		return -1;
	}

	RDMADBG("rdma_send_data addr:%p len:%d\n", data, len);

	post_write_wr(res, (uint64_t)bh, len + sizeof(struct buf_hdr));

	res->send_cnt++;
	return len;
}


static int rdma_buf_init(struct resources *res)
{
	int i;
	int mr_flags = 0;

	res->local_send = malloc(SEND_BUF_LEN);

	if(!res->local_send) {
		RDMAERR("failed to malloc %d bytes to memory buffer\n", SEND_BUF_LEN);
		return -1;
	}

	mr_flags = IBV_ACCESS_LOCAL_WRITE | IBV_ACCESS_REMOTE_READ | IBV_ACCESS_REMOTE_WRITE | IBV_ACCESS_REMOTE_ATOMIC;
	res->mr_lsd = ibv_reg_mr(res->pd, res->local_send, SEND_BUF_LEN, mr_flags);

	if(!res->mr_lsd) {
		RDMAERR("ibv_reg_mr failed with mr_flags=0x%x\n", mr_flags);
		free(res->local_send);
		return -1;
	}

	buffer_mgr_init(&res->bm_lsd, res->local_send, SEND_BUF_LEN);

	res->local_write = malloc(WRITE_BUF_LEN);

	if(!res->local_write) {
		RDMAERR("failed to malloc %d bytes to memory buffer\n", SEND_BUF_LEN);
		return -1;
	}

	mr_flags = IBV_ACCESS_LOCAL_WRITE | IBV_ACCESS_REMOTE_READ | IBV_ACCESS_REMOTE_WRITE | IBV_ACCESS_REMOTE_ATOMIC;
	res->mr_lwt = ibv_reg_mr(res->pd, res->local_write, WRITE_BUF_LEN, mr_flags);

	if(!res->mr_lwt) {
		RDMAERR("ibv_reg_mr failed with mr_flags=0x%x\n", mr_flags);
		free(res->local_write);
		free(res->local_send);
		return -1;
	}

	dl_list_init(&res->swrlist);

	pthread_cond_init(&res->sd_cond, NULL);
	pthread_mutex_init(&res->sd_mut, NULL);

	pthread_cond_init(&res->stop_cond, NULL);
	pthread_mutex_init(&res->stop_mut, NULL);

	return 0;
}

int rdma_buf_deinit(struct resources *res)
{
	int i;
	int mr_flags = 0;

	ibv_dereg_mr(res->mr_lsd);
	free(res->local_send);

	ibv_dereg_mr(res->mr_lwt);
	free(res->local_write);

	return 0;
}

int ibv_init(struct resources *res, char *dev_name, int ib_port)
{
	struct ibv_device **dev_list = NULL;
	struct ibv_qp_init_attr qp_init_attr;
	struct ibv_device *ib_dev = NULL;
	size_t size;
	int i;
	int num_devices;
	int rc;
	struct ibv_device_attr device_attr;

	RDMADBG("searching for IB devices in host\n");
	/* get device names in the system */
	dev_list = ibv_get_device_list(&num_devices);

	if(!dev_list) {
		RDMAERR("failed to get IB devices list\n");
		return -1;
	}

	/* if there isn't any IB device in host */
	if(!num_devices) {
		RDMAERR("found %d device(s)\n", num_devices);
		return -1;
	}

	RDMADBG("found %d device(s)\n", num_devices);

	/* search for the specific device we want to work with */
	for(i = 0; i < num_devices; i++) {
		if(!strcmp(ibv_get_device_name(dev_list[i]), dev_name)) {
			ib_dev = dev_list[i];
			break;
		}
	}

	/* if the device wasn't found in host */
	if(!ib_dev) {
		ibv_free_device_list(dev_list);
		RDMAERR("IB device %s wasn't found\n", dev_name);
		return -1;
	}

	/* get device handle */

	res->ib_ctx = ibv_open_device(ib_dev);
	ibv_free_device_list(dev_list);

	if(!res->ib_ctx) {
		RDMAERR("failed to open device %s\n", dev_name);
		return -1;
	}

	/* We are now done with device list, free it */

	if(ibv_query_device(res->ib_ctx, &device_attr)) {
		RDMAERR("ibv_query_device %u failed\n", ib_port);
		return -1;
	}

	RDMAERR("ibv_query_device atomic_cap:%d, %d, %d, %d \n", device_attr.atomic_cap, IBV_ATOMIC_NONE, IBV_ATOMIC_HCA, IBV_ATOMIC_GLOB);

	/* query port properties */
	if(ibv_query_port(res->ib_ctx, ib_port, &res->port_attr)) {
		RDMAERR("ibv_query_port on port %u failed\n", ib_port);
		return -1;
	}

	/* allocate Protection Domain */
	res->pd = ibv_alloc_pd(res->ib_ctx);

	if(!res->pd) {
		RDMAERR("ibv_alloc_pd failed\n");
		return -1;
	}

	res->cc = ibv_create_comp_channel(res->ib_ctx);

	if(!res->pd) {
		RDMAERR("ibv_alloc_pd failed\n");
		return -1;
	}

	/* each side will send only one WR, so Completion Queue with 1 entry is enough */
	res->cq = ibv_create_cq(res->ib_ctx, (RECV_MAX_WR_NUM + SEND_MAX_WR_NUM) * 2, NULL, res->cc, 0);

	if(!res->cq) {
		RDMAERR("failed to create CQ with %u entries\n", (RECV_MAX_WR_NUM + SEND_MAX_WR_NUM) * 2);
		return -1;
	}

	rc = ibv_req_notify_cq(res->cq, 0);

	if(rc) {
		RDMAERR("failed to create CQ with %u entries\n", (RECV_MAX_WR_NUM + SEND_MAX_WR_NUM) * 2);
		return -1;
	}

	/* allocate the memory buffer that will hold the data */
	/* create the Queue Pair */
	memset(&qp_init_attr, 0, sizeof(qp_init_attr));
	qp_init_attr.qp_type = IBV_QPT_RC;
	qp_init_attr.sq_sig_all = 1;
	qp_init_attr.send_cq = res->cq;
	qp_init_attr.recv_cq = res->cq;
	qp_init_attr.cap.max_send_wr = SEND_MAX_WR_NUM;
	qp_init_attr.cap.max_recv_wr = RECV_MAX_WR_NUM;
	qp_init_attr.cap.max_send_sge = 1;
	qp_init_attr.cap.max_recv_sge = 1;
	res->qp = ibv_create_qp(res->pd, &qp_init_attr);

	if(!res->qp) {
		RDMAERR("failed to create QP\n");
		return -1;
	}

	RDMADBG("QP was created, QP number=0x%x\n", res->qp->qp_num);
	return 0;
}

void ibv_deinit(struct resources *res)
{
	if(res->qp) {
		ibv_destroy_qp(res->qp);
		res->qp = NULL;
	}

	if(res->cq) {
		ibv_destroy_cq(res->cq);
		res->cq = NULL;
	}

	if(res->cc) {
		ibv_destroy_comp_channel(res->cc);
		res->cc = NULL;
	}

	if(res->pd) {
		ibv_dealloc_pd(res->pd);
		res->pd = NULL;
	}

	if(res->ib_ctx) {
		ibv_close_device(res->ib_ctx);
		res->ib_ctx = NULL;
	}
}

int sock_srv_init(const char *servername, int port)
{
	struct addrinfo *resolved_addr = NULL;
	struct addrinfo *iterator;
	char service[6];
	int sockfd = -1;
	int listenfd = 0;
	int tmp;
	struct addrinfo hints = {
		.ai_flags = AI_PASSIVE,
		.ai_family = AF_INET,
		.ai_socktype = SOCK_STREAM
	};

	if(sprintf(service, "%d", port) < 0)
		goto sock_connect_exit;

	/* Resolve DNS address, use sockfd as temp storage */
	sockfd = getaddrinfo(servername, service, &hints, &resolved_addr);

	if(sockfd < 0) {
		RDMAERR("%s for %s:%d\n", gai_strerror(sockfd), servername, port);
		goto sock_connect_exit;
	}

	/* Search through results and find the one we want */
	for(iterator = resolved_addr; iterator; iterator = iterator->ai_next) {
		sockfd = socket(iterator->ai_family, iterator->ai_socktype, iterator->ai_protocol);

		if(sockfd >= 0) {
			if(bind(sockfd, iterator->ai_addr, iterator->ai_addrlen)) {
				return -1;
			}

			listen(sockfd, 32);
			return sockfd;
		}
	}

sock_connect_exit:

	if(resolved_addr)
		freeaddrinfo(resolved_addr);

	return sockfd;
}

int sock_srv_accpt(int sockfd)
{
	int accptfd = accept(sockfd, NULL, 0);

	return accptfd;
}

int sock_connect(const char *servername, int port)
{
	struct addrinfo *resolved_addr = NULL;
	struct addrinfo *iterator;
	char service[6];
	int sockfd = -1;
	int listenfd = 0;
	int tmp;
	struct addrinfo hints = {
		.ai_flags = AI_PASSIVE,
		.ai_family = AF_INET,
		.ai_socktype = SOCK_STREAM
	};

	if(sprintf(service, "%d", port) < 0)
		goto sock_connect_exit;

	/* Resolve DNS address, use sockfd as temp storage */
	sockfd = getaddrinfo(servername, service, &hints, &resolved_addr);

	if(sockfd < 0) {
		RDMAERR("%s for %s:%d\n", gai_strerror(sockfd), servername, port);
		goto sock_connect_exit;
	}

	/* Search through results and find the one we want */
	for(iterator = resolved_addr; iterator; iterator = iterator->ai_next) {
		sockfd = socket(iterator->ai_family, iterator->ai_socktype, iterator->ai_protocol);

		if(sockfd >= 0) {
			if(servername) {
				/* Client mode. Initiate connection to remote */
				if((tmp = connect(sockfd, iterator->ai_addr, iterator->ai_addrlen))) {
					RDMAERR("failed connect \n");
					close(sockfd);
					sockfd = -1;
				}

			} else {
				/* Server mode. Set up listening socket an accept a connection */
				srv_sock = listenfd = sockfd;
				sockfd = -1;

				if(bind(listenfd, iterator->ai_addr, iterator->ai_addrlen))
					goto sock_connect_exit;

				listen(listenfd, 1);
				sockfd = accept(listenfd, NULL, 0);
			}
		}
	}

sock_connect_exit:

	if(listenfd)
		close(listenfd);

	if(resolved_addr)
		freeaddrinfo(resolved_addr);

	if(sockfd < 0) {
		if(servername)
			RDMAERR("Couldn't connect to %s:%d\n", servername, port);

		else {
			perror("server accept");
			RDMAERR("accept() failed\n");
		}
	}

	return sockfd;
}

static int modify_qp_to_init(struct ibv_qp *qp, int ib_port)
{
	struct ibv_qp_attr attr;
	int flags;
	int rc;
	memset(&attr, 0, sizeof(attr));
	attr.qp_state = IBV_QPS_INIT;
	attr.port_num = ib_port;
	attr.pkey_index = 0;
	attr.qp_access_flags = IBV_ACCESS_LOCAL_WRITE | IBV_ACCESS_REMOTE_READ | IBV_ACCESS_REMOTE_WRITE | IBV_ACCESS_REMOTE_ATOMIC;
	flags = IBV_QP_STATE | IBV_QP_PKEY_INDEX | IBV_QP_PORT | IBV_QP_ACCESS_FLAGS;
	rc = ibv_modify_qp(qp, &attr, flags);

	if(rc)
		RDMAERR("failed to modify QP state to INIT\n");

	return rc;
}

static int modify_qp_to_rtr(struct ibv_qp *qp, uint32_t remote_qpn, uint16_t dlid, uint8_t *dgid, int ib_port, int gid_idx)
{
	struct ibv_qp_attr attr;
	int flags;
	int rc;
	memset(&attr, 0, sizeof(attr));
	attr.qp_state = IBV_QPS_RTR;
	attr.path_mtu = IBV_MTU_1024;
	attr.dest_qp_num = remote_qpn;
	attr.rq_psn = 0;
	attr.max_dest_rd_atomic =  8;
	attr.min_rnr_timer = 10;
	attr.ah_attr.is_global = 0;
	attr.ah_attr.dlid = dlid;
	attr.ah_attr.sl = 0;
	attr.ah_attr.src_path_bits = 0;
	attr.ah_attr.port_num = ib_port;

	attr.ah_attr.is_global = 1;
	attr.ah_attr.port_num = 1;
	memcpy(&attr.ah_attr.grh.dgid, dgid, 16);
	attr.ah_attr.grh.flow_label = 0;
	attr.ah_attr.grh.hop_limit = 1;
	attr.ah_attr.grh.sgid_index = gid_idx;
	attr.ah_attr.grh.traffic_class = 0;

	flags = IBV_QP_STATE | IBV_QP_AV | IBV_QP_PATH_MTU | IBV_QP_DEST_QPN |
			IBV_QP_RQ_PSN | IBV_QP_MAX_DEST_RD_ATOMIC | IBV_QP_MIN_RNR_TIMER;
	rc = ibv_modify_qp(qp, &attr, flags);

	if(rc)
		RDMAERR("failed to modify QP state to RTR\n");

	return rc;
}

static int modify_qp_to_rts(struct ibv_qp *qp)
{
	struct ibv_qp_attr attr;
	int flags;
	int rc;
	memset(&attr, 0, sizeof(attr));
	attr.qp_state = IBV_QPS_RTS;
	attr.timeout = 12;
	attr.retry_cnt = 7;
	attr.rnr_retry = 7;
	attr.sq_psn = 0;
	attr.max_rd_atomic = 1;
	flags = IBV_QP_STATE | IBV_QP_TIMEOUT | IBV_QP_RETRY_CNT |
			IBV_QP_RNR_RETRY | IBV_QP_SQ_PSN | IBV_QP_MAX_QP_RD_ATOMIC;
	rc = ibv_modify_qp(qp, &attr, flags);

	if(rc)
		RDMAERR("failed to modify QP state to RTS\n");

	return rc;
}

static int connect_qp(struct resources *res, int ib_port, int gid_idx)
{
	struct cm_con_data_t local_con_data;
	struct cm_con_data_t remote_con_data;
	struct cm_con_data_t tmp_con_data;
	int rc = -1;
	char temp_char;
	union ibv_gid my_gid;

	rc = ibv_query_gid(res->ib_ctx, ib_port, gid_idx, &my_gid);

	if(rc) {
		RDMAERR("could not get gid for port %d, index %d\n", ib_port, gid_idx);
		return rc;
	}

	/* exchange using TCP sockets info required to connect QPs */
	local_con_data.addr = htonll((uint64_t)res->local_write);
	local_con_data.rkey = htonl(res->mr_lwt->rkey);
	local_con_data.qp_num = htonl(res->qp->qp_num);
	local_con_data.lid = htons(res->port_attr.lid);
	memcpy(local_con_data.gid, &my_gid, 16);
	RDMADBG("\nLocal LID = 0x%x\n", res->port_attr.lid);

	if(sock_sync_data(res->sock, sizeof(struct cm_con_data_t), (char *)&local_con_data, (char *)&tmp_con_data) < 0) {
		RDMAERR("failed to exchange connection data between sides\n");
		rc = -1;
		goto connect_qp_exit;
	}

	remote_con_data.addr = ntohll(tmp_con_data.addr);
	remote_con_data.rkey = ntohl(tmp_con_data.rkey);
	remote_con_data.qp_num = ntohl(tmp_con_data.qp_num);
	remote_con_data.lid = ntohs(tmp_con_data.lid);
	memcpy(remote_con_data.gid, tmp_con_data.gid, 16);
	/* save the remote side attributes, we will need it for the post SR */
	res->remote_props = remote_con_data;
	res->remote_write = remote_con_data.addr;
	res->remote_wtkey = remote_con_data.rkey;

	buffer_mgr_init(&res->bm_rwt, (void*)res->remote_write + WRITE_BUF_RESERVE, WRITE_BUF_LEN - WRITE_BUF_RESERVE);

	RDMADBG("Remote address = 0x%" PRIx64 "\n", remote_con_data.addr);
	RDMADBG("Remote rkey = 0x%x\n", remote_con_data.rkey);
	RDMADBG("Remote QP number = 0x%x\n", remote_con_data.qp_num);
	RDMADBG("Remote LID = 0x%x\n", remote_con_data.lid);

	uint8_t *p = remote_con_data.gid;
	RDMADBG("Remote GID =%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x\n ", p[0],
			p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10], p[11], p[12], p[13], p[14], p[15]);
	/* modify the QP to init */
	rc = modify_qp_to_init(res->qp, ib_port);

	if(rc) {
		RDMAERR("change QP state to INIT failed\n");
		goto connect_qp_exit;
	}

	/* modify the QP to RTR */
	rc = modify_qp_to_rtr(res->qp, remote_con_data.qp_num, remote_con_data.lid, remote_con_data.gid, ib_port, gid_idx);

	if(rc) {
		RDMAERR("failed to modify QP state to RTR\n");
		goto connect_qp_exit;
	}

	rc = modify_qp_to_rts(res->qp);

	if(rc) {
		RDMAERR("failed to modify QP state to RTR\n");
		goto connect_qp_exit;
	}

	RDMADBG("QP state was change to RTS\n");

	/* sync to make sure that both sides are in states that they can connect to prevent packet loose */
	if(sock_sync_data(res->sock, 1, "Q", &temp_char)) { /* just send a dummy char back and forth */
		RDMAERR("sync error after QPs are were moved to RTS\n");
		rc = 1;
	}

connect_qp_exit:
	return rc;
}




struct resources*  rdma_srv_init(int sockfd, char *dev_name, int ib_port, int gid_idx)
{
	struct resources *res = NULL;
	int rc = 0;

	RDMAERR("rdma info: sock:%d, devname:%s, ibport:%d, gid_idx:%d\n", sockfd, dev_name, ib_port, gid_idx);
	res = malloc(sizeof(struct resources));

	if(!res) {
		RDMAERR("failed to malloc memory.\n");
		return NULL;
	}

	memset(res, 0, sizeof * res);
	res->sock = sockfd;

	if(0 != ibv_init(res, dev_name, ib_port)) {
		free(res);
		RDMAERR("failed to ibv_init.\n");
		return NULL;
	}

	if(0 != rdma_buf_init(res)) {
		free(res);
		RDMAERR("failed to rdma_buf_init.\n");
		return NULL;
	}

	if(0 != connect_qp(res, ib_port, gid_idx)) {
		free(res);
		RDMAERR("failed to connect_qp.\n");
		return NULL;
	}

	//res->recv_cb = recv_cb;
	//rdma_start_event_thread(res);
	return res;

}

struct resources*  rdma_cli_init(char *dev_name, char *srv_name, int tcp_port, int ib_port, int gid_idx)
{
	struct resources *res = NULL;
	int rc = 0;

	RDMAERR("rdma info: sock:%s:%d, devname:%s, ibport:%d, gid_idx:%d\n", srv_name, tcp_port, dev_name, ib_port, gid_idx);

	res = malloc(sizeof(struct resources));

	if(!res) {
		RDMAERR("failed to malloc memory.\n");
		return NULL;
	}

	memset(res, 0, sizeof * res);
#if 0
	res->sock = sockfd;
#else
	res->sock = sock_connect(srv_name, tcp_port);

	if(res->sock < 0) {
		RDMAERR("failed to establish TCP connection with client on port %d\n",
				tcp_port);
		rc = -1;
		free(res);
		return NULL;
	}

	RDMADBG("TCP connection was established\n");
#endif

	if(0 != ibv_init(res, dev_name, ib_port)) {
		free(res);
		RDMAERR("failed to ibv_init.\n");
		return NULL;
	}

	if(0 != rdma_buf_init(res)) {
		free(res);
		RDMAERR("failed to rdma_buf_init.\n");
		return NULL;
	}

	if(0 != connect_qp(res, ib_port, gid_idx)) {
		free(res);
		RDMAERR("failed to connect_qp.\n");
		return NULL;
	}

	//res->recv_cb = recv_cb;
	//rdma_start_event_thread(res);
	return res;
}

void rdma_regist_cb(struct resources *res, struct rdma_cb *handle)
{
	res->cb_handle = handle;
}

void rdma_deinit(struct resources *res)
{
	RDMAERR("rdma end and release resources.\n");
	rdma_stop_event_thread(res);
	post_err_wr(res);
	rdma_destroy_event_thread(res);
	rdma_buf_deinit(res);
	ibv_deinit(res);
	free(res);
}

static void usage(const char *argv0)
{
	fprintf(stdout, "Usage:\n");
	fprintf(stdout, " %s start a server and wait for connection\n", argv0);
	fprintf(stdout, " %s <host> connect to server at <host>\n", argv0);
	fprintf(stdout, "\n");
	fprintf(stdout, "Options:\n");
	fprintf(stdout, " -p, --port <port> listen on/connect to port <port> (default 18515)\n");
	fprintf(stdout, " -d, --ib-dev <dev> use IB device <dev> (default first device found)\n");
	fprintf(stdout, " -i, --ib-port <port> use port <port> of IB device (default 1)\n");
	fprintf(stdout, " -g, --gid_idx <git index> gid index to be used in GRH (default not used)\n");
}

#if 1

int tst_rdma_recv_cb(void *priv, char *data, int len)
{
	static int old_cnt = 0;
	int *plen;
	int *cnt;
	char *p = (char*)priv;
	cnt = (int*)&data[11];
	plen = (int*)&data[len - 4];

	//data[10]=0;
	RDMAERR("-------------priv:%s\n", p);
	RDMAERR("--------------cnt:%d, old cnt:%d\n", *cnt, old_cnt);
	RDMAERR("--------------len:%d\n", len);
	RDMAERR("--------data addr:%p\n", data);
	RDMAERR("------in data len:%d \n", *plen);
	RDMAERR("---recv_proc data:%s\n", data);

	if(len != *plen || *cnt != (old_cnt + 1)) {
		exit(0);
	}

	old_cnt = *cnt;
	return 0;
}

int tst_rdma_err(void *priv, int errno)
{
	RDMAERR("rdma err, exit...\n");
	rdma_deinit(gRdma_handle);
	exit(0);
}

int main(int argc, char *argv[])
{
	char temp_char;
	char *dev_name; /* IB device name */
	char *server_name = NULL; /* server host name */
	u_int32_t tcp_port = 8084;   /* server TCP port */
	int ib_port = 1;          /* local IB port to work with */
	int gid_idx = -1;         /* gid index to use */
	int test_cnt = 0;
	char *sendbuf;
	struct resources *res;


	/* parse the command line parameters */
	while(1) {
		int c;
		static struct option long_options[] = {
			{.name = "port",    .has_arg = 1, .val = 'p'},
			{.name = "ib-dev",  .has_arg = 1, .val = 'd'},
			{.name = "ib-port", .has_arg = 1, .val = 'i'},
			{.name = "gid-idx", .has_arg = 1, .val = 'g'},
			{.name = NULL,      .has_arg = 0, .val = '\0'}
		};
		c = getopt_long(argc, argv, "p:d:i:g:", long_options, NULL);

		if(c == -1)
			break;

		switch(c) {
		case 'p':
			tcp_port = strtoul(optarg, NULL, 0);
			break;

		case 'd':
			dev_name = strdup(optarg);
			break;

		case 'i':
			ib_port = strtoul(optarg, NULL, 0);

			if(ib_port < 0) {
				usage(argv[0]);
				return 1;
			}

			break;

		case 'g':
			gid_idx = strtoul(optarg, NULL, 0);

			if(gid_idx < 0) {
				usage(argv[0]);
				return 1;
			}

			break;

		default:
			usage(argv[0]);
			return 1;
		}
	}

	/* parse the last parameter (if exists) as the server name */
	if(optind == argc - 1)
		server_name = argv[optind];

	if(server_name) {
		printf("servername=%s\n", server_name);

	} else if(optind < argc) {
		usage(argv[0]);
		return 1;
	}

	if(server_name) {
		res = rdma_cli_init(dev_name, server_name, tcp_port, ib_port, gid_idx);

	} else {
		int sockfd = -1;
		sockfd = sock_srv_init(NULL, tcp_port);

		printf("sock_srv_init return:%d, use port:%d\n", sockfd, tcp_port);

		if(sockfd <= 0) {
			exit(0);
		}

		while(1) {
			int accptsock = sock_srv_accpt(sockfd);

			switch(fork()) {
			case -1:
				printf("Can't create child (%s)\n", strerror(errno));
				break;

			case 0:
				res = rdma_srv_init(accptsock, dev_name, ib_port, gid_idx);
				goto start_test;
				break;

			default:
				break;
			}
		}
	}

start_test:

	if(!res) {
		RDMADBG("rdma init err!\n");
		return 0;
	}

	struct rdma_cb recv_cb_tst;

	recv_cb_tst.cb_priv = "TEST";

	recv_cb_tst.recv_cb = tst_rdma_recv_cb;

	recv_cb_tst.error_cb = tst_rdma_err;

	rdma_regist_cb(res, &recv_cb_tst);

	rdma_start_event_thread(res);

	while(test_cnt++ < 10000) {
		int len = rand() % 100;
		int *cnt;
		len += 30;

		RDMADBG("send test loop start:%d len:%d\n", test_cnt, len);

		sendbuf = rdma_alloc_send_buf(res, len);

		if(server_name) {
			snprintf(sendbuf, len, "cli:%d", test_cnt);

		} else {
			snprintf(sendbuf, len, "srv:%d", test_cnt);
		}

		sendbuf[10] = 0;
		cnt = (int*)&sendbuf[11];
		*cnt = test_cnt;
		cnt = (int*)&sendbuf[len - 4];
		*cnt = len;

		rdma_send_data(res, sendbuf, len);
		RDMADBG("send test loop end:%d\n", test_cnt);
	}

	sleep(60);
	rdma_stop_event_thread(res);
}

#endif
