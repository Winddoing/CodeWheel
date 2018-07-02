#ifndef _VX_RTP_QUEUE_H_
#define _VX_RTP_QUEUE_H_

#include "vxplayer.h"

#include "veloop.h"

#define RTP_SEQ_PROBATION_NUM 2
#define RTP_PACKET_MAX_DROPCOUNT 3000
#define RTP_SEQ_NUMBER_MOD (1u << 16)
#define RTP_BUFF_POOL_SIZE 5000
#define RTP_NODE_BUFF_SIZE 1400
#define RTP_DISORDER_COUNT 100


struct rtpBufNode
{
	struct dl_list list;
	unsigned char used;

	unsigned int payloadLen;
	unsigned int payloadOffet;
	int seqNo;
	unsigned int ssrc;

	unsigned char buf[RTP_NODE_BUFF_SIZE];

};

struct rtpStreamQueue
{
	struct dl_list rtpList;

	unsigned char inited;
	unsigned char seqInited;
	int mTotalBytesQueued;

	unsigned int rtpSsrc;

	int mLastDequeuedExtSeqNo;
	long long mFirstFailedAttemptUs;
	unsigned short mMaxSeq;
	unsigned int mBadSeq;
    unsigned int mCycles;
	unsigned int mProbation;
	unsigned char mRequestedRetry;
    unsigned char mRequestedRetransmission;

};

void  parseRtp(VXPLAYER *player ,struct rtpBufNode *node,unsigned int len);
void extern_start_rtpqueue(void);
void extern_stop_rtpqueue(void);
struct rtpBufNode* net_get_buf(void);
void putTsBuffer(VXPLAYER *player,struct rtpBufNode *node);

#endif
