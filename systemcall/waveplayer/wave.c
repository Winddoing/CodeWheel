#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <linux/soundcard.h>

#include <wave.h>

#define WAVE_FLAG "wave"

typedef struct{
	char 		riffFlag[4];
	uint32_t 	fileLength;
	char		waveFlag[4];
	char		fmtFlag[4];
	char		unused[4];
	uint16_t	fmtType;
	uint16_t	channels;
	uint16_t	rate;
	uint32_t	bps;
	uint16_t	blockSize;
	uint16_t	bits;
	char		dataFlag[4];
	uint32_t	voiceLength;
}WaveHead;
/* ****************************** *
 * 函数名:initWaveDecoder         *
 * 功  能:初始化并读取音步头信息  *
 * 参  数:fileName 音频文件名     *
 *        decoder 音频解码结构    *
 * 返回值:成功0，失败返回错误号   *
 * ****************************** */
int initWaveDecoder(const char *fileName, WaveDecoder* decoder)
{
	WaveHead head;	//用于从文件中读取音频头信息的结构

	if(NULL == fileName) return WAVE_SPECERR;	//未指定文件名
	decoder->fileName = fileName;		//将文件名存入结构
	/* 1.打开WAVE音频文件 */
	if(-1 == (decoder->fd = open(fileName, O_RDONLY)))
		return WAVE_OPENERR;	//文件打开失败
	/* 2.读取音频头信息 */
	if(-1 == read(decoder->fd, &head, sizeof(head)))
	{	//出错处理
		close(decoder->fd);
		decoder->fd = -1;
		return WAVE_BADFILE;
	}
	/* 3.检查文件类型 */
	if(0 != strncasecmp(head.waveFlag, WAVE_FLAG, 4))
	{
		close(decoder->fd);
		decoder->fd = -1;
		return WAVE_INVTYPE;
	}
	/* 4.设置音频数据读取的起始偏移量 */
	decoder->offset = -1;	//代表顺序读数据
	/* 5.设置每次读取的音频数据长度 */
	decoder->bytesPerRead = 	//每次读取1秒的数据
		head.bits / 8 * head.rate * head.channels;
	/* 6.为读取数到的数据分配内存 */
	decoder->buffer = (uint8_t *)malloc(
					decoder->bytesPerRead);
	if(NULL == decoder->buffer)//出错处理
	{
		close(decoder->fd);
		decoder->fd = -1;
		return WAVE_NOMEMOR;
	}
	/* 7.保存音频数据参数 */
	decoder->args.rate = head.rate;
	decoder->args.bits = head.bits;
	decoder->args.channels = head.channels;
	/* 8.初始化已读的数据长度 */
	decoder->dataLength = 0;
	return WAVE_SUCCESS;	
}

/* ****************************** *
 * 函数名:getWaveData             *
 * 功  能:从音频文件中读取数据    *
 * 参  数:decoder 音频解码结构    *
 * 返回值:成功0，失败返回错误号   *
 * ****************************** */
int getWaveData(WaveDecoder *decoder)
{
	ssize_t ret;
	/* 1.根据偏移计算读取数据的实际位置 （待实现）*/
	/* 2.从文件中读取数据 */
	ret = read(decoder->fd, decoder->buffer, 
			decoder->bytesPerRead);
	if(-1 == ret)
		return WAVE_READERR;
	/* 3.设置实际读取长度值 */
	decoder->dataLength = ret;//ret为零时代表已经读取到文件尾
	return WAVE_SUCCESS;
}
/* ****************************** *
 * 函数名:destroyWaveDecoder      *
 * 功  能:关闭文件、释放内存      *
 * 参  数:decoder 音频解码结构    *
 * 返回值:成功0，失败返回错误号   *
 * ****************************** */
int destroyWaveDecoder(WaveDecoder *decoder)
{
	/* 1.释放缓存 */
	if(NULL != decoder->buffer)
		free(decoder->buffer);
	/* 2.关闭文件 */
	close(decoder->fd);
	/* 3.清空解码结构 */
	bzero(decoder, sizeof(WaveDecoder));
	/* 4.设置文件描述符为非打开状态 */
	decoder->fd = -1;
	return WAVE_SUCCESS;
}

