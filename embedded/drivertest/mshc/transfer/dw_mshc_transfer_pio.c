#include <stdio.h>
#include <mshc/dw_mshc.h>

static unsigned int do_pio_read(unsigned int *addr, unsigned int blksize, unsigned int blkcnt)
{
	unsigned int nob, cnt;
	unsigned int val;

	printf("read data start..............\n");
	nob = blkcnt;

	for(; nob > 0; nob--) {
		cnt = blksize / 4;
#ifdef MSHC_USE_INT
		if(wait_buf_read_s())
#else
		if(wait_buf_read())
#endif
			return -1;

		while(cnt--) {
			*addr++ = mshc_buf_data_read();
		}
	}
#ifdef MSHC_USE_INT
	if(wait_xfer_complete_s())
#else
	if(wait_xfer_complete())
#endif
		return -1;

	printf("read data end..............\n");
	return 0;
}

static unsigned int do_pio_write(unsigned int *addr, unsigned int blksize, unsigned int blkcnt)
{
	unsigned int nob, cnt;
	unsigned int val;

	printf("write data start............\n");
	nob = blkcnt;

	for(; nob > 0; nob--) {
		cnt = blksize / 4;
#ifdef MSHC_USE_INT
		if(wait_buf_write_s())
#else
		if(wait_buf_write())
#endif
			return -1;

		while(cnt--) {
			mshc_buf_data_write(*addr++);
		}
	}
#ifdef MSHC_USE_INT
	if(wait_xfer_complete_s())
#else
	if(wait_xfer_complete())
#endif
		return -1;

	printf("write data end............\n");
	return 0;
}

unsigned int mshc_pio_read(unsigned int *bufaddr, unsigned int cardaddr, unsigned int blksize, unsigned int blkcnt)
{
	unsigned char *resp;
	unsigned int val;
	unsigned int err;

	printf("===========>>>... pio read\n");
	mshc_cmd_send(MMC_SET_BLOCKLEN, blksize, 0, MSHC_CMDAT_RESPONSE_R1);

	if(blkcnt == 1) {
		mshc_cmd_send(MMC_READ_SINGLE_BLOCK, cardaddr, MSHC_DATA_PRESENT_SEL_BIT, MSHC_CMDAT_RESPONSE_R1);
	} else {
		mshc_cmd_send(MMC_READ_MULTIPLE_BLOCK, cardaddr, MSHC_DATA_PRESENT_SEL_BIT, MSHC_CMDAT_RESPONSE_R1);
	}

	err = do_pio_read(bufaddr, blksize, blkcnt);

	//mshc_cmd_send(MMC_STOP_TRANSMISSION, 0, 0, MSHC_CMDAT_RESPONSE_R1b);
#ifdef MSHC_USE_INT
	if(xfer_err_stat_check_s())
#else
	if(xfer_err_stat_check())
#endif
		return -1;

	return err;
}

unsigned int mshc_pio_write(unsigned int bufaddr, unsigned int cardaddr, unsigned int blksize, unsigned int blkcnt)
{
	unsigned char *resp;
	unsigned int err;
	unsigned int val;

	printf("==========>>>... pio write\n");

	mshc_cmd_send(MMC_SET_BLOCKLEN, blksize, 0, MSHC_CMDAT_RESPONSE_R1);

	if(blkcnt == 1) {
		mshc_cmd_send(MMC_WRITE_BLOCK, cardaddr, MSHC_DATA_PRESENT_SEL_BIT, MSHC_CMDAT_RESPONSE_R1);
	} else {
		mshc_cmd_send(MMC_WRITE_MULTIPLE_BLOCK, cardaddr, MSHC_DATA_PRESENT_SEL_BIT, MSHC_CMDAT_RESPONSE_R1);
	}
	err = do_pio_write(bufaddr, blksize, blkcnt);

	//mshc_cmd_send(MMC_STOP_TRANSMISSION, 0, 0, MSHC_CMDAT_RESPONSE_R1b);
#ifdef MSHC_USE_INT
	if(xfer_err_stat_check_s())
#else
	if(xfer_err_stat_check())
#endif
		return -1;

	return err;
}

unsigned int mshc_pio_transfer(unsigned int *bufaddr, unsigned int cardaddr, unsigned int blksize, unsigned int blkcnt, unsigned int opsmode)
{
	unsigned int err;

	mshc_data_pre(opsmode, MSHC_DMA_DISABLE, blksize, blkcnt);
	/* transfer */
	if(opsmode == MSHC_DATA_XFER_DIR_RD)
		err = mshc_pio_read(bufaddr, cardaddr, blksize, blkcnt);
	else
		err = mshc_pio_write(bufaddr, cardaddr, blksize, blkcnt);

	return err;

}
