/*
 * Copyright (c) 2005 The Khronos Group Inc. 
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject
 * to the following conditions: 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software. 
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 
 *
 */

/* OMX_CONF_MyComponent.c
 * Sample MPEG2 decoder component 
 */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <OMX_Types.h>
#include <OMX_Component.h>
#include <OMX_Core.h>
#include <OMX_Index.h>
#include <OMX_Image.h>
#include <OMX_Audio.h>
#include <OMX_Video.h>
#include <OMX_IVCommon.h>
#include <OMX_Other.h>

#include "OMX_OSAL_Interfaces.h"
#include "OMX_CONF_StubbedComponent.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// FFMpeg Libraries
#include "avcodec.h"
#include "avformat.h"



/*
 *  D E C L A R A T I O N S 
 */
#define OMX_NOPORT 0xFFFFFFFE
#define NUM_IN_BUFFERS 2        // Input Buffers
#define NUM_OUT_BUFFERS 2       // Output Buffers
#define OMX_TIMEOUT 400         // Timeout value in milisecond
#define OMX_MAX_TIMEOUTS 4      // Count of Maximum number of times the component can time out

/*
 *     D E F I N I T I O N S
 */

typedef struct _BufferList BufferList;


/*
 * The main structure for buffer management.
 *
 *   pBufHdr     - An array of pointers to buffer headers. 
 *                 The size of the array is set dynamically using the nBufferCountActual value
 *                   send by the client.
 *   nListEnd    - Marker to the boundary of the array. This points to the last index of the 
 *                   pBufHdr array.
 *   nSizeOfList - Count of valid data in the list. 
 *   nAllocSize  - Size of the allocated list. This is equal to (nListEnd + 1) in most of 
 *                   the times. When the list is freed this is decremented and at that 
 *                   time the value is not equal to (nListEnd + 1). This is because
 *                   the list is not freed from the end and hence we cannot decrement   
 *                   nListEnd each time we free an element in the list. When nAllocSize is zero,
 *                   the list is completely freed and the other paramaters of the list are  
 *                   initialized.  
 *                 If the client crashes before freeing up the buffers, this parameter is 
 *                   checked (for nonzero value) to see if there are still elements on the list.  
 *                   If yes, then the remaining elements are freed.
 *    nWritePos  - The position where the next buffer would be written. The value is incremented
 *                   after the write. It is wrapped around when it is greater than nListEnd.
 *    nReadPos   - The position from where the next buffer would be read. The value is incremented
 *                   after the read. It is wrapped around when it is greater than nListEnd.
 *    eDir       - Type of BufferList.
 *                            OMX_DirInput  =  Input  Buffer List
 *                           OMX_DirOutput  =  Output Buffer List
 */
struct _BufferList{
   OMX_BUFFERHEADERTYPE **pBufHdr;             
   OMX_U32 nListEnd;   
   OMX_U32 nSizeOfList;
   OMX_U32 nAllocSize;
   OMX_U32 nWritePos;
   OMX_U32 nReadPos;
   OMX_DIRTYPE eDir;
};

static void* ComponentThread(void* pThreadData);


/*
 * Enumeration for the commands processed by the component
 */

typedef enum ThrCmdType
{
    SetState,
    Flush,
    StopPort,
    RestartPort,
    MarkBuf,
    Stop,
    FillBuf,
    EmptyBuf
} ThrCmdType;


/*
 * Private data of the component.
 * It includes input and output port related information (Port definition, format),
 *   buffer related information, specifications for the video decoder, the command and data pipes
 *   and the BufferList structure for storing input and output buffers.
 */

typedef struct MYDATATYPE {
    OMX_STATETYPE state;
    OMX_CALLBACKTYPE *pCallbacks;
    OMX_PTR pAppData;
    OMX_HANDLETYPE hSelf;
    OMX_PORT_PARAM_TYPE sPortParam;
    OMX_PARAM_PORTDEFINITIONTYPE sInPortDef;
    OMX_PARAM_PORTDEFINITIONTYPE sOutPortDef;
    OMX_VIDEO_PARAM_PORTFORMATTYPE sInPortFormat;
    OMX_VIDEO_PARAM_PORTFORMATTYPE sOutPortFormat;
    OMX_PRIORITYMGMTTYPE sPriorityMgmt;
    OMX_PARAM_BUFFERSUPPLIERTYPE sInBufSupplier;
    OMX_PARAM_BUFFERSUPPLIERTYPE sOutBufSupplier;
    OMX_VIDEO_PARAM_MPEG2TYPE sMpeg2;
    pthread_t thread_id;
    OMX_U32 datapipe[2];
    OMX_U32 cmdpipe[2]; 
    OMX_U32 cmddatapipe[2];
    ThrCmdType eTCmd;
    BufferList sInBufList;
    BufferList sOutBufList;
} MYDATATYPE;


/*
 *     M A C R O S
 */



/* 
 * Initializes a data structure using a pointer to the structure.
 * The initialization of OMX structures always sets up the nSize and nVersion fields 
 *   of the structure.
 */
#define OMX_CONF_INIT_STRUCT_PTR(_s_, _name_)	\
    memset((_s_), 0x0, sizeof(_name_));	\
    (_s_)->nSize = sizeof(_name_);		\
    (_s_)->nVersion.s.nVersionMajor = 0x1;	\
    (_s_)->nVersion.s.nVersionMinor = 0x0;	\
    (_s_)->nVersion.s.nRevision = 0x0;		\
    (_s_)->nVersion.s.nStep = 0x0



/*
 * Checking for version compliance.
 * If the nSize of the OMX structure is not set, raises bad parameter error.
 * In case of version mismatch, raises a version mismatch error.
 */
#define OMX_CONF_CHK_VERSION(_s_, _name_, _e_)				\
    if((_s_)->nSize != sizeof(_name_)) _e_ = OMX_ErrorBadParameter;	\
    if(((_s_)->nVersion.s.nVersionMajor != 0x1)||			\
       ((_s_)->nVersion.s.nVersionMinor != 0x0)||			\
       ((_s_)->nVersion.s.nRevision != 0x0)||				\
       ((_s_)->nVersion.s.nStep != 0x0)) _e_ = OMX_ErrorVersionMismatch;\
    if(_e_ != OMX_ErrorNone) goto OMX_CONF_CMD_BAIL;			



/*
 * Checking paramaters for non-NULL values.
 * The macro takes three parameters because inside the code the highest 
 *   number of parameters passed for checking in a single instance is three.
 * In case one or two parameters are passed, the ramaining parameters 
 *   are set to 1 (or a nonzero value). 
 */
#define OMX_CONF_CHECK_CMD(_ptr1, _ptr2, _ptr3)	\
{						\
    if(!_ptr1 || !_ptr2 || !_ptr3){		\
        eError = OMX_ErrorBadParameter;		\
	goto OMX_CONF_CMD_BAIL;			\
    }						\
}



/*
 * Redirects control flow in an error situation.
 * The OMX_CONF_CMD_BAIL label is defined inside the calling function.
 */
#define OMX_CONF_BAIL_IF_ERROR(_eError)		\
{						\
    if(_eError != OMX_ErrorNone)		\
        goto OMX_CONF_CMD_BAIL;			\
}



/*
 * Sets error type and redirects control flow to error handling and cleanup section
 */
#define OMX_CONF_SET_ERROR_BAIL(_eError, _eCode)\
{						\
    _eError = _eCode;				\
    goto OMX_CONF_CMD_BAIL;			\
}




/* 
 * Allocates a new entry in a BufferList.
 * Finds the position where memory has to be allocated.
 * Actual allocation happens in the caller function.
 */
#define ListAllocate(_pH, _nIndex)              \
   if (_pH.nListEnd == -1){                     \
      _pH.nListEnd = 0;                         \
      _pH.nWritePos = 0;                        \
      }                                         \
   else                                         \
   _pH.nListEnd++;                              \
   _pH.nAllocSize++;                            \
   _nIndex = _pH.nListEnd
     



/* 
 * Sets an entry in the BufferList.
 * The entry set is a BufferHeader.
 * The nWritePos value is incremented after the write. 
 * It is wrapped around when it is greater than nListEnd.
 */
#define ListSetEntry(_pH, _pB)                  \
   if (_pH.nSizeOfList < (_pH.nListEnd + 1)){   \
      _pH.nSizeOfList++;                        \
      _pH.pBufHdr[_pH.nWritePos++] = _pB;       \
      if (_pH.nReadPos == -1)                   \
         _pH.nReadPos = 0;                      \
      if (_pH.nWritePos > _pH.nListEnd)         \
         _pH.nWritePos = 0;                     \
      }




/* 
 * Gets an entry from the BufferList
 * The entry is a BufferHeader
 * The nReadPos value is incremented after the read. 
 * It is wrapped around when it is greater than nListEnd.
 */
#define ListGetEntry(_pH, _pB)                  \
   if (_pH.nSizeOfList > 0){                    \
      _pH.nSizeOfList--;                        \
      _pB = _pH.pBufHdr[_pH.nReadPos++];        \
      if (_pH.nReadPos > _pH.nListEnd)          \
         _pH.nReadPos = 0;                      \
      }



/*
 * Flushes all entries from the BufferList structure.
 * The nSizeOfList gives the number of valid entries in the list.
 * The nReadPos value is incremented after the read. 
 * It is wrapped around when it is greater than nListEnd.
 */
#define ListFlushEntries(_pH, _pC)              \
    while (_pH.nSizeOfList > 0){                \
       _pH.nSizeOfList--;                       \
       if (_pH.eDir == OMX_DirInput)            \
          _pC->pCallbacks->EmptyBufferDone(_pC->hSelf,_pC->pAppData,_pH.pBufHdr[_pH.nReadPos++]);\
       else if (_pH.eDir == OMX_DirOutput)      \
          _pC->pCallbacks->FillBufferDone(_pC->hSelf,_pC->pAppData,_pH.pBufHdr[_pH.nReadPos++]);\
       if (_pH.nReadPos > _pH.nListEnd)         \
          _pH.nReadPos = 0;                     \
      }

             
/*
 * Frees the memory allocated for BufferList entries 
 *   by comparing with client supplied buffer header.
 * The nAllocSize value gives the number of allocated (i.e. not free'd) entries in the list. 
 * When nAllocSize is zero, the list is completely freed
 *   and the other paramaters of the list are initialized.   
 */
#define ListFreeBuffer(_pH, _pB, _pP, _nIndex)                       \
    for (_nIndex = 0; _nIndex <= _pH.nListEnd; _nIndex++){           \
        if (_pH.pBufHdr[_nIndex] == _pB){                            \
           _pH.nAllocSize--;                                         \
           if (_pH.pBufHdr[_nIndex]){                                \
              if (_pH.pBufHdr[_nIndex]->pBuffer)                     \
                 _pH.pBufHdr[_nIndex]->pBuffer = NULL;               \
              OMX_BUFFERHEADERTYPE *bufhdr = (OMX_BUFFERHEADERTYPE *)_pH.pBufHdr[_nIndex]; \
              OMX_OSAL_Free(bufhdr);                                 \
              _pH.pBufHdr[_nIndex] = NULL;                           \
              }                                                      \
           if (_pH.nAllocSize == 0){                                 \
              _pH.nWritePos = -1;                                    \
              _pH.nReadPos = -1;                                     \
              _pH.nListEnd = -1;                                     \
              _pH.nSizeOfList = 0;                                   \
              _pP->bPopulated = OMX_FALSE;                           \
              }                                                      \
           break;                                                    \
           }                                                         \
        }


/*
 * Frees the memory allocated for BufferList entries.
 * This is called in case the client crashes suddenly before freeing all the component buffers. 
 * The nAllocSize parameter is 
 *   checked (for nonzero value) to see if there are still elements on the list.  
 * If yes, then the remaining elements are freed. 
 */
#define ListFreeAllBuffers(_pH, _nIndex)                             \
    for (_nIndex = 0; _nIndex <= _pH.nListEnd; _nIndex++){           \
        if (_pH.pBufHdr[_nIndex]){                                   \
           _pH.nAllocSize--;                                         \
           if (_pH.pBufHdr[_nIndex]->pBuffer)                        \
              _pH.pBufHdr[_nIndex]->pBuffer = NULL;                  \
           OMX_BUFFERHEADERTYPE *bufhdr = (OMX_BUFFERHEADERTYPE *)_pH.pBufHdr[_nIndex]; \
           OMX_OSAL_Free(bufhdr);                                    \
           _pH.pBufHdr[_nIndex] = NULL;                              \
           if (_pH.nAllocSize == 0){                                 \
              _pH.nWritePos = -1;                                    \
              _pH.nReadPos = -1;                                     \
              _pH.nListEnd = -1;                                     \
              _pH.nSizeOfList = 0;                                   \
              }                                                      \
           }                                                         \
        }



/*
 * Loads the parameters of the buffer header.
 * When the list has nBufferCountActual elements allocated
 *   then the bPopulated value of port definition is set to true. 
 */
#define LoadBufferHeader(_pList, _pBufHdr, _pAppPrivate, _nSizeBytes, _nPortIndex,    \ 
                                                            _ppBufHdr, _pPortDef)     \ 
    _pBufHdr->nAllocLen = _nSizeBytes;                                                \
    _pBufHdr->pAppPrivate = _pAppPrivate;                                             \
    if (_pList.eDir == OMX_DirInput){                                                 \
       _pBufHdr->nInputPortIndex = _nPortIndex;                                       \
       _pBufHdr->nOutputPortIndex = OMX_NOPORT;                                       \
       }                                                                              \
    else{                                                                             \
       _pBufHdr->nInputPortIndex = OMX_NOPORT;                                        \
       _pBufHdr->nOutputPortIndex = _nPortIndex;                                      \
       }                                                                              \
    _ppBufHdr = _pBufHdr;                                                             \
    if (_pList.nListEnd == (_pPortDef->nBufferCountActual - 1))                       \
       _pPortDef->bPopulated = OMX_TRUE                                  




/*
 * Initializes libavcodec variables.
 * The AVCodec, AVCodecContext and AVFrame structures of ffmpeg are initialized.
 */
#define OMX_CONF_OPEN_DECODER(pCodec, pCodecCtx, pFrame)    	        \
    av_register_all();							\
    pCodec = avcodec_find_decoder(CODEC_ID_MPEG2VIDEO);			\
    if (!pCodec)  							\
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "codec not found\n");	\
    pCodecCtx= avcodec_alloc_context();					\
    pCodecCtx->coded_width = 176;                                       \
    pCodecCtx->coded_height = 144;                                      \
    pCodecCtx->time_base = (AVRational){1,25};                          \
    if(pCodec->capabilities & CODEC_CAP_TRUNCATED)			\
        pCodecCtx->flags|= CODEC_FLAG_TRUNCATED;			\
    if(avcodec_open(pCodecCtx, pCodec) < 0)  				\
        OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "could not open codec\n");	\
    pFrame = avcodec_alloc_frame()					

  

/*
 * Frees libavcodec variables.
 * The AVCodec, AVCodecContext and AVFrame structures of ffmpeg are free'd.
 */
#define OMX_CONF_CLOSE_DECODER(pCodec, pCodecCtx, pFrame)       \
    if(pCodecCtx){						\
        avcodec_close(pCodecCtx);				\
	av_free(pCodecCtx);					\
	pCodecCtx = NULL;					\
	av_free(pFrame);					\
	pFrame = NULL;						\
    }								



/*
 * Uses libavcodec to decode a buffer 
 * 
 *    pCodecCtx - The AVCodecContext structure used for decoding.
 *    pInBuf    - The input buffer.
 *    nSize     - Size of the input buffer
 *    pFrame    - The decoder sets the AVFrame data structure with decoded data.     
 *
 *    returns 0 if the decoded data does not compose a complete frame.
 *            1 if the decoded data composes a complete frame.
 */
int OMX_CONF_DECODE_BUFFER(AVCodecContext *pCodecCtx,uint8_t* pInBuf, int nSize, AVFrame *pFrame)                        
{
    int nLen = 0;								        
    int nFrameDone = 0;							        
    while (nSize>0){								        
      nLen = avcodec_decode_video(pCodecCtx, pFrame, &nFrameDone, pInBuf, nSize);
      if (nLen < 0){								        
	 OMX_OSAL_Trace(OMX_OSAL_TRACE_ERROR, "Error while decoding frame\n"); 
         return 0;
         }  
      if (nFrameDone){								        
         printf("Frame Done\n");
         return 1;
	 }									        
      nSize -= nLen;								        
      pInBuf += nLen;								        
      }
   return 0;
}


/*
 *     F U N C T I O N S 
 */

/*****************************************************************************/
OMX_ERRORTYPE MySendCommand(OMX_IN  OMX_HANDLETYPE hComponent,
			    OMX_IN  OMX_COMMANDTYPE Cmd,
			    OMX_IN  OMX_U32 nParam1,
			    OMX_IN  OMX_PTR pCmdData)
{ 
    MYDATATYPE *pMyData;
    ThrCmdType eCmd;
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    
    pMyData = (MYDATATYPE *)(((OMX_COMPONENTTYPE*)hComponent)->pComponentPrivate);
    OMX_CONF_CHECK_CMD(pMyData, 1, 1);
    if (Cmd == OMX_CommandMarkBuffer) 
       OMX_CONF_CHECK_CMD(pCmdData, 1, 1);

    if (pMyData->state == OMX_StateInvalid)
       OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorInvalidState);

    switch (Cmd){
       case OMX_CommandStateSet: 
	       eCmd = SetState;
	       break;
       case OMX_CommandFlush: 
	       eCmd = Flush;
	       if (nParam1 > 1 && nParam1 != -1)
	          OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorBadPortIndex);
	       break;
       case OMX_CommandPortDisable: 
	       eCmd = StopPort;
	       break;
       case OMX_CommandPortEnable:
	       eCmd = RestartPort;
	       break;
       case OMX_CommandMarkBuffer:
	       eCmd = MarkBuf;
 	       if (nParam1 > 0)
                  OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorBadPortIndex);
               break;
       default:
               break;
       }

    write(pMyData->cmdpipe[1], &eCmd, sizeof(eCmd));

    // In case of MarkBuf, the pCmdData parameter is used to carry the data.
    // In other cases, the nParam1 parameter carries the data.    
    if(eCmd == MarkBuf)
        write(pMyData->cmddatapipe[1], &pCmdData, sizeof(OMX_PTR));
    else 
        write(pMyData->cmddatapipe[1], &nParam1, sizeof(nParam1));
    
OMX_CONF_CMD_BAIL:
    return eError; 
}



/*****************************************************************************/
OMX_ERRORTYPE MyGetState(OMX_IN  OMX_HANDLETYPE hComponent,
			 OMX_OUT OMX_STATETYPE* pState)
{ 
    MYDATATYPE *pMyData;
    OMX_ERRORTYPE eError = OMX_ErrorNone;

    pMyData = (MYDATATYPE *)(((OMX_COMPONENTTYPE*)hComponent)->pComponentPrivate);
    OMX_CONF_CHECK_CMD(pMyData, pState, 1);
    
    *pState = pMyData->state;

OMX_CONF_CMD_BAIL:  
    return eError;
}



/*****************************************************************************/
OMX_ERRORTYPE MySetCallbacks(OMX_IN  OMX_HANDLETYPE hComponent,
			     OMX_IN  OMX_CALLBACKTYPE* pCallbacks, 
			     OMX_IN  OMX_PTR pAppData)
{
    MYDATATYPE *pMyData;
    OMX_ERRORTYPE eError = OMX_ErrorNone;

    pMyData = (MYDATATYPE *)(((OMX_COMPONENTTYPE*)hComponent)->pComponentPrivate);
    OMX_CONF_CHECK_CMD(pMyData, pCallbacks, pAppData);

    pMyData->pCallbacks = pCallbacks;
    pMyData->pAppData = pAppData;
    
OMX_CONF_CMD_BAIL:  
    return eError;
}



/*****************************************************************************/
OMX_ERRORTYPE MyGetParameter(OMX_IN  OMX_HANDLETYPE hComponent, 
			     OMX_IN  OMX_INDEXTYPE nParamIndex,  
			     OMX_INOUT OMX_PTR ComponentParameterStructure)
{   
    MYDATATYPE *pMyData;
    OMX_ERRORTYPE eError = OMX_ErrorNone;

    pMyData = (MYDATATYPE *)(((OMX_COMPONENTTYPE*)hComponent)->pComponentPrivate);
    OMX_CONF_CHECK_CMD(pMyData, ComponentParameterStructure, 1);

    if (pMyData->state == OMX_StateInvalid)
       OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorIncorrectStateOperation);

    switch (nParamIndex){
       // Gets OMX_PORT_PARAM_TYPE structure
       case OMX_IndexParamVideoInit:
	       memcpy(ComponentParameterStructure, &pMyData->sPortParam, sizeof
                                                                       (OMX_PORT_PARAM_TYPE));
	       break;
       // Gets OMX_PARAM_PORTDEFINITIONTYPE structure
       case OMX_IndexParamPortDefinition:
	       if (((OMX_PARAM_PORTDEFINITIONTYPE *)(ComponentParameterStructure))->nPortIndex 
                                                            == pMyData->sInPortDef.nPortIndex)
	          memcpy(ComponentParameterStructure, &pMyData->sInPortDef, sizeof
                                                              (OMX_PARAM_PORTDEFINITIONTYPE));
      	       else if (((OMX_PARAM_PORTDEFINITIONTYPE *)
                            (ComponentParameterStructure))->nPortIndex == 
                  		                              pMyData->sOutPortDef.nPortIndex) 
	               memcpy(ComponentParameterStructure, &pMyData->sOutPortDef, sizeof
                                                              (OMX_PARAM_PORTDEFINITIONTYPE));
	            else
	               eError = OMX_ErrorBadPortIndex;
	       break;
       // Gets OMX_VIDEO_PARAM_PORTFORMATTYPE structure
       case OMX_IndexParamVideoPortFormat:
 	       if (((OMX_VIDEO_PARAM_PORTFORMATTYPE *)(ComponentParameterStructure))->nPortIndex 
                                                          == pMyData->sInPortFormat.nPortIndex){
	          if (((OMX_VIDEO_PARAM_PORTFORMATTYPE *)
                                     (ComponentParameterStructure))->nIndex > 
                                        		   pMyData->sInPortFormat.nIndex)
		     eError = OMX_ErrorNoMore;
		  else
		     memcpy(ComponentParameterStructure, &pMyData->sInPortFormat, sizeof
                                                            (OMX_VIDEO_PARAM_PORTFORMATTYPE));
	          }
	       else if (((OMX_VIDEO_PARAM_PORTFORMATTYPE *)
                               (ComponentParameterStructure))->nPortIndex == 
		                                        pMyData->sOutPortFormat.nPortIndex){
	               if (((OMX_VIDEO_PARAM_PORTFORMATTYPE *)
                               (ComponentParameterStructure))->nIndex > 
		                                        pMyData->sOutPortFormat.nIndex)
		          eError = OMX_ErrorNoMore;
		       else
		          memcpy(ComponentParameterStructure, &pMyData->sOutPortFormat, sizeof
                                                              (OMX_VIDEO_PARAM_PORTFORMATTYPE));
	               }
	            else
	               eError = OMX_ErrorBadPortIndex;
	       break;
       // Gets OMX_PRIORITYMGMTTYPE structure
       case OMX_IndexParamPriorityMgmt:
	       memcpy(ComponentParameterStructure, &pMyData->sPriorityMgmt, sizeof
                                                                        (OMX_PRIORITYMGMTTYPE));
	       break;
       // Gets OMX_VIDEO_PARAM_MPEG2TYPE structure
       case OMX_IndexParamVideoMpeg2:
	       if (((OMX_VIDEO_PARAM_MPEG2TYPE *)(ComponentParameterStructure))->nPortIndex 
                                                                   == pMyData->sMpeg2.nPortIndex)
 	          memcpy(ComponentParameterStructure, &pMyData->sMpeg2, sizeof
                                                                   (OMX_VIDEO_PARAM_MPEG2TYPE));
	       else
	          eError = OMX_ErrorBadPortIndex;
	       break;
       default:
	       eError = OMX_ErrorUnsupportedIndex;
	       break;
       }

OMX_CONF_CMD_BAIL:  
    return eError;
}

/*****************************************************************************/
OMX_ERRORTYPE MySetParameter(
        OMX_IN  OMX_HANDLETYPE hComponent, 
        OMX_IN  OMX_INDEXTYPE nIndex,
        OMX_IN  OMX_PTR ComponentParameterStructure)
{   
    MYDATATYPE *pMyData;
    OMX_ERRORTYPE eError = OMX_ErrorNone;

    pMyData = (MYDATATYPE *)(((OMX_COMPONENTTYPE*)hComponent)->pComponentPrivate);
    OMX_CONF_CHECK_CMD(pMyData, ComponentParameterStructure, 1);

    if (pMyData->state != OMX_StateLoaded)
       OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorIncorrectStateOperation);

    switch (nIndex){
       // Sets OMX_PORT_PARAM_TYPE structure
       case OMX_IndexParamVideoInit:
	       memcpy(&pMyData->sPortParam, ComponentParameterStructure, sizeof
                                                                       (OMX_PORT_PARAM_TYPE));
	       break;
       // Sets OMX_PARAM_PORTDEFINITIONTYPE structure
       case OMX_IndexParamPortDefinition:
	       if (((OMX_PARAM_PORTDEFINITIONTYPE *)(ComponentParameterStructure))->nPortIndex 
                                                             == pMyData->sInPortDef.nPortIndex)
	          memcpy(&pMyData->sInPortDef, ComponentParameterStructure, sizeof
                                                                (OMX_PARAM_PORTDEFINITIONTYPE));
               else if (((OMX_PARAM_PORTDEFINITIONTYPE *)
                               (ComponentParameterStructure))->nPortIndex == 
                                                        pMyData->sOutPortDef.nPortIndex) 
	               memcpy(&pMyData->sOutPortDef, ComponentParameterStructure, sizeof
                                                                (OMX_PARAM_PORTDEFINITIONTYPE));
	            else
	               eError = OMX_ErrorBadPortIndex;
	       break;
       // Sets OMX_VIDEO_PARAM_PORTFORMATTYPE structure
       case OMX_IndexParamVideoPortFormat:
	       if (((OMX_VIDEO_PARAM_PORTFORMATTYPE *)(ComponentParameterStructure))->nPortIndex 
                                                          == pMyData->sInPortFormat.nPortIndex){
	          if (((OMX_VIDEO_PARAM_PORTFORMATTYPE *)(ComponentParameterStructure))->nIndex 
                                                           > pMyData->sInPortFormat.nIndex)
		     eError = OMX_ErrorNoMore;
		  else
		     memcpy(&pMyData->sInPortFormat, ComponentParameterStructure, sizeof
                                                              (OMX_VIDEO_PARAM_PORTFORMATTYPE));
	          }
	       else if (((OMX_VIDEO_PARAM_PORTFORMATTYPE *)
                                    (ComponentParameterStructure))->nPortIndex == 
                                                    pMyData->sOutPortFormat.nPortIndex){
	               if (((OMX_VIDEO_PARAM_PORTFORMATTYPE *)
                                    (ComponentParameterStructure))->nIndex > 
                                                    pMyData->sOutPortFormat.nIndex)
		          eError = OMX_ErrorNoMore;
		       else
		          memcpy(&pMyData->sOutPortFormat, ComponentParameterStructure, sizeof
                                                               (OMX_VIDEO_PARAM_PORTFORMATTYPE));
	               }
	            else
	               eError = OMX_ErrorBadPortIndex;
	       break;
       // Sets OMX_PRIORITYMGMTTYPE structure
       case OMX_IndexParamPriorityMgmt:
	       memcpy(&pMyData->sPriorityMgmt, ComponentParameterStructure, sizeof
                                                                         (OMX_PRIORITYMGMTTYPE));
	       break;
       // Sets OMX_VIDEO_PARAM_MPEG2TYPE structure
       case OMX_IndexParamVideoMpeg2:
	       if (((OMX_VIDEO_PARAM_MPEG2TYPE *)(ComponentParameterStructure))->nPortIndex 
                                                                 == pMyData->sMpeg2.nPortIndex)
	          memcpy(&pMyData->sMpeg2, ComponentParameterStructure, sizeof
                                                                   (OMX_VIDEO_PARAM_MPEG2TYPE));
               else
	          eError = OMX_ErrorBadPortIndex;
	       break;
       default:
	       eError = OMX_ErrorUnsupportedIndex;
	       break;
       }
    
OMX_CONF_CMD_BAIL:  
    return eError;
}



/*****************************************************************************/
OMX_ERRORTYPE MyUseBuffer(OMX_IN OMX_HANDLETYPE hComponent,
			  OMX_INOUT OMX_BUFFERHEADERTYPE** ppBufferHdr,
			  OMX_IN OMX_U32 nPortIndex,
			  OMX_IN OMX_PTR pAppPrivate,
			  OMX_IN OMX_U32 nSizeBytes,
			  OMX_IN OMX_U8* pBuffer)
{   
    MYDATATYPE *pMyData;
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    OMX_PARAM_PORTDEFINITIONTYPE *pPortDef;
    OMX_U32 nIndex = 0x0;
    
    pMyData = (MYDATATYPE *)(((OMX_COMPONENTTYPE*)hComponent)->pComponentPrivate);
    OMX_CONF_CHECK_CMD(pMyData, ppBufferHdr, pBuffer);

    if (nPortIndex == pMyData->sInPortDef.nPortIndex)
       pPortDef = &pMyData->sInPortDef;
    else if (nPortIndex == pMyData->sOutPortDef.nPortIndex)
            pPortDef = &pMyData->sOutPortDef;
         else
            OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorBadParameter);

    if (!pPortDef->bEnabled)
       OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorIncorrectStateOperation);

    if (nSizeBytes != pPortDef->nBufferSize || pPortDef->bPopulated)
       OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorBadParameter);  
    
    // Find an empty position in the BufferList and allocate memory for the buffer header. 
    // Use the buffer passed by the client to initialize the actual buffer 
    // inside the buffer header. 
    if (nPortIndex == pMyData->sInPortDef.nPortIndex){
       ListAllocate(pMyData->sInBufList, nIndex);
       if (pMyData->sInBufList.pBufHdr[nIndex] == NULL){         
          pMyData->sInBufList.pBufHdr[nIndex] = (OMX_BUFFERHEADERTYPE*) 
                                                OMX_OSAL_Malloc(sizeof(OMX_BUFFERHEADERTYPE)) ;
          if (!pMyData->sInBufList.pBufHdr[nIndex])
             OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorInsufficientResources);  
          OMX_CONF_INIT_STRUCT_PTR (pMyData->sInBufList.pBufHdr[nIndex], OMX_BUFFERHEADERTYPE);
          }  
       pMyData->sInBufList.pBufHdr[nIndex]->pBuffer = pBuffer;
       LoadBufferHeader(pMyData->sInBufList, pMyData->sInBufList.pBufHdr[nIndex], pAppPrivate, 
                                            nSizeBytes, nPortIndex, *ppBufferHdr, pPortDef);      
    }else{
       ListAllocate(pMyData->sOutBufList,  nIndex);
       if (pMyData->sOutBufList.pBufHdr[nIndex] == NULL){         
          pMyData->sOutBufList.pBufHdr[nIndex] = (OMX_BUFFERHEADERTYPE*) 
                                                  OMX_OSAL_Malloc(sizeof(OMX_BUFFERHEADERTYPE));
          if (!pMyData->sOutBufList.pBufHdr[nIndex])
              OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorInsufficientResources);  
          OMX_CONF_INIT_STRUCT_PTR (pMyData->sOutBufList.pBufHdr[nIndex], OMX_BUFFERHEADERTYPE);
          }  
       pMyData->sOutBufList.pBufHdr[nIndex]->pBuffer = pBuffer; 
       LoadBufferHeader(pMyData->sOutBufList, pMyData->sOutBufList.pBufHdr[nIndex],  
                                   pAppPrivate, nSizeBytes, nPortIndex, *ppBufferHdr, pPortDef);
       }  

OMX_CONF_CMD_BAIL:  
    return eError;
}



/*****************************************************************************/
OMX_ERRORTYPE MyAllocateBuffer(OMX_IN OMX_HANDLETYPE hComponent,
			       OMX_INOUT OMX_BUFFERHEADERTYPE** ppBufferHdr,
			       OMX_IN OMX_U32 nPortIndex,
			       OMX_IN OMX_PTR pAppPrivate,
			       OMX_IN OMX_U32 nSizeBytes)
{
    MYDATATYPE *pMyData;
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    OMX_S8 nIndex = 0x0;
    OMX_PARAM_PORTDEFINITIONTYPE *pPortDef;

    pMyData = (MYDATATYPE *)(((OMX_COMPONENTTYPE*)hComponent)->pComponentPrivate);
    OMX_CONF_CHECK_CMD(pMyData, ppBufferHdr, 1);

    if (nPortIndex == pMyData->sInPortDef.nPortIndex)
       pPortDef = &pMyData->sInPortDef;
    else{ 
       if (nPortIndex == pMyData->sOutPortDef.nPortIndex)
	  pPortDef = &pMyData->sOutPortDef;
       else
          OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorBadParameter);
       }

    if (!pPortDef->bEnabled)
       OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorIncorrectStateOperation);

    if (nSizeBytes != pPortDef->nBufferSize || pPortDef->bPopulated)
       OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorBadParameter);  

    // Find an empty position in the BufferList and allocate memory for the buffer header 
    // and the actual buffer 
    if (nPortIndex == pMyData->sInPortDef.nPortIndex){
       ListAllocate(pMyData->sInBufList,  nIndex);
       if (pMyData->sInBufList.pBufHdr[nIndex] == NULL){         
          pMyData->sInBufList.pBufHdr[nIndex] = (OMX_BUFFERHEADERTYPE*) 
                                                OMX_OSAL_Malloc(sizeof(OMX_BUFFERHEADERTYPE)) ;
          if (!pMyData->sInBufList.pBufHdr[nIndex])
             OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorInsufficientResources);  
          OMX_CONF_INIT_STRUCT_PTR (pMyData->sInBufList.pBufHdr[nIndex], OMX_BUFFERHEADERTYPE);  
          }  
       pMyData->sInBufList.pBufHdr[nIndex]->pBuffer = (OMX_U8*) 
                                                           OMX_OSAL_Malloc(sizeof(nSizeBytes));
       if (!pMyData->sInBufList.pBufHdr[nIndex]->pBuffer)              
          OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorInsufficientResources);
       LoadBufferHeader(pMyData->sInBufList, pMyData->sInBufList.pBufHdr[nIndex], pAppPrivate, 
                                            nSizeBytes, nPortIndex, *ppBufferHdr, pPortDef);      
      }
    else{
       ListAllocate(pMyData->sOutBufList,  nIndex);
       if (pMyData->sOutBufList.pBufHdr[nIndex] == NULL){         
          pMyData->sOutBufList.pBufHdr[nIndex] = (OMX_BUFFERHEADERTYPE*) 
                                                OMX_OSAL_Malloc(sizeof(OMX_BUFFERHEADERTYPE)) ;
          if (!pMyData->sOutBufList.pBufHdr[nIndex])
             OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorInsufficientResources);  
          OMX_CONF_INIT_STRUCT_PTR(pMyData->sOutBufList.pBufHdr[nIndex],OMX_BUFFERHEADERTYPE);    
          }  
       pMyData->sOutBufList.pBufHdr[nIndex]->pBuffer = (OMX_U8*) 
                                                            OMX_OSAL_Malloc(sizeof(nSizeBytes));
       if (!pMyData->sOutBufList.pBufHdr[nIndex]->pBuffer) 
          OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorInsufficientResources);
       LoadBufferHeader(pMyData->sOutBufList, pMyData->sOutBufList.pBufHdr[nIndex],  
                                   pAppPrivate, nSizeBytes, nPortIndex, *ppBufferHdr, pPortDef);
       }  

OMX_CONF_CMD_BAIL:  
    return eError;
}



/*****************************************************************************/
OMX_ERRORTYPE MyFreeBuffer(OMX_IN  OMX_HANDLETYPE hComponent,
			   OMX_IN  OMX_U32 nPortIndex,
			   OMX_IN  OMX_BUFFERHEADERTYPE* pBufferHdr)
{   
    MYDATATYPE *pMyData;
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    OMX_PARAM_PORTDEFINITIONTYPE *pPortDef;
    OMX_U32 nIndex;
    
    pMyData = (MYDATATYPE *)(((OMX_COMPONENTTYPE*)hComponent)->pComponentPrivate);
    OMX_CONF_CHECK_CMD(pMyData, pBufferHdr, 1);
    OMX_CONF_CHK_VERSION(pBufferHdr, OMX_BUFFERHEADERTYPE, eError);

    // Match the pBufferHdr to the appropriate entry in the BufferList 
    // and free the allocated memory 
    if (nPortIndex == pMyData->sInPortDef.nPortIndex){
       pPortDef = &pMyData->sInPortDef;
       ListFreeBuffer(pMyData->sInBufList, pBufferHdr, pPortDef, nIndex)
       }
    else if (nPortIndex == pMyData->sOutPortDef.nPortIndex){
	    pPortDef = &pMyData->sOutPortDef;
            ListFreeBuffer(pMyData->sOutBufList, pBufferHdr, pPortDef, nIndex)
            }
         else
            OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorBadParameter);

    if (pPortDef->bEnabled && pMyData->state != OMX_StateIdle)
       OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorIncorrectStateOperation);

OMX_CONF_CMD_BAIL:  
    return eError;
}



/*****************************************************************************/
OMX_ERRORTYPE MyEmptyThisBuffer(OMX_IN  OMX_HANDLETYPE hComponent,
				OMX_IN  OMX_BUFFERHEADERTYPE* pBufferHdr)
{
    MYDATATYPE *pMyData;
    ThrCmdType eCmd = EmptyBuf;
    OMX_ERRORTYPE eError = OMX_ErrorNone;

    pMyData = (MYDATATYPE *)(((OMX_COMPONENTTYPE*)hComponent)->pComponentPrivate);
    OMX_CONF_CHECK_CMD(pMyData, pBufferHdr, 1);
    OMX_CONF_CHK_VERSION(pBufferHdr, OMX_BUFFERHEADERTYPE, eError);

    if (!pMyData->sInPortDef.bEnabled)
       OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorIncorrectStateOperation);

    if (pBufferHdr->nInputPortIndex != 0x0  || pBufferHdr->nOutputPortIndex != OMX_NOPORT)
       OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorBadPortIndex);

    if (pMyData->state != OMX_StateExecuting && pMyData->state != OMX_StatePause)
       OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorIncorrectStateOperation);

    // Put the command and data in the pipe 
    write(pMyData->cmdpipe[1], &eCmd, sizeof(eCmd));
    write(pMyData->cmddatapipe[1], &pBufferHdr, sizeof(OMX_BUFFERHEADERTYPE*));

OMX_CONF_CMD_BAIL:  
    return eError;
}

/*****************************************************************************/
OMX_ERRORTYPE MyFillThisBuffer(OMX_IN  OMX_HANDLETYPE hComponent,
			       OMX_IN  OMX_BUFFERHEADERTYPE* pBufferHdr)
{
    MYDATATYPE *pMyData;
    ThrCmdType eCmd = FillBuf;
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    
    pMyData = (MYDATATYPE *)(((OMX_COMPONENTTYPE*)hComponent)->pComponentPrivate);
    OMX_CONF_CHECK_CMD(pMyData, pBufferHdr, 1);
    OMX_CONF_CHK_VERSION(pBufferHdr, OMX_BUFFERHEADERTYPE, eError);

    if (!pMyData->sOutPortDef.bEnabled)
       OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorIncorrectStateOperation);

    if (pBufferHdr->nOutputPortIndex != 0x1 || pBufferHdr->nInputPortIndex != OMX_NOPORT)
       OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorBadPortIndex);

    if (pMyData->state != OMX_StateExecuting && pMyData->state != OMX_StatePause)
       OMX_CONF_SET_ERROR_BAIL(eError, OMX_ErrorIncorrectStateOperation);

    // Put the command and data in the pipe 
    write(pMyData->cmdpipe[1], &eCmd, sizeof(eCmd));
    write(pMyData->cmddatapipe[1], &pBufferHdr, sizeof(OMX_BUFFERHEADERTYPE*));

OMX_CONF_CMD_BAIL:  
    return eError;
}

/*****************************************************************************/
OMX_ERRORTYPE MyComponentDeInit(OMX_IN  OMX_HANDLETYPE hComponent)
{ 
    MYDATATYPE *pMyData;
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    ThrCmdType eCmd = Stop;
    OMX_U32 nIndex = 0;
    
    pMyData = (MYDATATYPE *)(((OMX_COMPONENTTYPE*)hComponent)->pComponentPrivate);

    // In case the client crashes, check for nAllocSize parameter.
    // If this is greater than zero, there are elements in the list that are not free'd.
    // In that case, free the elements.
    if (pMyData->sInBufList.nAllocSize > 0)
       ListFreeAllBuffers(pMyData->sInBufList, nIndex)   
    if (pMyData->sOutBufList.nAllocSize > 0)
       ListFreeAllBuffers(pMyData->sOutBufList, nIndex)   
    
    // Put the command and data in the pipe 
    write(pMyData->cmdpipe[1], &eCmd, sizeof(eCmd));
    write(pMyData->cmddatapipe[1], &eCmd, sizeof(eCmd));

    // Wait for thread to exit so we can get the status into "error"
    pthread_join(pMyData->thread_id, (void*)&eError);

    // close the pipe handles 
    close(pMyData->cmdpipe[0]);
    close(pMyData->cmdpipe[1]);
    close(pMyData->cmddatapipe[0]);
    close(pMyData->cmddatapipe[1]);
    OMX_OSAL_Free(pMyData);

    return eError;
}

/*****************************************************************************/
OMX_API OMX_ERRORTYPE MyComponentInit(OMX_IN  OMX_HANDLETYPE hComponent)
{
    OMX_COMPONENTTYPE *pComp;
    MYDATATYPE *pMyData; 
    OMX_ERRORTYPE eError = OMX_ErrorNone;
    OMX_U32 err;
    OMX_U32 nIndex;

    pComp = (OMX_COMPONENTTYPE *)hComponent;

    // Create private data
    pMyData = (MYDATATYPE *)OMX_OSAL_Malloc(sizeof(MYDATATYPE));
    memset(pMyData, 0x0, sizeof(MYDATATYPE));

    pComp->pComponentPrivate = (OMX_PTR)pMyData;
    pMyData->state = OMX_StateLoaded;
    pMyData->hSelf = hComponent; 
    
    // Fill in function pointers 
    pComp->SetCallbacks =           MySetCallbacks;
    pComp->GetComponentVersion =    StubbedGetComponentVersion;
    pComp->SendCommand =            MySendCommand;
    pComp->GetParameter =           MyGetParameter;
    pComp->SetParameter =           MySetParameter;
    pComp->GetConfig =              StubbedGetConfig;
    pComp->SetConfig =              StubbedSetConfig;
    pComp->GetExtensionIndex =      StubbedGetExtensionIndex;
    pComp->GetState =               MyGetState;
    pComp->ComponentTunnelRequest = StubbedComponentTunnelRequest;
    pComp->UseBuffer =              MyUseBuffer;
    pComp->AllocateBuffer =         MyAllocateBuffer;
    pComp->FreeBuffer =             MyFreeBuffer;
    pComp->EmptyThisBuffer =        MyEmptyThisBuffer;
    pComp->FillThisBuffer =         MyFillThisBuffer;
    pComp->ComponentDeInit =        MyComponentDeInit;

    // Initialize component data structures to default values 
    OMX_CONF_INIT_STRUCT_PTR(&pMyData->sPortParam, OMX_PORT_PARAM_TYPE);
    pMyData->sPortParam.nPorts = 0x2;
    pMyData->sPortParam.nStartPortNumber = 0x0;

    // Initialize the video parameters for input port 
    OMX_CONF_INIT_STRUCT_PTR(&pMyData->sInPortDef, OMX_PARAM_PORTDEFINITIONTYPE);
    pMyData->sInPortDef.nPortIndex = 0x0;
    pMyData->sInPortDef.bEnabled = OMX_TRUE;
    pMyData->sInPortDef.bPopulated = OMX_FALSE;
    pMyData->sInPortDef.eDomain = OMX_PortDomainVideo;
    pMyData->sInPortDef.format.video.cMIMEType = "MPEG2";
    pMyData->sInPortDef.format.video.nFrameWidth = 176;
    pMyData->sInPortDef.format.video.nFrameHeight = 144;
    pMyData->sInPortDef.eDir = OMX_DirInput;
    pMyData->sInPortDef.nBufferCountMin = NUM_IN_BUFFERS;
    pMyData->sInPortDef.nBufferCountActual = NUM_IN_BUFFERS;
    pMyData->sInPortDef.nBufferSize =  (OMX_U32)(2*1024);
    pMyData->sInPortDef.format.video.eCompressionFormat =  OMX_VIDEO_CodingMPEG2;

    // Initialize the video parameters for output port
    OMX_CONF_INIT_STRUCT_PTR(&pMyData->sOutPortDef, OMX_PARAM_PORTDEFINITIONTYPE);
    pMyData->sOutPortDef.nPortIndex = 0x1;
    pMyData->sOutPortDef.bEnabled = OMX_TRUE;
    pMyData->sOutPortDef.bPopulated = OMX_FALSE;
    pMyData->sOutPortDef.eDomain = OMX_PortDomainVideo;
    pMyData->sOutPortDef.format.video.cMIMEType = "YUV420";
    pMyData->sOutPortDef.format.video.nFrameWidth = 176;
    pMyData->sOutPortDef.format.video.nFrameHeight = 144;
    pMyData->sOutPortDef.eDir = OMX_DirOutput;
    pMyData->sOutPortDef.nBufferCountMin = NUM_OUT_BUFFERS; 
    pMyData->sOutPortDef.nBufferCountActual = NUM_OUT_BUFFERS;
    pMyData->sOutPortDef.nBufferSize =  (OMX_U32)(2*176*144);
    pMyData->sOutPortDef.format.video.eColorFormat =  OMX_COLOR_FormatYUV420Planar;

    // Initialize the video compression format for input port 
    OMX_CONF_INIT_STRUCT_PTR(&pMyData->sInPortFormat, OMX_VIDEO_PARAM_PORTFORMATTYPE);
    pMyData->sInPortFormat.nPortIndex = 0x0;
    pMyData->sInPortFormat.nIndex = 0x0;
    pMyData->sInPortFormat.eCompressionFormat =  OMX_VIDEO_CodingMPEG2;

    // Initialize the compression format for output port
    OMX_CONF_INIT_STRUCT_PTR(&pMyData->sOutPortFormat, OMX_VIDEO_PARAM_PORTFORMATTYPE);
    pMyData->sOutPortFormat.nPortIndex = 0x1;
    pMyData->sOutPortFormat.nIndex = 0x0;
    pMyData->sOutPortFormat.eColorFormat =  OMX_COLOR_FormatYUV420Planar;

    OMX_CONF_INIT_STRUCT_PTR(&pMyData->sPriorityMgmt, OMX_PRIORITYMGMTTYPE);

    OMX_CONF_INIT_STRUCT_PTR(&pMyData->sInBufSupplier, OMX_PARAM_BUFFERSUPPLIERTYPE );
    pMyData->sInBufSupplier.nPortIndex = 0x0;

    OMX_CONF_INIT_STRUCT_PTR(&pMyData->sOutBufSupplier, OMX_PARAM_BUFFERSUPPLIERTYPE );
    pMyData->sOutBufSupplier.nPortIndex = 0x1;

    OMX_CONF_INIT_STRUCT_PTR(&pMyData->sMpeg2, OMX_VIDEO_PARAM_MPEG2TYPE);
    pMyData->sMpeg2.nPortIndex = 0x0;
 
    // Initialize the input buffer list 
    memset(&(pMyData->sInBufList), 0x0, sizeof(BufferList));
    printf ("Was here too"); 
    pMyData->sInBufList.pBufHdr = (OMX_BUFFERHEADERTYPE**) 
                               OMX_OSAL_Malloc (sizeof(OMX_BUFFERHEADERTYPE*) * 
                                                       pMyData->sInPortDef.nBufferCountActual);  
    for (nIndex = 0; nIndex < pMyData->sInPortDef.nBufferCountActual; nIndex++) {
        pMyData->sInBufList.pBufHdr[nIndex] = (OMX_BUFFERHEADERTYPE*) 
                                                 OMX_OSAL_Malloc(sizeof(OMX_BUFFERHEADERTYPE));
        OMX_CONF_INIT_STRUCT_PTR (pMyData->sInBufList.pBufHdr[nIndex], OMX_BUFFERHEADERTYPE);
        }
    pMyData->sInBufList.nSizeOfList = 0;
    pMyData->sInBufList.nAllocSize = 0;
    pMyData->sInBufList.nListEnd = -1;
    pMyData->sInBufList.nWritePos = -1;
    pMyData->sInBufList.nReadPos = -1;
    pMyData->sInBufList.eDir = OMX_DirInput;    

    // Initialize the output buffer list 
    memset(&(pMyData->sOutBufList), 0x0, sizeof(BufferList));
    pMyData->sOutBufList.pBufHdr = (OMX_BUFFERHEADERTYPE**) 
                                         OMX_OSAL_Malloc (sizeof(OMX_BUFFERHEADERTYPE*) * 
                                                      pMyData->sOutPortDef.nBufferCountActual); 
    for (nIndex = 0; nIndex < pMyData->sOutPortDef.nBufferCountActual; nIndex++) {
        pMyData->sOutBufList.pBufHdr[nIndex] = (OMX_BUFFERHEADERTYPE*) 
                                                 OMX_OSAL_Malloc(sizeof(OMX_BUFFERHEADERTYPE));
        OMX_CONF_INIT_STRUCT_PTR (pMyData->sOutBufList.pBufHdr[nIndex], OMX_BUFFERHEADERTYPE);
        }
    pMyData->sOutBufList.nSizeOfList = 0;
    pMyData->sOutBufList.nAllocSize = 0;
    pMyData->sOutBufList.nListEnd = -1;
    pMyData->sOutBufList.nWritePos = -1;
    pMyData->sOutBufList.nReadPos = -1;
    pMyData->sOutBufList.eDir = OMX_DirOutput;
    
    // Create the pipe used to send commands to the thread 
    err = pipe(pMyData->cmdpipe);
    if (err){
       eError = OMX_ErrorInsufficientResources;
       goto EXIT;
       }

    // Create the pipe used to send command data to the thread 
    err = pipe(pMyData->cmddatapipe);
    if (err){
       eError = OMX_ErrorInsufficientResources;
       goto EXIT;
       }
    
    // Create the component thread 
    err = pthread_create(&pMyData->thread_id, NULL, ComponentThread, pMyData);
    if( err || !pMyData->thread_id ) {
        eError = OMX_ErrorInsufficientResources;
        goto EXIT;
    }
EXIT:
    return eError;
}


/*
 *  Component Thread 
 *    The ComponentThread function is exeuted in a separate pThread and
 *    is used to implement the actual component functions. 
 */
 /*****************************************************************************/
static void* ComponentThread(void* pThreadData)
{
    int i, fd1;
    fd_set rfds;
    OMX_U32 cmddata;
    ThrCmdType cmd;

    // Variables related to FFMpeg 
    AVCodecContext *pCodecCtx = NULL;
    AVCodec *pCodec  = NULL;
    AVFrame *pFrame  = NULL; 
    OMX_U32 frameDone;
    OMX_S32 nRetValue;

    // Variables related to decoder buffer handling 
    OMX_BUFFERHEADERTYPE *pInBufHdr = NULL;
    OMX_BUFFERHEADERTYPE *pOutBufHdr = NULL;
    OMX_MARKTYPE *pMarkBuf = NULL;
    OMX_U8 *pInBuf = NULL;
    OMX_U32 nInBufSize;
    
    // Variables related to decoder timeouts 
    OMX_HANDLETYPE hTimeout;
    OMX_BOOL bTimeout;
    OMX_U32 nTimeout;

    // Recover the pointer to my component specific data 
    MYDATATYPE* pMyData = (MYDATATYPE*)pThreadData;
    OMX_OSAL_EventCreate(&hTimeout);

    while (1){
       fd1 = pMyData->cmdpipe[0];
       FD_ZERO(&rfds);
       FD_SET(fd1,&rfds);

       // Check for new command 
       i = select(pMyData->cmdpipe[0]+1, &rfds, NULL, NULL, NULL);

       if (FD_ISSET(pMyData->cmdpipe[0], &rfds)){
          // retrieve command and data from pipe
          read(pMyData->cmdpipe[0], &cmd, sizeof(cmd));
	  read(pMyData->cmddatapipe[0], &cmddata, sizeof(cmddata));
           
          // State transition command
	  if (cmd == SetState){
             // If the parameter states a transition to the same state
             //   raise a same state transition error. 
             if (pMyData->state == (OMX_STATETYPE)(cmddata))
                pMyData->pCallbacks->EventHandler(pMyData->hSelf, pMyData->pAppData, 
                                                OMX_EventError, OMX_ErrorSameState, 0 , NULL);    
             else{   
	        // transitions/callbacks made based on state transition table 
                // cmddata contains the target state 
	        switch ((OMX_STATETYPE)(cmddata)){
             	   case OMX_StateInvalid:
		           pMyData->state = OMX_StateInvalid;
		           pMyData->pCallbacks->EventHandler(pMyData->hSelf, pMyData->pAppData, 
                                               OMX_EventError, OMX_ErrorInvalidState, 0 , NULL);
  		           pMyData->pCallbacks->EventHandler(pMyData->hSelf,pMyData->pAppData, 
                               OMX_EventCmdComplete, OMX_CommandStateSet, pMyData->state, NULL);
			   break;
		   case OMX_StateLoaded:
                           if (pMyData->state == OMX_StateIdle || 
                                                pMyData->state == OMX_StateWaitForResources){
			      nTimeout = 0x0;
			      while (1){
                                  // Transition happens only when the ports are unpopulated
			          if (!pMyData->sInPortDef.bPopulated && 
                                                     !pMyData->sOutPortDef.bPopulated){
 		                     pMyData->state = OMX_StateLoaded;
                   	             pMyData->pCallbacks->EventHandler(pMyData->hSelf,
                                                   pMyData->pAppData, OMX_EventCmdComplete, 
                                                   OMX_CommandStateSet, pMyData->state, NULL);
 			             // close decoder
                                     OMX_CONF_CLOSE_DECODER(pCodec, pCodecCtx, pFrame)
				     break;
 	                             }
				  else if (nTimeout++ > OMX_MAX_TIMEOUTS){
                   	                  pMyData->pCallbacks->EventHandler(
                                             pMyData->hSelf,pMyData->pAppData, OMX_EventError, 
                                                    OMX_ErrorInsufficientResources, 0 , NULL);
 	                                  OMX_OSAL_Trace(OMX_OSAL_TRACE_INFO, 
                                                             "Transition to loaded failed\n");
				          break;
				          }
				  OMX_OSAL_EventReset(hTimeout);
				  OMX_OSAL_EventWait(hTimeout, OMX_TIMEOUT, &bTimeout);
			          }
		              }
			   else
			      pMyData->pCallbacks->EventHandler(pMyData->hSelf,pMyData->pAppData, 
                                    OMX_EventError, OMX_ErrorIncorrectStateTransition, 0 , NULL);
			   break;
                   case OMX_StateIdle:
		           if (pMyData->state == OMX_StateInvalid)
		              pMyData->pCallbacks->EventHandler(pMyData->hSelf,pMyData->pAppData, 
                                     OMX_EventError, OMX_ErrorIncorrectStateTransition, 0 , NULL);
		           else{
			      // Return buffers if currently in pause and executing
			      if (pMyData->state == OMX_StatePause || 
                                                  pMyData->state == OMX_StateExecuting){  
                                 ListFlushEntries(pMyData->sInBufList, pMyData)  
			         ListFlushEntries(pMyData->sOutBufList, pMyData)
			         }
			      nTimeout = 0x0;
			      while (1){
			            // Ports have to be populated before transition completes
			            if ((!pMyData->sInPortDef.bEnabled && 
                                            !pMyData->sOutPortDef.bEnabled)||
                                          (pMyData->sInPortDef.bPopulated && 
                                                  pMyData->sOutPortDef.bPopulated)){
                                       pMyData->state = OMX_StateIdle;
                                       pMyData->pCallbacks->EventHandler(pMyData->hSelf,
                                               pMyData->pAppData, OMX_EventCmdComplete, 
                                                     OMX_CommandStateSet, pMyData->state, NULL);
                                       // Open decoder 
                                       //OMX_CONF_OPEN_DECODER(pCodec, pCodecCtx, pFrame);
                                       // Allocate input buffer
                                       pInBuf = (OMX_U8*)OMX_OSAL_Malloc(2048);
 		                       break;
			               }
			            else if (nTimeout++ > OMX_MAX_TIMEOUTS){
                                       pMyData->pCallbacks->EventHandler(pMyData->hSelf,
                                                      pMyData->pAppData, OMX_EventError, 
                                                      OMX_ErrorInsufficientResources, 0 , NULL);
                		       OMX_OSAL_Trace(OMX_OSAL_TRACE_INFO, 
                                                              "Idle transition failed\n");
	                               break;
				       }
			            OMX_OSAL_EventReset(hTimeout);
			            OMX_OSAL_EventWait(hTimeout, OMX_TIMEOUT, &bTimeout);
			            }
		              }
		           break;
		   case OMX_StateExecuting:
                           // Transition can only happen from pause or idle state 
                           if (pMyData->state == OMX_StateIdle || 
                                                       pMyData->state == OMX_StatePause){
                              // Return buffers if currently in pause
			      if (pMyData->state == OMX_StatePause){
			         ListFlushEntries(pMyData->sInBufList, pMyData)
			         ListFlushEntries(pMyData->sOutBufList, pMyData)
			         }
                              pMyData->state = OMX_StateExecuting;
                              pMyData->pCallbacks->EventHandler(pMyData->hSelf,pMyData->pAppData,
				 OMX_EventCmdComplete, OMX_CommandStateSet, pMyData->state, NULL);
			      }
			   else
			      pMyData->pCallbacks->EventHandler(pMyData->hSelf,pMyData->pAppData, 
                                     OMX_EventError, OMX_ErrorIncorrectStateTransition, 0 , NULL);
			   break;
                   case OMX_StatePause:
                           // Transition can only happen from idle or executing state 
		           if (pMyData->state == OMX_StateIdle || 
                                                       pMyData->state == OMX_StateExecuting){
			      pMyData->state = OMX_StatePause;
			      pMyData->pCallbacks->EventHandler(pMyData->hSelf,pMyData->pAppData,
				 OMX_EventCmdComplete, OMX_CommandStateSet, pMyData->state, NULL);
		              }
			   else 
			      pMyData->pCallbacks->EventHandler(pMyData->hSelf,pMyData->pAppData, 
                                     OMX_EventError, OMX_ErrorIncorrectStateTransition, 0 , NULL);
		           break;
                   case OMX_StateWaitForResources:
		           if (pMyData->state == OMX_StateLoaded) {
		      	      pMyData->state = OMX_StateWaitForResources;
			      pMyData->pCallbacks->EventHandler(pMyData->hSelf,pMyData->pAppData, 
                                 OMX_EventCmdComplete, OMX_CommandStateSet, pMyData->state, NULL);
			      } 
			   else
			      pMyData->pCallbacks->EventHandler(pMyData->hSelf,pMyData->pAppData, 
                                     OMX_EventError, OMX_ErrorIncorrectStateTransition, 0 , NULL);
		           break;
		   }
                }
	     }
	  else if (cmd == StopPort){
                  // Stop Port(s)
                  // cmddata contains the port index to be stopped.
                  // It is assumed that 0 is input and 1 is output port for this component
                  // The cmddata value -1 means that both input and output ports will be stopped.
	          if (cmddata == 0x0 || cmddata == -1){
                     // Return all input buffers 
                     ListFlushEntries(pMyData->sInBufList, pMyData) 
 		     // Disable port 
		     pMyData->sInPortDef.bEnabled = OMX_FALSE;
		     }
	          if (cmddata == 0x1 || cmddata == -1){
		     // Return all output buffers 
                     ListFlushEntries(pMyData->sOutBufList, pMyData) 
       	             // Disable port
		     pMyData->sOutPortDef.bEnabled = OMX_FALSE;
		     }
		  // Wait for all buffers to be freed
		  nTimeout = 0x0;
		  while (1){
		        if (cmddata == 0x0 && !pMyData->sInPortDef.bPopulated){
		           // Return cmdcomplete event if input unpopulated 
		           pMyData->pCallbacks->EventHandler(pMyData->hSelf, pMyData->pAppData, 
                                         OMX_EventCmdComplete, OMX_CommandPortDisable, 0x0, NULL);
			   break;
		           }
		        if (cmddata == 0x1 && !pMyData->sOutPortDef.bPopulated){
		           // Return cmdcomplete event if output unpopulated 
		           pMyData->pCallbacks->EventHandler(pMyData->hSelf, pMyData->pAppData, 
                                         OMX_EventCmdComplete, OMX_CommandPortDisable, 0x1, NULL);
			   break;
		           }
		        if (cmddata == -1 &&  !pMyData->sInPortDef.bPopulated && 
                                                  !pMyData->sOutPortDef.bPopulated){
            		   // Return cmdcomplete event if inout & output unpopulated 
		           pMyData->pCallbacks->EventHandler(pMyData->hSelf, pMyData->pAppData, 
                                         OMX_EventCmdComplete, OMX_CommandPortDisable, 0x0, NULL);
			   pMyData->pCallbacks->EventHandler(pMyData->hSelf, pMyData->pAppData, 
                                         OMX_EventCmdComplete, OMX_CommandPortDisable, 0x1, NULL);
			   break;
		           }
		        if (nTimeout++ > OMX_MAX_TIMEOUTS){
		           pMyData->pCallbacks->EventHandler(pMyData->hSelf,pMyData->pAppData, 
                                   OMX_EventError, OMX_ErrorPortUnresponsiveDuringDeallocation, 
                                                                                     0 , NULL);
       		           break;
		           }
		        OMX_OSAL_EventReset(hTimeout);
		        OMX_OSAL_EventWait(hTimeout, OMX_TIMEOUT, &bTimeout);
		        }
	          }
	  else if (cmd == RestartPort){
                  // Restart Port(s)
                  // cmddata contains the port index to be restarted.
                  // It is assumed that 0 is input and 1 is output port for this component.
                  // The cmddata value -1 means both input and output ports will be restarted.
                  if (cmddata == 0x0 || cmddata == -1)
	             pMyData->sInPortDef.bEnabled = OMX_TRUE;
	          if (cmddata == 0x1 || cmddata == -1)
	             pMyData->sOutPortDef.bEnabled = OMX_TRUE;
 	          // Wait for port to be populated 
		  nTimeout = 0x0;
		  while (1){
                        // Return cmdcomplete event if input port populated 
		        if (cmddata == 0x0 && (pMyData->state == OMX_StateLoaded || 
                                                        pMyData->sInPortDef.bPopulated)){
		           pMyData->pCallbacks->EventHandler(pMyData->hSelf, pMyData->pAppData, 
                                        OMX_EventCmdComplete, OMX_CommandPortEnable, 0x0, NULL);
			   break;
		           }
                        // Return cmdcomplete event if output port populated 
		        else if (cmddata == 0x1 && (pMyData->state == OMX_StateLoaded || 
                                                             pMyData->sOutPortDef.bPopulated)){
                                pMyData->pCallbacks->EventHandler(pMyData->hSelf, 
                                                  pMyData->pAppData, OMX_EventCmdComplete, 
                                                              OMX_CommandPortEnable, 0x1, NULL);
                                break;
		                }
                             // Return cmdcomplete event if input and output ports populated 
		             else if (cmddata == -1 && (pMyData->state == OMX_StateLoaded || 
                                                  (pMyData->sInPortDef.bPopulated && 
                                                         pMyData->sOutPortDef.bPopulated))){
 		                     pMyData->pCallbacks->EventHandler(pMyData->hSelf, 
                                                pMyData->pAppData, OMX_EventCmdComplete, 
                                                            OMX_CommandPortEnable, 0x0, NULL);
				     pMyData->pCallbacks->EventHandler(pMyData->hSelf, 
                                                pMyData->pAppData, OMX_EventCmdComplete, 
                                                             OMX_CommandPortEnable, 0x1, NULL);
			             break;
		                     }
		                  else if (nTimeout++ > OMX_MAX_TIMEOUTS){
		                          pMyData->pCallbacks->EventHandler(
                                                   pMyData->hSelf,pMyData->pAppData, 
                                                   OMX_EventError, 
                                                OMX_ErrorPortUnresponsiveDuringAllocation,0,NULL);
                                          break;
	                                  }
	                OMX_OSAL_EventReset(hTimeout);
		        OMX_OSAL_EventWait(hTimeout, OMX_TIMEOUT, &bTimeout);
		        }
	          }
	  else if (cmd == Flush){
	          // Flush port(s) 
                  // cmddata contains the port index to be flushed.
                  // It is assumed that 0 is input and 1 is output port for this component
                  // The cmddata value -1 means that both input and output ports will be flushed.
	          if (cmddata == 0x0 || cmddata == -1){
	             // Return all input buffers and send cmdcomplete
                     ListFlushEntries(pMyData->sInBufList, pMyData)
                     pMyData->pCallbacks->EventHandler(pMyData->hSelf, pMyData->pAppData, 
                                              OMX_EventCmdComplete, OMX_CommandFlush, 0x0, NULL);
		     }
	          if (cmddata == 0x1 || cmddata == -1){
	             // Return all output buffers and send cmdcomplete
                     ListFlushEntries(pMyData->sOutBufList, pMyData)
         	     pMyData->pCallbacks->EventHandler(pMyData->hSelf, pMyData->pAppData, 
                                              OMX_EventCmdComplete, OMX_CommandFlush, 0x1, NULL);
		     }
	          }
	  else if (cmd == Stop){
  		  // Kill thread 
 	          goto EXIT;
	          }
	  else if (cmd == FillBuf){
	          // Fill buffer 
                  ListSetEntry(pMyData->sOutBufList, ((OMX_BUFFERHEADERTYPE*) cmddata)) 
                  }
	  else if (cmd == EmptyBuf){
	          // Empty buffer 
	          ListSetEntry(pMyData->sInBufList, ((OMX_BUFFERHEADERTYPE *) cmddata)) 
        	  // Mark current buffer if there is outstanding command 
		  if (pMarkBuf){
		     ((OMX_BUFFERHEADERTYPE *)(cmddata))->hMarkTargetComponent = 
                                                       pMarkBuf->hMarkTargetComponent;
		     ((OMX_BUFFERHEADERTYPE *)(cmddata))->pMarkData = pMarkBuf->pMarkData;
		     pMarkBuf = NULL;
		     }
		  }
	  else if (cmd == MarkBuf){
	          if (!pMarkBuf)
	             pMarkBuf = (OMX_MARKTYPE *)(cmddata);
	          }
	  }
       // Buffer processing 
       // Only happens when the component is in executing state. 
       if (pMyData->state == OMX_StateExecuting && pMyData->sInPortDef.bEnabled && 
                          pMyData->sOutPortDef.bEnabled && (pMyData->sInBufList.nSizeOfList > 0) 
                                   && ((pMyData->sOutBufList.nSizeOfList > 0) || pOutBufHdr)){
  	  ListGetEntry(pMyData->sInBufList, pInBufHdr)
          // If there is no output buffer, get one from list 
	  if (!pOutBufHdr)
             ListGetEntry(pMyData->sOutBufList, pOutBufHdr)
  	  // Check for EOS flag 
          if (pInBufHdr){
             if (pInBufHdr->nFlags & OMX_BUFFERFLAG_EOS){
	        // Copy flag to output buffer header 
                if (pOutBufHdr)  
	           pOutBufHdr->nFlags |= OMX_BUFFERFLAG_EOS;
 	        // Trigger event handler 
	        pMyData->pCallbacks->EventHandler(pMyData->hSelf, pMyData->pAppData, 
                                        OMX_EventBufferFlag, 0x1, pInBufHdr->nFlags, NULL);
 		// Clear flag 
		pInBufHdr->nFlags = 0;
	        }
	     // Check for mark buffers 
	     if (pInBufHdr->pMarkData){
                // Copy mark to output buffer header 
	        if (pOutBufHdr){   
	           pOutBufHdr->pMarkData = pInBufHdr->pMarkData;
                   // Copy handle to output buffer header 
                   pOutBufHdr->hMarkTargetComponent = pInBufHdr->hMarkTargetComponent;}
 		   }
		// Trigger event handler 
	        if (pInBufHdr->hMarkTargetComponent == pMyData->hSelf && pInBufHdr->pMarkData)
		   pMyData->pCallbacks->EventHandler(pMyData->hSelf, pMyData->pAppData, 
                                                    OMX_EventMark, 0, 0, pInBufHdr->pMarkData);
                }
             memcpy(pInBuf, pInBufHdr->pBuffer, pInBufHdr->nFilledLen);
             if (pCodecCtx == NULL)
                OMX_CONF_OPEN_DECODER(pCodec, pCodecCtx, pFrame);
             if (pCodecCtx){
                if (pCodecCtx->coded_width !=176 || pCodecCtx->coded_height !=144)
                   OMX_CONF_OPEN_DECODER(pCodec, pCodecCtx, pFrame);
             }  
	  nInBufSize = pInBufHdr->nFilledLen;
          // Return input buffer to client for refill
          pMyData->pCallbacks->EmptyBufferDone(pMyData->hSelf, pMyData->pAppData, pInBufHdr);
          // Decode frame 
          nRetValue = OMX_CONF_DECODE_BUFFER(pCodecCtx, pInBuf, nInBufSize, pFrame);	
          if (nRetValue == 1){
             pOutBufHdr->nFilledLen = pCodecCtx->coded_width * pCodecCtx->coded_height;
             memcpy(pOutBufHdr->pBuffer, pFrame->data[0], pOutBufHdr->nFilledLen);
             pMyData->pCallbacks->FillBufferDone(pMyData->hSelf, pMyData->pAppData, pOutBufHdr);
             pOutBufHdr = NULL;  
             }  
          pInBufHdr = NULL;  
          }
       if (pOutBufHdr){
          pMyData->pCallbacks->FillBufferDone(pMyData->hSelf, pMyData->pAppData, pOutBufHdr);
          pOutBufHdr = NULL;
          }
       }

EXIT:
    OMX_OSAL_EventDestroy(hTimeout);
    return (void*)OMX_ErrorNone;
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

/* File EOF */
