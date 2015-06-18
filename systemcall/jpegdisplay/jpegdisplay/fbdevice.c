#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <linux/fb.h>

#include <fbdevice.h>

const char * fbErrorString(const FbInfo *info)
{
	static const char *errorStrings[] = 
	{
		[ERROR_FBSUCCESS] = "Frame buffer device operator success.",
		[ERROR_FBOPEN] = "Cannot open frame buffer device.",
		[ERROR_FBIOCTL] = "Cannot get frame buffer device arguement.",
		[ERROR_FBMMAP] = "Cannot map frame buffer device.",
	};
	return errorStrings[info->errNumber];
}
int initFbDevice(const char *deviceName, FbInfo *info)
{
	struct fb_var_screeninfo screenInfo;

	bzero(info, sizeof(FbInfo));
	info->fd = -1;
	info->mapAddr = ERRADDR;
	info->deviceName = 
			(NULL == deviceName) ? 
			DEF_DEVICENAME : deviceName;
	info->fd = open(info->deviceName, O_RDWR);
	if(-1 == info->fd)
	{
		info->errNumber = ERROR_FBOPEN;
		goto ErrorInitFbDevice;
	}
	if(-1 == ioctl(info->fd, FBIOGET_VSCREENINFO, 
			&screenInfo)) 
	{
		info->errNumber = ERROR_FBIOCTL;
		goto ErrorInitFbDevice;
	}
	info->args.width = screenInfo.xres;
	info->args.height = screenInfo.yres;
	info->args.format = screenInfo.bits_per_pixel;
	info->mapSize = info->args.width *
			info->args.height *
			info->args.format / 8;
	info->mapAddr = mmap(NULL, info->mapSize,
					PROT_READ | PROT_WRITE,
					MAP_SHARED, info->fd, 0);
	if(ERRADDR == info->mapAddr)
	{
		info->errNumber = ERROR_FBMMAP;
		goto ErrorInitFbDevice;
	}
	return ERROR_FBSUCCESS;
ErrorInitFbDevice:
	perror("InitFbDevice");
	if(-1 != info->fd)
	{
		close(info->fd);
		info->fd = -1;
	}
	return info->errNumber;
}
void destoryFbDevice(FbInfo *info)
{
	if(NULL == info) return;
	if(ERRADDR != info->mapAddr)
	{
		munmap(info->mapAddr, info->mapSize);
		info->mapAddr = (void *)-1;
	}
	if(-1 != info->fd)
	{
		close(info->fd);
		info->fd = -1;
	}
}

