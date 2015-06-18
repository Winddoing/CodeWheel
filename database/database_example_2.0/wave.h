#ifndef WAVE_H
#define WAVE_H

#include <stdint.h>
#include <sys/types.h>

typedef struct {
	const char *fileName;
	int fd;
	struct{
		int rate;
		int channels;
		int bits;
	}args;
	uint32_t bytesPerRead;//每次要读取的数据长度
	uint32_t dataLength;//实际读取的数据长度
	uint8_t * buffer;//读到数据的存放位置
	off_t offset;//开始读取的偏移地址
}WaveDecoder;

#define WAVE_SUCCESS	0
#define WAVE_SPECERR	1
#define WAVE_OPENERR	2
#define WAVE_BADFILE	3
#define WAVE_INVTYPE	4
#define WAVE_READERR	5
#define WAVE_NOMEMOR	6
/* ****************************** *
 * 函数名:initWaveDecoder         *
 * 功  能:初始化并读取音步头信息  *
 * 参  数:fileName 音频文件名     *
 *        decoder 音频解码结构    *
 * 返回值:成功0，失败返回错误号   *
 * ****************************** */
int initWaveDecoder(const char *fileName, WaveDecoder* decoder);
/* ****************************** *
 * 函数名:getWaveData             *
 * 功  能:从音频文件中读取数据    *
 * 参  数:decoder 音频解码结构    *
 * 返回值:成功0，失败返回错误号   *
 * ****************************** */
int getWaveData(WaveDecoder *decoder);
/* ****************************** *
 * 函数名:destroyWaveDecoder      *
 * 功  能:关闭文件、释放内存      *
 * 参  数:decoder 音频解码结构    *
 * 返回值:成功0，失败返回错误号   *
 * ****************************** */
int destroyWaveDecoder(WaveDecoder *decoder);

#endif // WAVE_H

