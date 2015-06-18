#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <wave.h>
#include <snddevice.h>

int main(int argc, char *argv[])
{
	WaveDecoder decoder;
	SndPlayer player;
	int i, ret;
	
	if(argc < 2)
	{
		fprintf(stderr, "用法:%s WAVE音频列表\n",
						argv[0]);
		return EXIT_FAILURE;
	}
	
	bzero(&decoder, sizeof(decoder));
	ret = openSndDevice(NULL, &player);
	if(0 != ret) goto Error;
	
	for(i = 1; i < argc; i++)
	{
		fprintf(stdout, "play file: %s\n", argv[i]);
		ret = initWaveDecoder(argv[i], &decoder);
		if(0 != ret) continue;
		player.args.rate = decoder.args.rate;
		player.args.bits = decoder.args.bits;
		player.args.channels = decoder.args.channels;
		player.buffer = decoder.buffer;
		ret = setSndDevice(&player);
		if(0 != ret)
		{
			player.error = SNDERR_INVAARG;	
			goto Error;
		}
		while(1)
		{
			ret = getWaveData(&decoder);
			if(0 != ret) break;//读取音频失败
			/* 音频播放完成 */
			if(0 == decoder.dataLength) break;
			/* 用实际读取数据长度，设置本次播放长度 */
			player.length = decoder.dataLength;
			/* 播放音频数据 */
			ret = writeSndDevice(&player);
			/* 写数据到音频设备失败 */
			if(0 != ret) goto Error;
		}
		destroyWaveDecoder(&decoder);
	}
	closeSndDevice(&player);
	
	return EXIT_SUCCESS;
Error:
	if(player.error != 0)
		fprintf(stderr, sndErrorString(&player));
	perror(": Waveplay");
	destroyWaveDecoder(&decoder);
	closeSndDevice(&player);

	return ret;
}

