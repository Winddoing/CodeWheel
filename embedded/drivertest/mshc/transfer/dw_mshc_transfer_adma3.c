#include <stdio.h>
#include <mshc/dw_mshc.h>

#include <dma_default.h>

#define MAX_INTEGRATED_DESC_CNT  16
#define MAX_ADMA3_DESC_CNT		 32

static void pre_cmd_desc(adma3_conf_info adma3_conf)
{
	unsigned int b_count, b_size, cmd_mode;

	b_count = 0;  //block count 16bit disable and set 32bit

	b_size = adma3_conf.block_size; //tarnsfer size
	b_size |= MSHC_SDMA_BUF_BDARY_BYTES_128K; //SDMA buffer boundary  128k byte
	b_size |= b_count << BLKCNT_HIGH_16;

	cmd_mode = MSHC_BLOCK_COUNT_ENABLE_BIT; //enable block_count
	if(1 == adma3_conf.block_count)
		cmd_mode &= ~MSHC_MULTI_BLK_SEL_BIT;
	else
		cmd_mode |= MSHC_MULTI_BLK_SEL_BIT;

	cmd_mode |= MSHC_AUTO_CMD12_ENABLE;
	switch(adma3_conf.is_rw){
		case MSHC_DATA_XFER_DIR_WR:
			cmd_mode &= ~MSHC_DATA_XFER_DIR_BIT;
			cmd_mode |= MSHC_DMA_ENABLE_BIT;  // enable DMA
			break;
		case MSHC_DATA_XFER_DIR_RD:
			cmd_mode |= MSHC_DATA_XFER_DIR_BIT;
			cmd_mode |= MSHC_DMA_ENABLE_BIT;  // enable DMA
			break;
		default:
			cmd_mode &= ~MSHC_DATA_XFER_DIR_BIT;
			break;
	}

	cmd_mode |= adma3_conf.index << (CMD_HIGH_16 + MSHC_CMD_INDEX_LBIT);  //8+16
	if(adma3_conf.no_data == CMD_DATA){
		cmd_mode |= MSHC_DATA_PRESENT_SEL_BIT << CMD_HIGH_16; //data present
		adma3_conf.desc_p[adma3_conf.id].one_cmd_desc.cmd_tran_mode.attr.d32 = CMD_HAVE_DATA;
	}else{
		cmd_mode &= ~(MSHC_DATA_PRESENT_SEL_BIT << CMD_HIGH_16); //no data present
		adma3_conf.desc_p[adma3_conf.id].one_cmd_desc.cmd_tran_mode.attr.d32 = CMD_NOT_HAVE_DATA;
	}

	cmd_mode |= MSHC_CMD_CRC_CHK_ENABLE_BIT << CMD_HIGH_16;
	cmd_mode |= MSHC_CMD_IDX_CHK_ENABLE_BIT << CMD_HIGH_16;
	cmd_mode |= MSHC_DATA_PRESENT_SEL_BIT << CMD_HIGH_16;
	cmd_mode |= MSHC_RESP_TYPE_SELECT_RESP_LEN_48 << CMD_HIGH_16; //resp 10 --- 48bit

	adma3_conf.desc_p[adma3_conf.id].one_cmd_desc.block_count.reg = adma3_conf.block_count;
	adma3_conf.desc_p[adma3_conf.id].one_cmd_desc.block_count.attr.d32 = CMD_HAVE_DATA;
	adma3_conf.desc_p[adma3_conf.id].one_cmd_desc.block_size.reg = b_size;
	adma3_conf.desc_p[adma3_conf.id].one_cmd_desc.block_size.attr.d32 = CMD_HAVE_DATA;
	adma3_conf.desc_p[adma3_conf.id].one_cmd_desc.argument.reg = adma3_conf.argument;
	adma3_conf.desc_p[adma3_conf.id].one_cmd_desc.argument.attr.d32 = CMD_HAVE_DATA;
	adma3_conf.desc_p[adma3_conf.id].one_cmd_desc.cmd_tran_mode.reg = cmd_mode;
}

static void pre_data_desc(adma3_conf_info adma3_conf)
{
	unsigned int block_count, length, attr;
	block_count = adma3_conf.block_count;

	length = ((block_count*adma3_conf.block_size) & 0xffff) << 16;     //length low 16bit
	length |= (((block_count*adma3_conf.block_size) & 0x3ff0000) >> 16) << 6; //length upper 10bit

	attr = adma3_conf.adma2_attr_IVE;
	attr |= adma3_conf.adma2_attr_act;

	switch(adma3_conf.is_rw){
		case MSHC_DATA_XFER_DIR_WR:
			adma3_conf.desc_p[adma3_conf.id].one_data_desc.addr = adma3_conf.buf;
			adma3_conf.desc_p[adma3_conf.id].one_data_desc.attr.d32 = length | attr;
			break;
		case MSHC_DATA_XFER_DIR_RD:
			adma3_conf.desc_p[adma3_conf.id].one_data_desc.addr = adma3_conf.buf;
			adma3_conf.desc_p[adma3_conf.id].one_data_desc.attr.d32 = length | attr;
			break;
	}
}

static void one_adma3_desc(adma3_integrated_desc_t *integrated_desc_p, adma3_conf_info adma3_conf)
{
	pre_cmd_desc(adma3_conf);

	if(adma3_conf.no_data)
		pre_data_desc(adma3_conf);

	integrated_desc_p[adma3_conf.id].one_desc_addr = virt_to_phys((void *)&adma3_conf.desc_p[adma3_conf.id]);

	if(adma3_conf.is_end){
		integrated_desc_p[adma3_conf.id].attr.d32 = DATA_END;
	}else{
		integrated_desc_p[adma3_conf.id].attr.d32 = DATA_TRAN;
	}
}

static void create_one_adma3_desc(int id, adma3_integrated_desc_t *integrated_desc_p, adma3_desc_t *desc, unsigned int is_rw,
		unsigned int no_data, unsigned int *buf, unsigned int block_count, unsigned int block_size, unsigned int arg,
		unsigned int adma2_attr_act, unsigned int adma2_attr_IVE, int is_end)
{
	adma3_conf_info adma3_conf;

	switch(is_rw){
		case MSHC_DATA_XFER_DIR_RD:
			if(1 == block_count)
				adma3_conf.index = MMC_READ_SINGLE_BLOCK;
			else
				adma3_conf.index = MMC_READ_MULTIPLE_BLOCK;
			break;
		case MSHC_DATA_XFER_DIR_WR:
			if(1 == block_count)
				adma3_conf.index = MMC_WRITE_BLOCK;
			else
				adma3_conf.index = MMC_WRITE_MULTIPLE_BLOCK;
			break;
	}

	adma3_conf.id = id;
	adma3_conf.desc_p = desc;
	adma3_conf.is_rw = is_rw;
	adma3_conf.no_data = no_data;
	adma3_conf.buf = virt_to_phys((void*)buf);
	adma3_conf.argument = arg;
	adma3_conf.block_count = block_count;
	adma3_conf.block_size = block_size;
	adma3_conf.adma2_attr_act = adma2_attr_act;
	adma3_conf.adma2_attr_IVE = adma2_attr_IVE;
	adma3_conf.is_end = is_end;

	one_adma3_desc(integrated_desc_p, adma3_conf);
}

static unsigned int mshc_adma3_create_desc(unsigned int *bufaddr_w, unsigned int bufaddr_r, unsigned int cardaddr,
		unsigned int blksize, unsigned int blkcnt)
{
	adma3_integrated_desc_t integrated_desc[MAX_INTEGRATED_DESC_CNT];
	adma3_desc_t desc[MAX_ADMA3_DESC_CNT];
	adma3_integrated_desc_t *integrated_desc_p;
	adma3_desc_t *desc_p;

	desc_p = (adma3_desc_t *)_ALIGH((unsigned int)desc | 0xa0000000, 64);
	integrated_desc_p =(adma3_integrated_desc_t *)_ALIGH((unsigned int)integrated_desc | 0xa0000000, 64);

	memset(integrated_desc_p, 0, sizeof(adma3_desc_t)*MAX_INTEGRATED_DESC_CNT);
	memset(desc_p, 0, sizeof(adma3_desc_t)*MAX_ADMA3_DESC_CNT);

	create_one_adma3_desc(0, integrated_desc_p, desc_p, MSHC_DATA_XFER_DIR_WR, CMD_DATA,
			bufaddr_w, blkcnt, blksize,  cardaddr,
			MSHC_ADMA2_DESC_TRAN, MSHC_ADMA2_DESC_VALID_END, ADMA3_NO_END);

	create_one_adma3_desc(1, integrated_desc_p, desc_p, MSHC_DATA_XFER_DIR_RD, CMD_DATA,
			bufaddr_r, blkcnt, blksize, cardaddr,
			MSHC_ADMA2_DESC_TRAN, MSHC_ADMA2_DESC_VALID_END, ADMA3_END);
#ifdef DEBUG
	dump_adma3_desc(integrated_desc_p);
#endif

	return virt_to_phys((void*)integrated_desc_p);
}

static unsigned int do_wait_adma3_transfer()
{
	unsigned int timeout = 2000000;

	if(cmd_err_stat_check())
		goto err;

	printf("===>:%s, %d\n", __func__, __LINE__);
	while(do_wait_xfer_complete() &&\
		  do_wait_dma_interrupt() && --timeout) {
		udelay(10);
	}

	if(!timeout)
		goto timeout;

	if(!do_wait_dma_interrupt())
		goto err;
	mshc_normal_int_xfer_complete();

	return 0;
timeout:
	printf("===>:%s, ADMA3 transfer timeout!!!\n", __func__);
	dump_error_status();

	return -1;
err:
	printf("===>:%s, ADMA3 transfer fail!!!\n", __func__);
	dump_error_status();
	mshc_normal_int_xfer_complete();
	mshc_err_int_adma_err_clear();

	return -1;
}

static unsigned int do_wait_adma3_transfer_s()
{
	unsigned int timeout = 2000000;

	if(cmd_err_stat_check_s())
		goto err;

	printf("===>:%s, %d\n", __func__, __LINE__);
	while(do_wait_xfer_complete_s() &&\
		  do_wait_dma_interrupt_s() && --timeout) {
		udelay(10);
	}

	if(!timeout)
		goto timeout;

	if(!do_wait_dma_interrupt_s())
		goto err;
	mshc_normal_int_xfer_complete_s();

	return 0;
timeout:
	printf("===>:%s, ADMA3 transfer timeout!!!\n", __func__);
	dump_error_status();

	return -1;
err:
	printf("===>:%s, ADMA3 transfer fail!!!\n");
	dump_error_status();
	mshc_normal_int_xfer_complete_s();
	mshc_err_int_adma_err_clear_s();

	return -1;
}

unsigned int mshc_adma3_transfer(unsigned int *bufaddr_w, unsigned int *bufaddr_r,
		unsigned int cardaddr, unsigned int blksize, unsigned int blkcnt)
{
	unsigned int err;
	unsigned int addr;

	printf("\n===>>%s----adma3 data transfer start!!!\n", __func__);
	mshc_host1_dma_sel(MSHC_DMA_SEL_ADMA2_3);

	mshc_host2_adma2_len_sel(MSHC_ADMA2_LEN_MODE_26);
	//mshc_host2_adma_addr_len_sel(MSHC_ADDRESSING_32);

	addr = mshc_adma3_create_desc(bufaddr_w, bufaddr_r, cardaddr, blksize, blkcnt);
	mshc_adma_id_low_setup(addr);
#ifdef DEBUG
	dump_adma3_tran();
	dump_cmd_cfg();
	dump_response();
	dump_error_status();
#endif

#ifdef MSHC_USE_INT
	err = do_wait_adma3_transfer_s();
#else
	err = do_wait_adma3_transfer();
#endif

	printf("===>>%s----adma3 data transfer start!!!\n\n", __func__);
	return err;
}
