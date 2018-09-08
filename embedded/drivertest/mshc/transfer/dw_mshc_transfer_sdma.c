#include <stdio.h>
#include <mshc/dw_mshc.h>

unsigned int do_wait_sdma_transfer()
{
	unsigned int err;
	unsigned int timeout = 1000000;

/*	dump_error_status();*/
next:
	printf("===>%s, DMA one transfer SDMA_BUF_BDARY=0x%x\n", __func__,
			(mshc_readw(MSHC_BASE, MSHC_BLOCKSIZE_R) & MSHC_SDMA_BUF_BDARY_MASK) >> MSHC_SDMA_BUF_BDARY_LBIT);
	while(do_wait_dma_interrupt() && \
		  do_wait_xfer_complete() && --timeout)
	{
		udelay(10);
	}
	if(!timeout)
		goto clear;

	if(!do_wait_dma_interrupt()){
		mshc_normal_int_dma_interrupt_generated();
		mshc_sdma_sa_setup(mshc_readl(MSHC_BASE, MSHC_SDMASA_R));
		goto next;
	}

	mshc_normal_int_xfer_complete();
	mshc_normal_int_dma_interrupt_generated();

	return 0;

clear:
	printf("[ERROR]: Wait xfer complete timeout.MSHC_ERROR_INT_STAT_R=%08x\n", mshc_readw(MSHC_BASE, MSHC_ERROR_INT_STAT_R));
	dump_error_status();
	mshc_normal_int_xfer_complete_s();
	mshc_normal_int_dma_interrupt_generated_s();

	return -1;
}

unsigned int do_wait_sdma_transfer_s()
{
	unsigned int err;
	unsigned int timeout = 100000;

next:
	printf("===>%s, DMA one transfer SDMA_BUF_BDARY=0x%x\n", __func__,
			(mshc_readw(MSHC_BASE, MSHC_BLOCKSIZE_R) & MSHC_SDMA_BUF_BDARY_MASK) >> MSHC_SDMA_BUF_BDARY_LBIT);
	while(do_wait_dma_interrupt_s() && \
		  do_wait_xfer_complete_s() && --timeout)
	{
		udelay(10);
	}
	if(!timeout)
		goto clear;

	if(!do_wait_dma_interrupt_s()){
		mshc_normal_int_dma_interrupt_generated_s();
		mshc_sdma_sa_setup(mshc_readl(MSHC_BASE, MSHC_SDMASA_R));
		goto next;
	}

	mshc_normal_int_xfer_complete_s();
	mshc_normal_int_dma_interrupt_generated_s();

	return 0;

clear:
	printf("[ERROR]: Wait xfer complete timeout. mshc_error_int_stat_s =%08x\n", mshc_error_int_stat_s);
	dump_error_status();
	mshc_normal_int_xfer_complete_s();
	mshc_normal_int_dma_interrupt_generated_s();

	return -1;
}


unsigned int mshc_sdma_read(unsigned int cardaddr, unsigned int blksize, unsigned int blkcnt)
{
	unsigned int err;

	mshc_cmd_send(MMC_SET_BLOCKLEN, blksize, 0, MSHC_CMDAT_RESPONSE_R1);

	if(blkcnt == 1) {
		mshc_cmd_send(MMC_READ_SINGLE_BLOCK, cardaddr, MSHC_DATA_PRESENT_SEL_BIT, MSHC_CMDAT_RESPONSE_R1);
	} else {
		mshc_cmd_send(MMC_READ_MULTIPLE_BLOCK, cardaddr, MSHC_DATA_PRESENT_SEL_BIT, MSHC_CMDAT_RESPONSE_R1);
	}

#ifdef MSHC_USE_INT
	err = do_wait_sdma_transfer_s();
#else
	err = do_wait_sdma_transfer();
#endif

	return err;
}

unsigned int mshc_sdma_write(unsigned int cardaddr, unsigned int blksize, unsigned int blkcnt)
{
	unsigned int err;

	mshc_cmd_send(MMC_SET_BLOCKLEN, blksize, 0, MSHC_CMDAT_RESPONSE_R1);

	if(blkcnt == 1) {
		mshc_cmd_send(MMC_WRITE_BLOCK, cardaddr, MSHC_DATA_PRESENT_SEL_BIT, MSHC_CMDAT_RESPONSE_R1);
	} else {
		mshc_cmd_send(MMC_WRITE_MULTIPLE_BLOCK, cardaddr, MSHC_DATA_PRESENT_SEL_BIT, MSHC_CMDAT_RESPONSE_R1);
	}

#ifdef MSHC_USE_INT
	err = do_wait_sdma_transfer_s();
#else
	err = do_wait_sdma_transfer();
#endif

	return err;
}

unsigned int mshc_sdma_transfer(unsigned int *bufaddr, unsigned int cardaddr,
		unsigned int blksize, unsigned int blkcnt, unsigned int opsmode)
{
	unsigned int err;
	unsigned int adr;

	printf("===>>%s----sdma %s data start!!!\n", __func__, (opsmode == MSHC_DATA_XFER_DIR_RD) ? "read" : "write");
	mshc_host1_dma_sel(MSHC_DMA_SEL_SDMA);

	if(mshc_readw(MSHC_BASE, MSHC_HOST_CTRL2_R) & MSHC_HOST_VER4_ENABLE_BIT)
		mshc_adma_sa_low_setup(bufaddr);
	else
		mshc_sdma_sa_setup(bufaddr);

	mshc_data_pre(opsmode, MSHC_DMA_ENABLE, blksize, blkcnt);

	mshc_blksize_sdma_boundary_setup(MSHC_SDMA_BUF_BDARY_BYTES_512K);

	if(opsmode == MSHC_DATA_XFER_DIR_RD)
		err = mshc_sdma_read(cardaddr, blksize, blkcnt);
	else
		err = mshc_sdma_write(cardaddr, blksize, blkcnt);

	printf("===>>%s----sdma %s data end!!!\n", __func__, (opsmode == MSHC_DATA_XFER_DIR_RD) ? "read" : "write");

	return err;
}
