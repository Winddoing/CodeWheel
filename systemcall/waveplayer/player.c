#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <wave.h>
#include <snddevice.h>

int main(int argc, char *argv[])
{
	WaveDecoder wave;
	SndPlayer player;
	int i;

	if(0 != openSndDevice(DEF_DEVICE_NAME, &player))
		goto Error;
	for(i = 1; i < argc; i++)
	{
		if(0 != initWaveDecoder(argv[i], &wave))
			goto Error;
		player.args.rate = wave.args.rate;
		player.args.channels = wave.args.channels;
		player.args.bits = wave.args.bits;
		player.buffer = wave.buffer;
		if(0 != setSndDevice(&player))
			goto Error;
		while(1)
		{
			if(0 != getWaveData(&wave))
				goto Error;
			if(0 == wave.dataLength)
				break;
			player.length = wave.dataLength;
			if(0 != writeSndDevice(&player))
				goto Error;
		}
		destroyWaveDecoder(&wave);
	}
	closeSndDevice(&player);
	
	return EXIT_SUCCESS;

Error:
	perror("WavePlayer");
	return -errno;
	destroyWaveDecoder(&wave);
	closeSndDevice(&player);
}

