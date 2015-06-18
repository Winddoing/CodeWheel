#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include <linux/soundcard.h>

#include <snddevice.h>

/* ****************************** *
 * 函数名:openSndDevice           *
 * 功  能:打开音频设备            *
 * 参  数:devName 设备文件名   *
 *        decoder 音频解码结构    *
 * 返回值:成功0，失败返回错误号   *
 * ****************************** */
int openSndDevice(const char *devName, SndPlayer *player)
{
	if(NULL == devName)
		return SNDERR_SPECREE;
	player->devName = devName;
	player->fd = open(devName, O_WRONLY);

	return SNDERR_SUCCESS;
}
/* ****************************** *
 * 函数名:setSndDevice            *
 * 功  能:初始化音频设备          *
 * 参  数:devName 音频设备文件名  *
 * 返回值:成功0，失败返回错误号   *
 * ****************************** */
int setSndDevice(const SndPlayer *player)
{
	if(-1 == ioctl(player->fd, SNDCTL_DSP_RESET))
		return SNDERR_INVAARG;
	if(-1 == ioctl(player->fd, SNDCTL_DSP_SPEED, 
				&player->args.rate))
		return SNDERR_INVAARG;
	if(-1 == ioctl(player->fd, SNDCTL_DSP_SETFMT, 
				&player->args.bits))
		return SNDERR_INVAARG;
	if(-1 == ioctl(player->fd, SNDCTL_DSP_CHANNELS, 
				&player->args.channels))
		return SNDERR_INVAARG;
	return SNDERR_SUCCESS;
}
/* ****************************** *
 * 函数名:writeSndDevice          *
 * 功  能:写入音频数据到设备      *
 * 参  数:devName 音频设备文件名  *
 * 返回值:成功0，失败返回错误号   *
 * ****************************** */
int writeSndDevice(const SndPlayer *player)
{
	ssize_t wRet;
	
	if(-1 == (wRet = write(player->fd, player->buffer,
				   	player->length)))
		return SNDERR_INVAARG;
	return SNDERR_SUCCESS;
}
/* ****************************** *
 * 函数名:closeSndDevice          *
 * 功  能:关闭音频设备            *
 * 参  数:devName 音频设备文件名  *
 * 返回值:成功0，失败返回错误号   *
 * ****************************** */
int closeSndDevice(SndPlayer *player)
{
	close(player->fd);

	return SNDERR_SUCCESS;
}

