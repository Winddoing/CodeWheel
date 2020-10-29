#ifndef GP_COMMON_H
#define GP_COMMON_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TCP_RCV_BUF_SIZE     83886080   /* 80MB */
#define TCP_SND_BUF_SIZE     83886080   /* 80MB */

#define GPCMD_QUEUE_SIZE     5000

#define MAGIC_NUMBER_FOR_BYTE_ORDER  0x1234   /* use uint16_t to store */
#define MAGIC_NUMBER_LEN             2

typedef enum { false, true } bool;

/* Used for CMD_COMMON */
enum gpCommonOpcode {
    OPCODE_BYTEORDER,
    OPCODE_SEND_CONN_PROFILE,
};

/* Used for CMD_GLX */
enum gpGlxOpcode {
    OPCODE_GLXMAKECURRENT,
    OPCODE_GLXCLEARCURRENT,
    OPCODE_GLXDESTROYCURRENT,
    OPCODE_GLXSWAPBUFFERS,
    OPCODE_GLXCREATECONTEXT,
    OPCODE_GLXCREATECONTEXTATTRIBSARB,
    OPCODE_GLXQUERYCONTEXT,
    OPCODE_GLXQUERYDRAWABLE,
    OPCODE_GLXCHOOSEVISUAL,
    OPCODE_GLXCHOOSEFBCONFIG,
    OPCODE_GLXQUERYEXTENSIONSSTRING,

    OPCODE_GLXEND,
};

/* we use int:4 to store type, so max <= 16  */
typedef enum {
    CMD_OPENGL_SINGLE,
    CMD_OPENGL_SINGLE_REPLY,
    CMD_OPENGL_BATCH,
    CMD_OPENGL_BATCH_REPLY,
    CMD_OPENGL_BATCH_HEADER,
    CMD_GLX,
    CMD_GLX_REPLY,
    CMD_COMMON,          /* should process byte-order */
    CMD_COMMON_REPLY,    /* should process byte-order */
    CMD_VGR_SINGLE,
    CMD_VGR_SINGLE_REPLY,
    CMD_VGR_CB,
    CMD_VGR_CB_REPLAY,

} gpCmdType_t;

/*
 * 一个(gpCmd_t)就是一个完整的最小传输单元，以字节流的形式发送。
 */
#define MAX_GPCMD_DATALEN   0xFFFFFFF   /* 28bit */
typedef struct {
    uint32_t type:4;        /* support 16 different types at most */
    uint32_t datalen:28;    /* strlen(gpcmd->data)==256MBytes at most */

} gpCmdDes_t;

typedef struct {
    union {
        uint32_t value;     /* always htonl()/ntohl() when send/recv, no matter client or server */
        gpCmdDes_t des;
    } header;
    uint8_t  data[0];       /* opcode(2 bytes) + [parameters | reply] */
} gpCmd_t;

#define SIZEOF_GPCMDHEADER     4    /* sizeof(gpCmdDes_t) */
#define SIZEOF_OPCODE          2    /* opcode is defined as uint16_t, support 65536 cmds at most */
#define SIZEOF_GPCMD(gpcmd)   ((gpcmd)->header.des.datalen + SIZEOF_GPCMDHEADER)

#define GET_GPCMD_DATALEN(gpcmd)      ((gpcmd)->header.des.datalen)
#define SET_GPCMD_DATALEN(gpcmd, len) (GET_GPCMD_DATALEN(gpcmd)=(len))

#define GET_GPCMD_PDATA(gpcmd)        ((gpcmd)->data)

#define GET_GPCMD_TYPE(gpcmd)         ((gpcmd)->header.des.type)
#define SET_GPCMD_TYPE(gpcmd, type)   (GET_GPCMD_TYPE(gpcmd)=(type))

#define NTOH_GPCMD_HEADER(gpcmd)      ((gpcmd)->header.value=ntohl((gpcmd)->header.value))
#define HTON_GPCMD_HEADER(gpcmd)      ((gpcmd)->header.value=htonl((gpcmd)->header.value))

#define VGRGL_OPCODE_BASE     0
#define VGRGL_OP(op)        ((op)+VGRGL_OPCODE_BASE)
/*
 *
  Byte-order convert
 */
#define	bswap_16(value) ((((value) & 0xff) << 8) | ((value) >> 8))

#define	bswap_32(value) \
 	(((uint32_t)bswap_16((uint16_t)((value) & 0xffff)) << 16) | (uint32_t)bswap_16((uint16_t)((value) >> 16)))

#define	bswap_64(value)	\
 	(((uint64_t)bswap_32((uint32_t)((value) & 0xffffffff)) << 32) | (uint64_t)bswap_32((uint32_t)((value) >> 32)))


#define CHECK_ENV(x) \
    if(!getenv(x) || !strlen(getenv(x))) { \
        printf("$s: env %s is not set!\n", __func__, x); \
        exit(-1);  \
    }

static inline gpCmd_t * alloc_recv_gpcmd(int datalen)
{
    /* 允许创建一个data为空的gpcmd */
    if(datalen < 0) return NULL;
    gpCmd_t *gpcmd = (gpCmd_t *)alloc_memory(SIZEOF_GPCMDHEADER + datalen, NULL);
    if(!gpcmd) {
        return NULL;
    }

    SET_GPCMD_DATALEN(gpcmd, datalen);
    return gpcmd;
}

static inline void free_recv_gpcmd(gpCmd_t *gpcmd)
{
    if(gpcmd) free_memory(gpcmd, NULL);
}


static inline gpCmd_t * alloc_gpcmd(int datalen)
{
    /* 允许创建一个data为空的gpcmd */
    if(datalen < 0) return NULL;
    gpCmd_t *gpcmd = (gpCmd_t *)alloc_memory(SIZEOF_GPCMDHEADER + datalen, NULL);
    if(!gpcmd) {
        return NULL;
    }
    SET_GPCMD_DATALEN(gpcmd, datalen);
    return gpcmd;
}

static inline void free_gpcmd(gpCmd_t *gpcmd)
{
    if(gpcmd) free_memory(gpcmd, NULL);
}

static inline uint16_t get_opcode(gpCmd_t *gpcmd, int need_swap)
{
    uint16_t opcode;
    memcpy(&opcode, GET_GPCMD_PDATA(gpcmd), SIZEOF_OPCODE);
    if(need_swap)
        return bswap_16(opcode);
    else
        return opcode;
}

static inline void set_sockopt(int sockfd)
{
    int rcvbuf_size, sndbuf_size;
    int rcvopt_len = 4;
    int sndopt_len = 4;
    if(0 != getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &sndbuf_size, &sndopt_len)) {
        printf("getsockopt SO_SNDBUF fail: %s\n", strerror(errno));
    } else {
        //printf("old SO_SNDBUF: %d\n", sndbuf_size);
    }
    sndbuf_size = TCP_SND_BUF_SIZE;
    if(0 != setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &sndbuf_size, 4)) {
        printf("setsockopt SO_SNDBUF fail: %s\n", strerror(errno));
    } else {
        //printf("new SO_SNDBUF: %d\n", sndbuf_size);
    }

    if(0 != getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcvbuf_size, &rcvopt_len)) {
        printf("getsockopt SO_RCVBUF fail: %s\n", strerror(errno));
    } else {
        //printf("old SO_RCVBUF: %d\n", rcvbuf_size);
    }
    rcvbuf_size = TCP_RCV_BUF_SIZE;
    if(0 != setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcvbuf_size, 4)) {
        printf("setsockopt SO_RCVBUF fail: %s\n", strerror(errno));
    } else {
        //printf("new SO_RCVBUF: %d\n", rcvbuf_size);
    }

    /* Disable Nagle */
    int disable_nagle;
    int nagleopt_len = 4;
    if(0 != getsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &disable_nagle, &nagleopt_len)) {
        printf("getsockopt TCP_NODELAY fail: %s\n", strerror(errno));
    } else {
        //printf("old TCP_NODELAY: %d\n", disable_nagle);
    }
    disable_nagle = 1;
    if(0 != setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &disable_nagle, 4)) {
        printf("setsockopt TCP_NODELAY fail: %s\n", strerror(errno));
    } else {
        //printf("new TCP_NODELAY: %d\n", disable_nagle);
    }
}

static inline int enable_keepalive(int fd)
{
    int keepalive = 1;   // enable keepalive
    int keepidle  = 1;   // Idle time before start keepalive
    int keepintvl = 10;  // time interval between two keepalive
    int keepcnt   = 1;   // keepalive counts

    if(setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepalive, sizeof(keepalive)) < 0) {
        printf("setsockopt SO_KEEPALIVE fail: %s\n", strerror(errno));
        return -1;
    }

    if(setsockopt(fd, SOL_TCP, TCP_KEEPIDLE, (void *)&keepidle, sizeof(keepidle)) < 0) {
        printf("setsockopt TCP_KEEPIDLE fail: %s\n", strerror(errno));
        return -1;
    }

    if(setsockopt(fd, SOL_TCP, TCP_KEEPINTVL, (void *)&keepintvl, sizeof(keepintvl)) < 0) {
        printf("setsockopt TCP_KEEPINTVL fail: %s\n", strerror(errno));
        return -1;
    }

    if(setsockopt(fd, SOL_TCP, TCP_KEEPCNT, (void *)&keepcnt, sizeof(keepcnt)) < 0) {
        printf("setsockopt TCP_KEEPCNT fail: %s\n", strerror(errno));
        return -1;
    }
}


#ifdef __cplusplus
}
#endif

#endif
