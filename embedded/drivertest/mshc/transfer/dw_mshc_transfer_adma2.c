#include <stdio.h>
#include <mshc/dw_mshc.h>

#include <dma_default.h>

#define MAX_ADMA2_DESC 6
#define ADMA2_DESC_CNT 3

unsigned int do_wait_adma2_transfer()
{
	unsigned int timeout = 1000000;

	printf("===>:%s, %d\n", __func__, __LINE__);
	while(do_wait_xfer_complete() &&\
		  do_wait_dma_interrupt() && --timeout) {
		udelay(10);
	}

	if(!timeout)
		goto timeout;

	if(!do_wait_dma_interrupt())
		goto clear;
	mshc_normal_int_xfer_complete();

	return 0;
timeout:
	printf("===>:%s, ADMA2 transfer timeout!!!\n", __func__);
	dump_error_status();

	return -1;

clear:
	printf("===>:%s, ADMA2 transfer fail!!!\n", __func__);
	dump_error_status();
	mshc_err_int_adma_err_clear();

	return -1;
}

unsigned int do_wait_adma2_transfer_s()
{
	unsigned int timeout = 2000000;

	printf("===>:%s, %d\n", __func__, __LINE__);
	while(do_wait_xfer_complete_s() &&\
		  do_wait_dma_interrupt_s() && --timeout) {
		udelay(10);
	}

	if(!timeout)
		goto timeout;

	if(!do_wait_dma_interrupt_s())
		goto clear;
	mshc_normal_int_xfer_complete_s();

	return 0;
timeout:
	printf("===>:%s, ADMA2 transfer timeout!!!\n", __func__);
	dump_error_status();

	return -1;

clear:
	printf("===>:%s, ADMA2 transfer fail!!!\n", __func__);
	dump_error_status();
	mshc_err_int_adma_err_clear_s();

	return -1;
}

unsigned int mshc_adma2_read(unsigned int cardaddr, unsigned int blksize, unsigned int blkcnt)
{
	unsigned int err;

	mshc_cmd_send(MMC_SET_BLOCKLEN, blksize, 0, MSHC_CMDAT_RESPONSE_R1);

	if(blkcnt == 1) {
		mshc_cmd_send(MMC_READ_SINGLE_BLOCK, cardaddr, MSHC_DATA_PRESENT_SEL_BIT, MSHC_CMDAT_RESPONSE_R1);
	} else {
		mshc_cmd_send(MMC_READ_MULTIPLE_BLOCK, cardaddr, MSHC_DATA_PRESENT_SEL_BIT, MSHC_CMDAT_RESPONSE_R1);
	}

#ifdef MSHC_USE_INT
	err = do_wait_adma2_transfer_s();
#else
	err = do_wait_adma2_transfer();
#endif

	return err;
}

unsigned int mshc_adma2_write(unsigned int cardaddr, unsigned int blksize, unsigned int blkcnt)
{
	unsigned int err;

	mshc_cmd_send(MMC_SET_BLOCKLEN, blksize, 0, MSHC_CMDAT_RESPONSE_R1);

	if(blkcnt == 1) {
		mshc_cmd_send(MMC_WRITE_BLOCK, cardaddr, MSHC_DATA_PRESENT_SEL_BIT, MSHC_CMDAT_RESPONSE_R1);
	} else {
		mshc_cmd_send(MMC_WRITE_MULTIPLE_BLOCK, cardaddr, MSHC_DATA_PRESENT_SEL_BIT, MSHC_CMDAT_RESPONSE_R1);
	}

#ifdef MSHC_USE_INT
	err = do_wait_adma2_transfer_s();
#else
	err = do_wait_adma2_transfer();
#endif

	return err;
}

void mshc_adma2_desc_table_add(unsigned int id, adma2_desc_t *desc, unsigned int *bufaddr, unsigned int len, unsigned int attr_high_3, unsigned int attr_low_3)
{
	unsigned int arg;
	unsigned int length;
	adma2_desc_t *tmp;

	tmp = desc;

	tmp[id].addr = bufaddr;
	length = (len & 0xffff) << 16;
	length |= ((len & 0x3ff0000) >> 16) << 6;
	tmp[id].attr.d32 = length | attr_high_3 | attr_low_3;

	printf("====>:%s, addr=0x%08x, attr=0x%08x\n", __func__, tmp[id].addr, tmp[id].attr.d32);
}

static unsigned int mshc_adma2_create_desc(unsigned int *bufaddr, unsigned int blksize, unsigned int blkcnt)
{
	static adma2_desc_t desc_tab[MAX_ADMA2_DESC];
	adma2_desc_t *desc_p;
	unsigned int length, i, offset;

	if ( 1 != blkcnt ) {
		if( blkcnt % ADMA2_DESC_CNT ) {
			printf("[Warning] func: %s, ADMA2 data transfer blkcnt %d aligned\n", __func__, ADMA2_DESC_CNT);
				return -11;
		}
	}
	desc_p = MY_UNCAC_ADDR(&desc_tab);
	memset(desc_p, 0, sizeof(adma2_desc_t)*MAX_ADMA2_DESC);

	offset = (blkcnt / ADMA2_DESC_CNT) * (blksize / 4);

	if(1 == blkcnt){
		length = blkcnt * blksize;
		mshc_adma2_desc_table_add(0, desc_p, virt_to_phys((void*)bufaddr), length, MSHC_ADMA2_DESC_TRAN,
				MSHC_ADMA2_DESC_VALID_END);
	} else {
		length = (blkcnt / ADMA2_DESC_CNT) * blksize;
		for(i=0; i<(ADMA2_DESC_CNT - 1); i++){
			mshc_adma2_desc_table_add(i, desc_p, virt_to_phys((void*)(bufaddr+i*offset)), length, MSHC_ADMA2_DESC_TRAN,
					MSHC_ADMA2_DESC_VALID);
		}
		mshc_adma2_desc_table_add(ADMA2_DESC_CNT - 1, desc_p, virt_to_phys((void*)(bufaddr+(ADMA2_DESC_CNT - 1)*offset)),
				length, MSHC_ADMA2_DESC_TRAN, MSHC_ADMA2_DESC_VALID_END);
	}
#if 0
	for(i=0; i<10; i++){
		printf("**_%d_**,addr=0x%08x, attr==0x%08x\n",i, (unsigned int*)desc_p+i, *((unsigned int*)desc_p+i));
	}
#endif
	return virt_to_phys((void *)desc_p);
}

unsigned int mshc_adma2_transfer(unsigned int *bufaddr, unsigned int cardaddr,
		unsigned int blksize, unsigned int blkcnt, unsigned int opsmode)
{
	unsigned int err;
	unsigned int addr;

	printf("===>>%s----adma2 %s data start!!!\n", __func__, (opsmode == MSHC_DATA_XFER_DIR_RD) ? "read" : "write");
	mshc_host1_dma_sel(MSHC_DMA_SEL_ADMA2);

	addr = mshc_adma2_create_desc(bufaddr, blksize, blkcnt);
	if ( -11 == addr )
		return -1;

	mshc_adma_sa_low_setup(addr);

	mshc_data_pre(opsmode, MSHC_DMA_ENABLE, blksize, blkcnt);

	//mshc_blksize_sdma_boundary_setup(MSHC_SDMA_BUF_BDARY_BYTES_4K);

	//mshc_host2_adma2_len_sel(MSHC_ADMA2_LEN_MODE_16);
	//mshc_host2_adma_addr_len_sel(MSHC_ADDRESSING_32);

	if(opsmode == MSHC_DATA_XFER_DIR_RD)
		err = mshc_adma2_read(cardaddr, blksize, blkcnt);
	else
		err = mshc_adma2_write(cardaddr, blksize, blkcnt);

	printf("===>>%s----adma2 %s data end!!!\n", __func__, (opsmode == MSHC_DATA_XFER_DIR_RD) ? "read" : "write");
	return err;
}
