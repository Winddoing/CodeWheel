/*************************************************************************
	> File Name: polaris_dma.h
	> Author: tao.wang@byavs.com
	> 
 ************************************************************************/

#ifndef __GV_DMA_API_H__
#define __GV_DMA_API_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


#ifndef TYPE_GV_U8
#define TYPE_GV_U8
typedef unsigned char           GV_U8;
#endif

#ifndef TYPE_GV_U16
#define TYPE_GV_U16
typedef unsigned short          GV_U16;
#endif

#ifndef TYPE_GV_U32
#define TYPE_GV_U32
typedef unsigned int            GV_U32;
#endif

#ifndef TYPE_GV_UL32
#define TYPE_GV_UL32
typedef unsigned long           GV_UL32;
#endif

#ifndef TYPE_GV_S8
#define TYPE_GV_S8
typedef signed char             GV_S8;
#endif

#ifndef TYPE_GV_S16
#define TYPE_GV_S16
typedef short                   GV_S16;
#endif

#ifndef TYPE_GV_S32
#define TYPE_GV_S32
typedef int                     GV_S32;
#endif

#ifndef TYPE_GV_SL32
#define TYPE_GV_SL32
typedef signed long             GV_SL32;
#endif

/*float*/
#ifndef TYPE_GV_FLOAT
#define TYPE_GV_FLOAT
typedef float               	GV_FLOAT;
#endif


/*double*/
#ifndef TYPE_GV_DOUBLE
#define TYPE_GV_DOUBLE
typedef double                  GV_DOUBLE;
#endif

#ifndef TYPE_GV_VOID
#define TYPE_GV_VOID
typedef void                    GV_VOID;
#endif

#define DMA_STATUS_UNFINISHED   0
#define DMA_STATUS_FINISHED     1
	
#define DRIVER_NAME        				"/dev/dma"
#define AXIDMA_IOC_MAGIC            	'A'
#define AXIDMA_IOCCFGANDSTART_AHB       _IO(AXIDMA_IOC_MAGIC, 1)
#define AXIDMA_IOCCFGANDSTART_AXI       _IO(AXIDMA_IOC_MAGIC, 2)

typedef struct dma_chncfg {
    GV_U32 src_addr;
    GV_U32 dst_addr;
    GV_U32 len;
}GV_DMA_CHN_CFG_S;


typedef struct dma_chncfg_sg{
   unsigned int src_addr;
   unsigned int dst_addr;
   unsigned int len;
   unsigned int src_stride;
   unsigned int dst_stride;
   unsigned int high;
}GV_DMA_CHN_CFG_SG_S;


#define DMA_MAX_SG 			32
typedef struct dma_chncfg_multi_sg{
   GV_DMA_CHN_CFG_SG_S chncfg_sg[DMA_MAX_SG];
   unsigned int cnt;
}GV_DMA_CHN_CFG_MULTI_SG_S;

extern GV_S32 gv_dma_device_init();
extern GV_VOID gv_dma_device_deinit(int fd);
extern GV_S32 gv_dma_start(int fd, GV_DMA_CHN_CFG_S *chncfg);
extern GV_S32 gv_hp_dma_start(int fd, GV_DMA_CHN_CFG_SG_S *chncfg_sg);
extern GV_S32 gv_hp_multi_dma_start(int fd, GV_DMA_CHN_CFG_MULTI_SG_S *chncfg_multi_sg);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus*/


#endif /* __GV_POLARIS_DMA_H__*/

