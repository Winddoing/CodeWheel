#include <stdio.h>
#include <mshc/dw_mshc.h>
#include <mshc/dw_mshc_test.h>

#include <dma_default.h>

#define BLKSIZE		512
#define SIG_BLKCNT	1
#define MUL_BLKCNT	300  //blkcnt is a multiple of 3 only for adma2

#define PIO_CARD_ADDR	   70 // 100 * 512 BYTE  2G
#define SDMA_CARD_ADDR     30
#define ADMA2_CARD_ADDR    66
#define ADMA3_CARD_ADDR    10
#define CQE_CARD_ADDR	   40

#define BUF_SIZE_MAX	(MUL_BLKCNT * BLKSIZE / 4)

extern unsigned int rca;
extern unsigned int highcap;
unsigned int buf_w[BUF_SIZE_MAX];
unsigned int buf_r[BUF_SIZE_MAX];
unsigned int *buffer_r;
unsigned int *buffer_w;


void data_pre(unsigned int blkcnt)
{
	unsigned int i;

	buffer_r = MY_UNCAC_ADDR(&buf_r);
	buffer_w = MY_UNCAC_ADDR(&buf_w);

	memset(buffer_w, 0, sizeof(buf_w));
	memset(buffer_r, 0, sizeof(buf_r));

	for(i = 0; i < blkcnt * BLKSIZE / 4; i++){
#ifdef MSHC_RAND_DATA_SOURCE
		buffer_w[i] = rand();
#else
		buffer_w[i] = i;
#endif
	}

	buffer_w[0] = 0x55011555;
	buffer_w[1] = 0x66022666;
	buffer_w[2] = 0x77033777;
	buffer_w[3] = 0x88044888;
	buffer_w[4] = 0x99055999;
	buffer_w[5] = 0xabbbbbba;
	buffer_w[i-1] = 0x11100111;
	buffer_w[i-2] = 0x22200222;
	buffer_w[i-3] = 0x33300333;
	buffer_w[i-4] = 0x44400444;

	printf("--_%s_--buffer_r is A-addr=%08x\n", __func__, buffer_r);
	printf("--_%s_--buffer_w is A-addr=%08x\n", __func__, buffer_w);
}

int pio_transfer_sigblk_blk()
{
	unsigned int addr;
	unsigned int err;

	addr = mshc_card_addr_adjust(highcap, PIO_CARD_ADDR, BLKSIZE);

	data_pre(SIG_BLKCNT);

	err = mshc_pio_transfer(buffer_w, addr, BLKSIZE, SIG_BLKCNT, MSHC_DATA_XFER_DIR_WR);
	if(err) {
		printf("pio sig blk write err \n");
		return -1;
	}

	mdelay(100);

	err = mshc_pio_transfer(buffer_r, addr, BLKSIZE, SIG_BLKCNT, MSHC_DATA_XFER_DIR_RD);
	if(err) {
		printf("pio sig blk read err \n");
		return -1;
	}

	return check_WR_data(buffer_w, buffer_r, SIG_BLKCNT * BLKSIZE);
}

int pio_transfer_mulblk_blk()
{
	unsigned int addr;
	unsigned int err;

	addr = mshc_card_addr_adjust(highcap, PIO_CARD_ADDR, BLKSIZE);

	data_pre(MUL_BLKCNT);

	err = mshc_pio_transfer(buffer_w, addr, BLKSIZE, MUL_BLKCNT, MSHC_DATA_XFER_DIR_WR);
	if(err) {
		printf("pio mul blk write err \n");
		return -1;
	}

	mdelay(100);

	err = mshc_pio_transfer(buffer_r, addr, BLKSIZE, MUL_BLKCNT, MSHC_DATA_XFER_DIR_RD);
	if(err) {
		printf("pio mul blk read err \n");
		return -1;
	}

	return check_WR_data(buffer_w, buffer_r, MUL_BLKCNT * BLKSIZE);
}

int sdma_transfer_sigblk_blk()
{
	unsigned int addr;
	unsigned int err;

	addr = mshc_card_addr_adjust(highcap, SDMA_CARD_ADDR, BLKSIZE);

	data_pre(SIG_BLKCNT);

	err = mshc_sdma_transfer(buffer_w, addr, BLKSIZE, SIG_BLKCNT, MSHC_DATA_XFER_DIR_WR);
	if(err) {
		printf("sdma sig blk write err \n");
		return -1;
	}

	mdelay(100);

	err = mshc_sdma_transfer(buffer_r, addr, BLKSIZE, SIG_BLKCNT, MSHC_DATA_XFER_DIR_RD);
	if(err) {
		printf("sdma sig blk read err \n");
		return -1;
	}

	return check_WR_data(buffer_w, buffer_r, SIG_BLKCNT * BLKSIZE);
}

int sdma_transfer_mulblk_blk()
{
	unsigned int addr;
	unsigned int err;

	addr = mshc_card_addr_adjust(highcap, SDMA_CARD_ADDR, BLKSIZE);

	data_pre(MUL_BLKCNT);

	err = mshc_sdma_transfer(buffer_w, addr, BLKSIZE, MUL_BLKCNT, MSHC_DATA_XFER_DIR_WR);
	if(err) {
		printf("sdma mul blk write err \n");
		return -1;
	}

	mdelay(100);

	err = mshc_sdma_transfer(buffer_r, addr, BLKSIZE, MUL_BLKCNT, MSHC_DATA_XFER_DIR_RD);
	if(err) {
		printf("sdma mul blk read err \n");
		return -1;
	}

	return check_WR_data(buffer_w, buffer_r, MUL_BLKCNT * BLKSIZE);
}

int adma2_transfer_sigblk_blk()
{
	unsigned int addr;
	unsigned int err;

	addr = mshc_card_addr_adjust(highcap, ADMA2_CARD_ADDR, BLKSIZE);

	data_pre(SIG_BLKCNT);

	err = mshc_adma2_transfer(buffer_w, addr, BLKSIZE, SIG_BLKCNT, MSHC_DATA_XFER_DIR_WR);
	if(err) {
		printf("adma2 sig blk write err \n");
		return -1;
	}

	mdelay(100);

	err = mshc_adma2_transfer(buffer_r, addr, BLKSIZE, SIG_BLKCNT, MSHC_DATA_XFER_DIR_RD);
	if(err) {
		printf("adma2 sig blk read err \n");
		return -1;
	}

	return check_WR_data(buffer_w, buffer_r, SIG_BLKCNT * BLKSIZE);
}

int adma2_transfer_mulblk_blk()
{
	unsigned int addr;
	unsigned int err, i, offset;

	offset = (MUL_BLKCNT / 3) * (BLKSIZE / 4 );
	addr = mshc_card_addr_adjust(highcap, ADMA2_CARD_ADDR, BLKSIZE);

	data_pre(MUL_BLKCNT);

	err = mshc_adma2_transfer(buffer_w, addr, BLKSIZE, MUL_BLKCNT, MSHC_DATA_XFER_DIR_WR);
	if(err) {
		printf("sdma2 mul blk write err \n");
		return -1;
	}

	mdelay(1000);

	err = mshc_adma2_transfer(buffer_r, addr, BLKSIZE, MUL_BLKCNT, MSHC_DATA_XFER_DIR_RD);
	if(err) {
		printf("sdma2 mul blk read err \n");
		return -1;
	}


	for(i=0; i<3; i++){
		printf("buf_w=%08x, buf_r=%08x\n", buffer_w+i*offset, buffer_r+i*offset);
		if(!check_WR_data(buffer_w+i*offset, buffer_r+i*offset, BLKSIZE * (MUL_BLKCNT / 3))){
			printf("ADMA2 %d desc test ok....\n\n", i+1);
		}else{
			return -1;
		}
	}
	return 0;
}

int adma3_transfer_sigblk_blk()
{
	unsigned int addr;
	unsigned int err;

	addr = mshc_card_addr_adjust(highcap, ADMA3_CARD_ADDR, BLKSIZE);

	data_pre(SIG_BLKCNT);

	err = mshc_adma3_transfer(buffer_w, buffer_r, addr, BLKSIZE, SIG_BLKCNT);
	if(err) {
		printf("sdma sig blk transfer err \n");
		return -1;
	}

	return check_WR_data(buffer_w, buffer_r, SIG_BLKCNT * BLKSIZE);
}

int adma3_transfer_mulblk_blk()
{
	unsigned int addr;
	unsigned int err, i, offset;

	offset = (MUL_BLKCNT / 3) * (BLKSIZE / 4 );
	addr = mshc_card_addr_adjust(highcap, ADMA3_CARD_ADDR, BLKSIZE);

	data_pre(MUL_BLKCNT);

	err = mshc_adma3_transfer(buffer_w, buffer_r, addr, BLKSIZE, MUL_BLKCNT);
	if(err) {
		printf("sdma mul blk transfer err \n");
		return -1;
	}

	return check_WR_data(buffer_w, buffer_r, MUL_BLKCNT * BLKSIZE);
}

int cqe_transfer_sigblk_blk()
{
	unsigned int addr;
	unsigned int err;

	addr = mshc_card_addr_adjust(highcap, CQE_CARD_ADDR, BLKSIZE);

	data_pre(SIG_BLKCNT);

	err = mshc_cqe_transfer(buffer_w, buffer_r, rca, addr, BLKSIZE, SIG_BLKCNT);
	if(err) {
		printf("cqe sig blk transfer err \n");
		return -1;
	}

	return check_WR_data(buffer_w, buffer_r, SIG_BLKCNT * BLKSIZE);
}

int cqe_transfer_mulblk_blk()
{
	unsigned int addr;
	unsigned int err;

	addr = mshc_card_addr_adjust(highcap, CQE_CARD_ADDR, BLKSIZE);

	data_pre(MUL_BLKCNT);

	err = mshc_cqe_transfer(buffer_w, buffer_r, rca, addr, BLKSIZE, MUL_BLKCNT);
	if(err) {
		printf("cqe mul blk transfer err \n");
		return -1;
	}

	return check_WR_data(buffer_w, buffer_r, MUL_BLKCNT * BLKSIZE);
}

