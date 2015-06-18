#include <painter.h>
#include <string.h>


void drawPixmap(const FbInfo *info, const Pixmap *map)
{
	Size size;	
	uint16_t i;

	//检测格式
	if(info->args.format != map->sampleFormat)
		return;
	//检测图像边缘
	size.width = info->args.width > map->width ? 
			map->width : info->args.width;
	size.height = info->args.height > map->height ? 
			map->height : info->args.height;
	for(i = 0; i < size.height; i++)
	{
		memcpy((uint8_t*)(info->mapAddr) + (i * info->args.width) *
						map->sampleFormat / 8, 
				(uint8_t*)(map->sampleBuffer) + (i * map->width) * 
						map->sampleFormat / 8,
				size.width * map->sampleFormat / 8);
	}

	
}

