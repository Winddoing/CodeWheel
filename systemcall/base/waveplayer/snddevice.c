#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/soundcard.h>


#include <snddevice.h>

int openSndDevice(const char *devName, SndPlayer *player)
{
	bzero(player, sizeof(SndPlayer));

	if(NULL == devName) devName = DEF_DEVICE_NAME;
	player->devName = devName;

	player->fd = open(devName, O_WRONLY);
	if(-1 == player->fd)
	{
		player->error = SNDERR_OPENDEV;
		return SNDERR_OPENDEV;
	}
	return SNDERR_SUCCESS;
}
int setSndDevice(SndPlayer *player)
{
	if(-1 == ioctl(player->fd, SNDCTL_DSP_RESET))
		goto setSndDeviceError;
	if(-1 == ioctl(player->fd, SNDCTL_DSP_SPEED, 
							&player->args.rate))
		goto setSndDeviceError;
	if(-1 == ioctl(player->fd, SNDCTL_DSP_SETFMT, 
							&player->args.bits))
		goto setSndDeviceError;
	if(-1 == ioctl(player->fd, SNDCTL_DSP_CHANNELS, 
							&player->args.channels))
		goto setSndDeviceError;
	return SNDERR_SUCCESS;
setSndDeviceError:
	player->error = SNDERR_INVAARG;
	return SNDERR_INVAARG;
}
int writeSndDevice(SndPlayer *player)
{
	ssize_t ret;

	ret = write(player->fd, player->buffer, player->length);
	if(-1 == ret)
	{
		player->error = SNDERR_WRITEDA;
		return SNDERR_WRITEDA;
	}
	return SNDERR_SUCCESS;
}
int closeSndDevice(SndPlayer *player)
{
	close(player->fd);
	bzero(player, sizeof(SndPlayer));

	return SNDERR_SUCCESS;
}
const char *sndErrorString(const SndPlayer *player)
{
	static const char *ErrorStrings[] =
	{
		[SNDERR_OPENDEV - 10] = "打开设备失败",
		[SNDERR_INVAARG	- 10] = "设置设备工作参数失败",
		[SNDERR_SUCCESS] = "成功",
		[SNDERR_WRITEDA - 10] = "写入音频数据失败",
	};
	int t = player->error - 10;
	if(t < 0) t = 0;
	return ErrorStrings[t];
}


