
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>


#include "veloop.h"
#include "VxLog.h"
#include "libvxipc.h"
#include "vxplayer.h"
#include "rtp_queue.h"


struct rtpBufNode rtp_buf_pool[RTP_BUFF_POOL_SIZE];

unsigned short U16_AT(const unsigned char *ptr)
{
    return ptr[0] << 8 | ptr[1];
}

unsigned int U32_AT(const unsigned char *ptr)
{
    return ptr[0] << 24 | ptr[1] << 16 | ptr[2] << 8 | ptr[3];
}

unsigned long long U64_AT(const unsigned char *ptr)
{
    return ((unsigned long long)U32_AT(ptr)) << 32 | U32_AT(ptr + 4);
}



VXPLAYER *vxplayer = NULL;

struct rtpStreamQueue rtp_queue ;
void rtcp_init(void);

long long GetNowUs(void)
{

    struct timespec time_now;
    clock_gettime(CLOCK_MONOTONIC,&time_now);

    return time_now.tv_sec*1000000LL+time_now.tv_nsec/1000;
}
void init_rtp_buf_pool(void)
{
    int i = 0;

    for(i=0;i<RTP_BUFF_POOL_SIZE;i++)
    {
        memset(rtp_buf_pool+i,0,sizeof(struct rtpBufNode));

    }
    return ;
}


struct rtpBufNode* net_get_buf(void)
{
    int i = 0;


    for(i=0;i<RTP_BUFF_POOL_SIZE;i++)
    {
        if(rtp_buf_pool[i].used == 0)
        {
            return rtp_buf_pool + i;

        }
    }
    printf("no rtp buf available. clear rtp buf\n");
    rtcp_init();
    return net_get_buf();

}

void free_rtp_buf(struct rtpBufNode* buffer)
{

    rtp_queue.mTotalBytesQueued -- ;
    dl_list_del(&buffer->list);
    buffer->used = 0;
    return ;
}

void initSeq(unsigned short seq)
{
    rtp_queue.mMaxSeq = seq;
    rtp_queue.mCycles = 0;
    rtp_queue.mBadSeq  = (1u << 16) + 1;
    rtp_queue.mLastDequeuedExtSeqNo = -1;
}

void queuePacket(struct rtpBufNode *node)
{
    struct rtpBufNode *tmp;

    int newExtendedSeqNo =  node->seqNo ;

    if(dl_list_empty(&rtp_queue.rtpList)) {
        dl_list_add(&rtp_queue.rtpList,&(node->list));
        goto end ;
    }

    /*this is a retransmittion*/
    dl_list_for_each_reverse(tmp,&rtp_queue.rtpList,struct rtpBufNode,list) {
        if(tmp->seqNo == newExtendedSeqNo) {
            /*duplicate packet, ignore*/
            printf("queue duplicate packet %d ignore\n",newExtendedSeqNo);
            return ;
        }

        if(newExtendedSeqNo > tmp->seqNo) {
            dl_list_add(&tmp->list,&node->list);
            goto end ;
        }
    }
    dl_list_add(&rtp_queue.rtpList,&node->list);
end:
    node->used = 1;
    rtp_queue.mTotalBytesQueued ++;

    return ;
}

int updateSeq(unsigned short seq, struct rtpBufNode *node)
{
    unsigned short udelta = seq - rtp_queue.mMaxSeq;

    if (rtp_queue.mProbation) {
        // Startup phase
        if (seq == rtp_queue.mMaxSeq + 1) {
            node->seqNo = rtp_queue.mCycles | seq;
            putTsBuffer(vxplayer,node);
            --rtp_queue.mProbation;
            rtp_queue.mMaxSeq = seq;
            if (rtp_queue.mProbation == 0) {
                initSeq(seq);
                return 0;
            }
        } else {
            // Packet out of sequence, restart startup phase
            rtp_queue.mProbation = RTP_SEQ_PROBATION_NUM - 1;
            rtp_queue.mMaxSeq = seq;
            node->seqNo = rtp_queue.mCycles | seq;
            putTsBuffer(vxplayer,node);
        }

        return -1;
    }

    if (udelta < RTP_PACKET_MAX_DROPCOUNT) {
        // In order, with permissible gap.
        if (seq <rtp_queue.mMaxSeq) {
            // Sequence number wrapped - count another 64K cycle
            rtp_queue.mCycles += RTP_SEQ_NUMBER_MOD;
        }
        rtp_queue.mMaxSeq = seq;
    } else if (udelta <= RTP_SEQ_NUMBER_MOD - RTP_DISORDER_COUNT) {
        // The sequence number made a very large jump
        if (seq == rtp_queue.mBadSeq) {
            // Two sequential packets -- assume that the other side
            // restarted without telling us so just re-sync
            // (i.e. pretend this was the first packet)
            initSeq(seq);
        } else {
            rtp_queue.mBadSeq = (seq + 1) & (RTP_SEQ_NUMBER_MOD - 1);
            return -1;
        }
    } else {
        // Duplicate or reordered packet.
    }

    node->seqNo = rtp_queue.mCycles | seq;
    putTsBuffer(vxplayer,node);
    return 0;
}

/* main */
void  parseRtp(VXPLAYER *player ,struct rtpBufNode *node,unsigned int len)
{
    unsigned int paddingLength = 0;     //填充长度
    unsigned int  payloadOffset = 12;   //有效负载
    unsigned char *extensionData = NULL;    //扩展数据
    unsigned int extensionLength = 0;       //扩展数据长度
    unsigned short seqNo;
    unsigned char *packet = node->buf ;
    vxplayer = player;

    if(len < 12) {
        printf("rtp packet too short ,lenght %d\n",len);
        return ;
    }

    if ((packet[0] >> 6) != 2) {
        printf("wrong rtp version %d\n",packet[0] >> 6);
        return ;
    }

    if (packet[0] & 0x20) {
        // Padding present.
        paddingLength = packet[len - 1];
        if (paddingLength + 12 > len) {
            // If we removed this much padding we'd end up with something
            // that's too short to be a valid RTP header.
            printf("rtp too short 1\n");
            return ;
        }

        len -= paddingLength;
    }

    if (packet[0] & 0x10) {
        // Header eXtension present.
        if (len < payloadOffset + 4) {
            // Not enough data to fit the basic header, all CSRC entries
            // and the first 4 bytes of the extension header.
            printf("rtp too short 2\n");
            return ;
        }
        extensionData = &packet[payloadOffset];
        extensionLength =  4 * (extensionData[2] << 8 | extensionData[3]);
        if (len < payloadOffset + 4 + extensionLength) {
            printf("rtp too short 3\n");
            return ;
        }
        payloadOffset += 4 + extensionLength;
    }

    seqNo = U16_AT(&packet[2]);

    node->ssrc = U32_AT(&packet[8]);
    rtp_queue.rtpSsrc =  node->ssrc ;
    node->payloadOffet = payloadOffset;
    node->payloadLen = len-payloadOffset;

    if(!rtp_queue.seqInited) {
        initSeq(seqNo);
        rtp_queue.mMaxSeq = seqNo -1;
        rtp_queue.seqInited = 1;
        node->seqNo = rtp_queue.mCycles|seqNo;
        putTsBuffer(player,node);
    } else {
        updateSeq(seqNo,node);
    }

    return ;
}

void requestLostPacket(struct vx_udp_netsession_s_type *rtcp,int seqNo)
{
    char rtcpbuf[128]={0};
    unsigned int srcId = rtp_queue.rtpSsrc;
    int n;
    int blp = 0;

    rtcpbuf[0] = 0x80 | 1;  // generic NACK
    rtcpbuf[1] = 205;  // RTPFB
    rtcpbuf[2] = 0;
    rtcpbuf[3] = 3;
    rtcpbuf[4] = 0xde;  // sender SSRC
    rtcpbuf[5] = 0xad;
    rtcpbuf[6] = 0xbe;
    rtcpbuf[7] = 0xef;
    rtcpbuf[8] = (srcId >> 24) & 0xff;
    rtcpbuf[9] = (srcId >> 16) & 0xff;
    rtcpbuf[10] = (srcId >> 8) & 0xff;
    rtcpbuf[11] = (srcId & 0xff);
    rtcpbuf[12] = (seqNo >> 8) & 0xff;
    rtcpbuf[13] = (seqNo & 0xff);
    rtcpbuf[14] = (blp >> 8) & 0xff;
    rtcpbuf[15] = (blp & 0xff);

    do {
        n = send(rtcp->fd,rtcpbuf,16,0);
    } while(n == -1 && errno == EINTR);
}

struct rtpBufNode* dequeueBuffer(VXPLAYER *player)
{
    struct rtpBufNode *tmp ,*node;
    int extSeqNo;

    dl_list_for_each_safe(node,tmp,&rtp_queue.rtpList,struct rtpBufNode,list) {
        extSeqNo = node->seqNo;
        if(extSeqNo > rtp_queue.mLastDequeuedExtSeqNo || rtp_queue.mLastDequeuedExtSeqNo <0)
            break;
        printf("drop duplicate packet seqno %d\n",extSeqNo);
        free_rtp_buf(node);
        node = NULL;
        extSeqNo = -1;
    }

    if(dl_list_empty(&rtp_queue.rtpList)) {
        //printf("queue empty \n");
        return NULL;
    }

    if (rtp_queue.mLastDequeuedExtSeqNo < 0 || extSeqNo == rtp_queue.mLastDequeuedExtSeqNo + 1) {
        if (rtp_queue.mRequestedRetransmission) {
            printf("Recovered after requesting retransmission of %d\n",
                    extSeqNo & 0xffff);
        }

        rtp_queue.mLastDequeuedExtSeqNo = extSeqNo;
        rtp_queue.mFirstFailedAttemptUs = -1ll;
        rtp_queue.mRequestedRetransmission = 0;

        return node;
    }

#if 0
    if (rtp_queue.mFirstFailedAttemptUs < 0ll) {
        rtp_queue.mFirstFailedAttemptUs = GetNowUs();

        printf("failed to get the correct packet the first time.\n");
        return NULL;
    }


    if (rtp_queue.mFirstFailedAttemptUs + 50000ll > GetNowUs()) {
        // We're willing to wait a little while to get the right packet.

        if (!rtp_queue.mRequestedRetransmission) {
            printf("requesting retransmission of seqNo %d\n",
                    (rtp_queue.mLastDequeuedExtSeqNo + 1) & 0xffff);

            /*post rtcp message to request the lost package*/
            requestLostPacket(&player->rtcpUdp,(rtp_queue.mLastDequeuedExtSeqNo + 1)&0xffff);


            rtp_queue.mRequestedRetransmission = 1;

        } else {
            printf("still waiting for the correct packet to arrive.\n");
        }

        return NULL;
    }
#endif
    printf("lost packet %d forever\n",(rtp_queue.mLastDequeuedExtSeqNo + 1)&0xffff);

    rtp_queue.mLastDequeuedExtSeqNo = extSeqNo;
    rtp_queue.mFirstFailedAttemptUs = -1ll;
    rtp_queue.mRequestedRetransmission = 0;


    return node;

}


void putTsBuffer(VXPLAYER *player,struct rtpBufNode *node)
{
    struct rtpBufNode *dequeueNode = NULL;

    queuePacket(node);

    if(!player->find_pmt) {
        put_ts_stream(player,node->buf+node->payloadOffet,node->payloadLen);
        return;
    }

    do {
        dequeueNode = dequeueBuffer(player);

        if(dequeueNode) {
            put_ts_stream(player,dequeueNode->buf+dequeueNode->payloadOffet,dequeueNode->payloadLen);
            free_rtp_buf(dequeueNode);
        }
    } while(dequeueNode);

    return ;
}

void rtcp_deinit(void)
{
    dl_list_init(&rtp_queue.rtpList);
    init_rtp_buf_pool();
    rtp_queue.inited = 0;
    rtp_queue.seqInited = 0;
}

void rtcp_init(void)
{
    dl_list_init(&rtp_queue.rtpList);
    init_rtp_buf_pool();

    rtp_queue.mProbation = RTP_SEQ_PROBATION_NUM;
    rtp_queue.mFirstFailedAttemptUs = -1ll;
    rtp_queue.mRequestedRetransmission = 0;
    rtp_queue.mTotalBytesQueued = 0;
    rtp_queue.mCycles = 0;
    rtp_queue.inited = 1;
}

void extern_start_rtpqueue(void)
{
    rtcp_init();
}

void extern_stop_rtpqueue(void)
{
    rtcp_deinit();
}
