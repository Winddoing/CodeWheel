#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#define WAV_HEAD_BYTES	(44)
#define FFT_DURATION_N	(16384)

int threshold_ENERGY_DB = 50;

double snd_energy_calc(double *data, unsigned int size)
{
	double temp;
	int i0;

	temp = 0;

	for (i0 = 0; i0 < size; i0++)
		temp += pow(data[i0], 2);

	temp = temp/size;

	return 10.0 * log10(temp/1000);
}

int snd_energy_calc_raw16(short *data_in, int nfft, int fs, int channels, int counts)
{
	int i,j;
	int cn;
	double *data;
	int ret = 0;
	double *energy_1k;

	data = (double *)malloc(nfft * sizeof(double));
	if (!data) {
		fprintf(stderr, "%s: failed to alloc buffer.\n", __func__);
		return -1;
	}

	energy_1k = (double *)malloc(channels * sizeof(double));
	if (!energy_1k) {
		fprintf(stderr, "%s: failed to alloc energy_1k.\n", __func__);
		free(data);
		return -1;
	}

	for(cn = 0; cn < counts; cn++) {
		for (i = 0; i < nfft; i++)
			data[i] = (double)data_in[i * channels + cn];
		energy_1k[cn] = snd_energy_calc(data, nfft);
		printf("channel[%d] energy: %lf\n",cn, energy_1k[cn]);
	}

	for(cn = 0; cn < counts; cn++) {
		if (energy_1k[cn] > (double)threshold_ENERGY_DB) {
			printf("channel[%d] energy %lf greater than threshold_ENERGY_DB %d\n",
					cn, energy_1k[cn], threshold_ENERGY_DB);
			ret = -1;
			goto end;
		}

		for(j=cn-1; j>=0; j--) {
			if(energy_1k[cn] == energy_1k[j]) {
				printf("channel[%d] or channel[%d] mic is abnormal!!!\n",cn,j);
				ret = -1;
				goto end;
			}
		}
	}

end:
	free(data);
	free(energy_1k);
	return ret;
}

int main(int argc, char *argv[])
{
	int speed, channel;
	short * sbuff;
	int ret = 0;
	int skip_ms = 0;
	int skip_bytes = 0;
	int test_channel_count = 0;
	FILE* fd;

	fd = fopen(argv[1], "rb+");
	if (fd < 0) {
		fprintf(stderr, "%s: cannot open file %s\n", __func__, argv[1]);
	}

	speed = atoi(argv[2]);
	channel = atoi(argv[3]);
	test_channel_count = atoi(argv[3]);
	if (argc > 4)
		threshold_ENERGY_DB = atoi(argv[4]);
	if (argc > 5)
		skip_ms = atoi(argv[5]);

	skip_bytes = (speed * sizeof(short) * channel / 1000) * skip_ms + WAV_HEAD_BYTES;
	printf ("\n\tskip head %d\n\tthreshold_ENERGY_DB %d\n",
			skip_bytes, threshold_ENERGY_DB);

	sbuff = (short *)malloc(sizeof(short) * FFT_DURATION_N * channel);
	memset(sbuff, 0, sizeof(short) * FFT_DURATION_N * channel);

	ret = fseek(fd, skip_bytes, SEEK_SET);
	if (ret) {
		printf("fseek err!! ret = %d\n", ret);
		return -2;
	}

	fread(sbuff, sizeof(short), FFT_DURATION_N * channel, fd);
	ret = snd_energy_calc_raw16((short *)sbuff, FFT_DURATION_N, speed, channel, test_channel_count);
	if(ret==-1){
		free(sbuff);
		return -1;
	}

	free(sbuff);
	return 0;
}
