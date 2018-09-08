#include <stdio.h>
#include <mshc/dw_mshc.h>


#define CMD_ERR "mshc_resp_mask"

void mshc_sdma_sa_setup(unsigned int addr)
{
	mshc_writel(MSHC_BASE, addr, MSHC_SDMASA_R);
}

void mshc_blksize_setup(unsigned int blksize)
{
	unsigned int val;

	val = mshc_readw(MSHC_BASE, MSHC_BLOCKSIZE_R);
	val	&= ~MSHC_XFER_BLOCK_SIZE_MASK;
	val |= blksize << MSHC_XFER_BLOCK_SIZE_LBIT;
	mshc_writew(MSHC_BASE, val, MSHC_BLOCKSIZE_R);
}

void mshc_blksize_sdma_boundary_setup(unsigned int boundary)
{
	unsigned int val;

	val = mshc_readw(MSHC_BASE, MSHC_BLOCKSIZE_R);
	val &= ~MSHC_SDMA_BUF_BDARY_MASK;

	switch (boundary) {
	case MSHC_SDMA_BUF_BDARY_BYTES_4K:
		val |= MSHC_SDMA_BUF_BDARY_BYTES_4K;
		break;
	case MSHC_SDMA_BUF_BDARY_BYTES_8K:
		val |= MSHC_SDMA_BUF_BDARY_BYTES_8K;
		break;
	case MSHC_SDMA_BUF_BDARY_BYTES_16K:
		val |= MSHC_SDMA_BUF_BDARY_BYTES_16K;
		break;
	case MSHC_SDMA_BUF_BDARY_BYTES_32K:
		val |= MSHC_SDMA_BUF_BDARY_BYTES_32K;
		break;
	case MSHC_SDMA_BUF_BDARY_BYTES_64K:
		val |= MSHC_SDMA_BUF_BDARY_BYTES_64K;
		break;
	case MSHC_SDMA_BUF_BDARY_BYTES_128K:
		val |= MSHC_SDMA_BUF_BDARY_BYTES_128K;
		break;
	case MSHC_SDMA_BUF_BDARY_BYTES_256K:
		val |= MSHC_SDMA_BUF_BDARY_BYTES_256K;
		break;
	case MSHC_SDMA_BUF_BDARY_BYTES_512K:
		val |= MSHC_SDMA_BUF_BDARY_BYTES_512K;
		break;
	default:
		val |= MSHC_SDMA_BUF_BDARY_BYTES_4K;
		break;
	}

	mshc_writew(MSHC_BASE, val, MSHC_BLOCKSIZE_R);
}

void mshc_blkcnt_setup(unsigned int blkcnt)
{
	mshc_writew(MSHC_BASE, blkcnt, MSHC_BLOCKCOUNT_R);
}

void mshc_argument_setup(unsigned int arg)
{
	mshc_writel(MSHC_BASE, arg, MSHC_ARGUMENT_R);
}

void mshc_xfer_resp_int_enable()
{
	unsigned int val;

	val = mshc_readw(MSHC_BASE, MSHC_XFER_MODE_R);
	val &= ~MSHC_RESP_INT_DISABLE_BIT;
	mshc_writew(MSHC_BASE, val, MSHC_XFER_MODE_R);
}

void mshc_xfer_resp_int_disable()
{
	unsigned int val;

	val = mshc_readw(MSHC_BASE, MSHC_XFER_MODE_R);
	val |= MSHC_RESP_INT_DISABLE_BIT;
	mshc_writew(MSHC_BASE, val, MSHC_XFER_MODE_R);
}

void mshc_xfer_resp_err_chk_enable()
{
	unsigned int val;

	val = mshc_readw(MSHC_BASE, MSHC_XFER_MODE_R);
	val |= MSHC_RESP_ERR_CHK_ENABLE_BIT;
	mshc_writew(MSHC_BASE, val, MSHC_XFER_MODE_R);
}

void mshc_xfer_resp_err_chk_disable()
{
	unsigned int val;

	val = mshc_readw(MSHC_BASE, MSHC_XFER_MODE_R);
	val &= ~MSHC_RESP_ERR_CHK_ENABLE_BIT;
	mshc_writew(MSHC_BASE, val, MSHC_XFER_MODE_R);
}

void mshc_xfer_resp_type_setup(unsigned int type)
{
	unsigned int val;

	/* R1 :mem
	 * R5 :sdio
	 */

	val = mshc_readw(MSHC_BASE, MSHC_XFER_MODE_R);
	if(type == MSHC_RESP_TYPE_R1)
		val &= ~MSHC_RESP_TYPE_BIT;
	else
		val |= MSHC_RESP_TYPE_BIT;
	mshc_writew(MSHC_BASE, val, MSHC_XFER_MODE_R);
}

void mshc_xfer_sigblk_setup()
{
	unsigned int val;

	val = mshc_readw(MSHC_BASE, MSHC_XFER_MODE_R);
	val &= ~MSHC_MULTI_BLK_SEL_BIT; //single block
	mshc_writew(MSHC_BASE, val, MSHC_XFER_MODE_R);
}

void mshc_xfer_mulblk_setup()
{
	unsigned int val;

	val = mshc_readw(MSHC_BASE, MSHC_XFER_MODE_R);
	val |= MSHC_MULTI_BLK_SEL_BIT; //multiple block
	mshc_writew(MSHC_BASE, val, MSHC_XFER_MODE_R);
}

void mshc_xfer_mode_setup(unsigned int opsmode)
{
	unsigned int val;

	val = mshc_readw(MSHC_BASE, MSHC_XFER_MODE_R);
	if(opsmode == MSHC_DATA_XFER_DIR_RD){
		val |= MSHC_DATA_XFER_DIR_BIT;
	}else{
		val &= ~MSHC_DATA_XFER_DIR_BIT;
	}
	mshc_writew(MSHC_BASE, val, MSHC_XFER_MODE_R);
}

void mshc_xfer_auto_cmd_enable_setup(unsigned int cmd)
{
	unsigned int val;

	val = mshc_readw(MSHC_BASE, MSHC_XFER_MODE_R);

	val &= ~MSHC_AUTO_CMD_ENABLE_MASK; //auto cmd12
	switch (cmd) {
	case MSHC_AUTO_CMD_DISABLE:
		val |= MSHC_AUTO_CMD_DISABLE;
		break;
	case MSHC_AUTO_CMD12_ENABLE:
		val |= MSHC_AUTO_CMD12_ENABLE;
		break;
	case MSHC_AUTO_CMD23_ENABLE:
		val |= MSHC_AUTO_CMD23_ENABLE;
		break;
	case MSHC_AUTO_CMD_AUTO_SEL:
		val |= MSHC_AUTO_CMD_AUTO_SEL;
		break;
	default:
		break;
	}
	mshc_writew(MSHC_BASE, val, MSHC_XFER_MODE_R);
}

void mshc_xfer_blkcnt_enable()
{
	unsigned int val;

	val = mshc_readw(MSHC_BASE, MSHC_XFER_MODE_R);
	val |= MSHC_BLOCK_COUNT_ENABLE_BIT; // enable BLOCKCOUNT_R
	mshc_writew(MSHC_BASE, val, MSHC_XFER_MODE_R);
}

void mshc_xfer_blkcnt_disable()
{
	unsigned int val;

	val = mshc_readw(MSHC_BASE, MSHC_XFER_MODE_R);
	val &= ~MSHC_BLOCK_COUNT_ENABLE_BIT; // disble BLOCKCOUNT_R
	mshc_writew(MSHC_BASE, val, MSHC_XFER_MODE_R);
}


void mshc_xfer_dma_enable()
{
	unsigned int val;

	val = mshc_readw(MSHC_BASE, MSHC_XFER_MODE_R);
	val |= MSHC_DMA_ENABLE_BIT;
	mshc_writew(MSHC_BASE, val, MSHC_XFER_MODE_R);
}

void mshc_xfer_pio_enable()
{
	unsigned int val;

	val = mshc_readw(MSHC_BASE, MSHC_XFER_MODE_R);
	val &= ~MSHC_DMA_ENABLE_BIT;
	mshc_writew(MSHC_BASE, val, MSHC_XFER_MODE_R);
}

void mshc_adma_sa_low_setup(unsigned int addr)
{
	mshc_writel(MSHC_BASE, addr, MSHC_ADMA_SA_LOW_R);
}

void mshc_adma_id_low_setup(unsigned int addr)
{
	mshc_writel(MSHC_BASE, addr, MSHC_ADMA_ID_LOW_R);
}

void mshc_emmc_cqe_cqtdlba_setup(unsigned int addr)
{
	mshc_writel(MSHC_CQE_BASE, addr, MSHC_CQTDLBA);
}

void mshc_emmc_cqe_config_timeout()
{
	unsigned int val;

	val = mshc_readl(MSHC_CQE_BASE, MSHC_CQSSC1);
	val &= ~MSHC_SQSCMD_BLK_CNT_MASK;
	val |= 5 << MSHC_SQSCMD_BLK_CNT_LBIT;
	val &= ~MSHC_SQSCMD_IDLE_TMR_MASK;
	val |= 0x1000 << MSHC_SQSCMD_IDLE_TMR_LBIT;

	mshc_writel(MSHC_CQE_BASE, val, MSHC_CQSSC1);
}

void mshc_emmc_cqe_enable_inter()
{
	mshc_writel(MSHC_CQE_BASE, 0xffffffff, MSHC_CQISE);
	mshc_writel(MSHC_CQE_BASE, 0xffffffff, MSHC_CQISGE);

	mshc_writew(MSHC_BASE, 0xffff, MSHC_ERROR_INT_STAT_R);
	mshc_writew(MSHC_BASE, 0xffff, MSHC_ERROR_INT_STAT_EN_R);

	mshc_writew(MSHC_BASE, 0xfffc, MSHC_NORMAL_INT_STAT_EN_R);
	mshc_writew(MSHC_BASE, 0xfffc, MSHC_NORMAL_INT_SIGNAL_EN_R);
}

void mshc_emmc_cqe_set_rca(unsigned int cqe_rca)
{
	unsigned int val;

	val = mshc_readl(MSHC_CQE_BASE, MSHC_CQSSC2);

	val &= ~MSHC_SQSCMD_RCA_MASK;
	val |= cqe_rca << MSHC_SQSCMD_RCA_LBIT;

	mshc_writel(MSHC_CQE_BASE, val, MSHC_CQSSC2);
}

void mshc_emmc_cqe_config_dcmd(unsigned int is_cmd)
{
	unsigned int val;

	val = mshc_readl(MSHC_CQE_BASE, MSHC_CQCFG);

	if(MSHC_DCMD_CMD == is_cmd)
		val |= MSHC_DCMD_EN_BIT;
	else
		val &= MSHC_DCMD_EN_BIT;

	mshc_writel(MSHC_CQE_BASE, val, MSHC_CQCFG);
}

void mshc_emmc_cqe_config_task_size(unsigned int task_size)
{
	unsigned int val;

	val = mshc_readl(MSHC_CQE_BASE, MSHC_CQCFG);

	if(MSHC_TASK_DESC_SIZE_128 == task_size)
		val |= MSHC_TASK_DESC_SIZE_BIT;
	else
		val &= MSHC_TASK_DESC_SIZE_BIT;

	mshc_writel(MSHC_CQE_BASE, val, MSHC_CQCFG);
}

void mshc_emmc_cqe_enable()
{
	unsigned int val;

	val = mshc_readl(MSHC_CQE_BASE, MSHC_CQCFG);

	val |= MSHC_CQ_EN_BIT;

	mshc_writel(MSHC_CQE_BASE, val, MSHC_CQCFG);
}

void mshc_emmc_submit_one_task(unsigned int id)
{
	mshc_writel(MSHC_CQE_BASE, BIT(id), MSHC_CQTDBR);
}

unsigned int mshc_emmc_cqe_task_complete_int()
{
	unsigned int timeout = 3000000;

	while(!(mshc_readl(MSHC_CQE_BASE, MSHC_CQIS) \
				& MSHC_TCC_BIT) && --timeout)
	{
		udelay(10);
	}

	if(!timeout) {
		printf("emmc one task complete err ...\n");
		return -1;
	}

	mshc_emmc_cqe_task_complete_clear();

	return 0;
}

unsigned int mshc_emmc_cqe_task_complete_int_s()
{
	unsigned int timeout = 3000000;

	while(!(mshc_emmc_cqe_cqis_s \
				& MSHC_TCC_BIT) && --timeout)
	{
		udelay(10);
	}

	if(!timeout) {
		printf("emmc one task complete err ...\n");
		return -1;
	}

	mshc_emmc_cqe_task_complete_clear_s();

	return 0;
}

unsigned int get_1_offset(unsigned int a)
{
	int offset = 0;
	int timeout = 10000;
	while(!((a >> offset) & 0x1) && --timeout){
		offset++;
	}

	if(!timeout)
		printf("[error]:CQE Task has not complete. MSHC_CQTCN=%08x\n", a);

	return offset;
}
unsigned int mshc_emmc_get_task_complete_notified()
{
	unsigned int val;

	val = mshc_readl(MSHC_CQE_BASE, MSHC_CQTCN);

	return get_1_offset(val);
}

void mshc_emmc_clear_slot_complete(unsigned int tdl_index)
{
	mshc_writel(MSHC_CQE_BASE, BIT(tdl_index), MSHC_CQTCN);
}
unsigned char *mshc_response_get()
{
	static unsigned char resp[20];
	unsigned int RESP_ARRAY[4];
	unsigned int words, i, j;

	memset(RESP_ARRAY, 0, sizeof(RESP_ARRAY));
	RESP_ARRAY[3] = mshc_readl(MSHC_BASE, MSHC_RESP01_R);
	RESP_ARRAY[2] = mshc_readl(MSHC_BASE, MSHC_RESP23_R);
	RESP_ARRAY[1] = mshc_readl(MSHC_BASE, MSHC_RESP45_R);
	RESP_ARRAY[0] = mshc_readl(MSHC_BASE, MSHC_RESP67_R);

#ifdef DEBUG
	dump_response();
#endif

	words = 1; //low lost eight
	memset(resp, 0, 20);
	for(i = 4; i > 0; i--){
		for(j = 0; j < 4; j++){
			resp[words] = (RESP_ARRAY[i - 1] & (0xff << j * 8)) >> j * 8;
			words++;
		}
	}

	return resp;
}

unsigned int mshc_buf_data_read()
{
	unsigned val;

	val = mshc_readl(MSHC_BASE, MSHC_BUF_DATA_R);

	return val;
}

void mshc_buf_data_write(unsigned int data)
{
	mshc_writel(MSHC_BASE, data, MSHC_BUF_DATA_R);
}

unsigned int mshc_card_addr_adjust(unsigned int cardtype, unsigned int addr, unsigned int blksize)
{
	if(cardtype)
		return addr;
	else
		return blksize * addr;
}

unsigned int wait_buf_read()
{
	unsigned int timeout = 30000;

	while(!(mshc_readw(MSHC_BASE, MSHC_NORMAL_INT_STAT_R) \
				& MSHC_BUF_RD_READY_STAT_BIT) && --timeout)
	{
		udelay(10);  //1 block read time
	}

	if(!timeout) {
		printf("buf read timeout err ...\n");
		dump_error_status();
		return -1;
	}

	mshc_normal_int_buf_rd_ready();

	return 0;
}

unsigned int wait_buf_read_s()
{
	unsigned int timeout = 30000;

	while(!(mshc_normal_int_stat_s \
				& MSHC_BUF_RD_READY_STAT_BIT) && --timeout)
	{
		udelay(10);  //1 block read time
	}

	if(!timeout) {
		printf("software: buf read timeout err ...\n");
		printf("\n===>mshc_error_int_stat_s=0x%08x\n", mshc_error_int_stat_s);
		return -1;
	}

	mshc_normal_int_buf_rd_ready_s();

	return 0;
}

unsigned int wait_buf_write()
{
	unsigned int timeout = 30000;

	while(!(mshc_readw(MSHC_BASE, MSHC_NORMAL_INT_STAT_R) \
				& MSHC_BUF_WR_READY_STAT_BIT) && --timeout)
	{
		udelay(10);
	}

	if(!timeout) {
		printf("buf write timeout err ...\n");
		dump_error_status();
		return -1;
	}

	mshc_normal_int_buf_wr_ready();

	return 0;
}

unsigned int wait_buf_write_s()
{
	unsigned int timeout = 30000;

	while(!(mshc_normal_int_stat_s \
				& MSHC_BUF_WR_READY_STAT_BIT) && --timeout)
	{
		udelay(10);
	}

	if(!timeout) {
		printf("software: buf write timeout err ...\n");
		printf("\n===>mshc_error_int_stat_s=0x%08x\n", mshc_error_int_stat_s);
		return -1;
	}

	mshc_normal_int_buf_wr_ready_s();

	return 0;
}

unsigned int wait_cmd_complete()
{
	unsigned int timeout = 100000;

	while(!(mshc_readw(MSHC_BASE, MSHC_NORMAL_INT_STAT_R) \
				& MSHC_CMD_COMPLETE_STAT_BIT) && --timeout)
	{
		udelay(10);
	}

	if(!timeout) {
		printf("cmd%d complete timeout err ...\n", MSHC_GET_CMD(mshc_readw(MSHC_BASE, MSHC_CMD_R)));
		dump_error_status();
		return -1;
	}

	mshc_normal_int_cmd_complete();

	return 0;
}

unsigned int wait_cmd_complete_s()
{
	unsigned int timeout = 3000000;

	while(!(mshc_normal_int_stat_s \
				& MSHC_CMD_COMPLETE_STAT_BIT) && --timeout)
	{
		udelay(10);
	}

	if(!timeout) {
		printf("software: cmd complete timeout err ...\n");
		printf("\n===>mshc_error_int_stat_s=0x%08x\n", mshc_error_int_stat_s);
		return -1;
	}

	mshc_normal_int_cmd_complete_s();

	return 0;
}

unsigned int wait_xfer_complete()
{
	unsigned int timeout = 3000000;

	while(!(mshc_readw(MSHC_BASE, MSHC_NORMAL_INT_STAT_R) \
				& MSHC_XFER_COMPLETE_STAT_BIT) && --timeout){
		udelay(10);
	}

	if(!timeout) {
		printf("xfer complete timeout err ...\n");
		dump_error_status();
		return -1;
	}

	mshc_normal_int_xfer_complete();

	return 0;
}

unsigned int wait_xfer_complete_s()
{
	unsigned int timeout = 3000000;

	while(!(mshc_normal_int_stat_s \
				& MSHC_XFER_COMPLETE_STAT_BIT) && --timeout){
		udelay(10);
	}

	if(!timeout) {
		printf("software:xfer complete timeout err ...\n");
		printf("\n===>mshc_error_int_stat_s=0x%08x\n", mshc_error_int_stat_s);
		return -1;
	}

	mshc_normal_int_xfer_complete_s();

	return 0;
}

unsigned int do_wait_dma_interrupt()
{
	int val;

	val = mshc_readw(MSHC_BASE, MSHC_NORMAL_INT_STAT_R) \
		  & MSHC_DMA_INTERRUPT_STAT_BIT;

	if(val)
		return 0;
	else
		return -1;
}

unsigned int do_wait_dma_interrupt_s()
{
	int val;

	val = mshc_normal_int_stat_s \
		  & MSHC_DMA_INTERRUPT_STAT_BIT;

	if(val)
		return 0;
	else
		return -1;
}

unsigned int do_wait_xfer_complete()
{
	int val;

	val = mshc_readw(MSHC_BASE, MSHC_NORMAL_INT_STAT_R)  \
		  & MSHC_XFER_COMPLETE_STAT_BIT;

	if(val)
		return 0;
	else
		return -1;
}

unsigned int do_wait_xfer_complete_s()
{
	int val;

	val = mshc_normal_int_stat_s \
		  & MSHC_XFER_COMPLETE_STAT_BIT;

	if(val)
		return 0;
	else
		return -1;
}

unsigned int cmd_err_stat_check()
{
	if(mshc_readw(MSHC_BASE, MSHC_ERROR_INT_STAT_R) \
			& (MSHC_CMD_TOUT_ERR_STAT_BIT \
				| MSHC_CMD_CRC_ERR_STAT_BIT \
				| MSHC_CMD_END_BIT_ERR_STAT_BIT \
				| MSHC_CMD_IDX_ERR_STAT_BIT)){
		printf("[ERROR]:Command [%d] check Error!!!\n", MSHC_GET_CMD(mshc_readw(MSHC_BASE, MSHC_CMD_R)));
		dump_error_status();
		mshc_writew(MSHC_BASE, 0xffff, MSHC_ERROR_INT_STAT_R);
		return -1;
	}
	return 0;
}

unsigned int cmd_err_stat_check_s()
{
	if(mshc_error_int_stat_s \
			& (MSHC_CMD_TOUT_ERR_STAT_BIT \
				| MSHC_CMD_CRC_ERR_STAT_BIT \
				| MSHC_CMD_END_BIT_ERR_STAT_BIT \
				| MSHC_CMD_IDX_ERR_STAT_BIT)){
		printf("[ERROR]:software:Command check Error!!!\n");
		printf("\n===>mshc_error_int_stat_s=0x%08x\n", mshc_error_int_stat_s);
		dump_error_status();
		return -1;
	}
	return 0;
}

unsigned int xfer_err_stat_check()
{
	if(mshc_readw(MSHC_BASE, MSHC_ERROR_INT_STAT_R) \
			& (MSHC_DATA_TOUT_ERR_STAT_BIT \
				| MSHC_DATA_CRC_ERR_STAT_BIT)){
		printf("[ERROR]:Data Xfer Error!!!\n");
		dump_error_status();
		return -1;
	}
	return 0;
}

unsigned int xfer_err_stat_check_s()
{
	if(mshc_error_int_stat_s \
			& (MSHC_DATA_TOUT_ERR_STAT_BIT \
				| MSHC_DATA_CRC_ERR_STAT_BIT)){
		printf("[ERROR]: software:Data Xfer Error!!!\n");
		printf("\n===>mshc_error_int_stat_s=0x%08x\n", mshc_error_int_stat_s);
		dump_error_status();
		return -1;
	}
	return 0;
}


void mshc_data_pre(int ops, int is_dma, unsigned int blksize, unsigned int blkcnt)
{

	/* common set */
	mshc_blksize_setup(blksize);
	if(mshc_readw(MSHC_BASE, MSHC_HOST_CTRL2_R) & MSHC_HOST_VER4_ENABLE_BIT) {
		mshc_blkcnt_setup(0);
		mshc_writel(MSHC_BASE, blkcnt, MSHC_SDMASA_R);
	} else {
		mshc_blkcnt_setup(blkcnt);
	}

	mshc_xfer_blkcnt_enable();
	if(blkcnt == 1)
		mshc_xfer_sigblk_setup();
	else
		mshc_xfer_mulblk_setup();


	if(ops == MSHC_DATA_XFER_DIR_RD)
		mshc_xfer_mode_setup(MSHC_DATA_XFER_DIR_RD);
	else
		mshc_xfer_mode_setup(MSHC_DATA_XFER_DIR_WR);

	mshc_xfer_auto_cmd_enable_setup(MSHC_AUTO_CMD12_ENABLE);

	/*  if(mshc_readw(MSHC_BASE, MSHC_XFER_MODE_R) & MSHC_RESP_ERR_CHK_ENABLE_BIT) {*/
	/*      mshc_xfer_resp_int_disable();*/
	/*      mshc_xfer_resp_type_setup(MSHC_RESP_TYPE_R1);*/
	/*  }*/
	if(is_dma == MSHC_DMA_ENABLE)
		mshc_xfer_dma_enable();
	else
		mshc_xfer_pio_enable();
}

unsigned int mshc_cmd_dat_line_chk(unsigned int cmdidx)
{
	unsigned int timeout = 1000;

	while((mshc_readl(MSHC_BASE, MSHC_PSTATE_REG) & MSHC_CMD_INHIBIT_BIT) && --timeout) {
		udelay(10);
	}

	if(!timeout) {
		printf("cmd line chk timeout \n");
		return -1;
	}

	if(!mshc_check_data_line(14)) {
		if(MMC_STOP_TRANSMISSION == cmdidx) {
			return 0;
		} else {
			timeout = 1000;
			while((mshc_readl(MSHC_BASE, MSHC_PSTATE_REG) & MSHC_CMD_INHIBIT_BIT) && --timeout) {
				udelay(10);
			}

			if(!timeout) {
				printf("dat line chk timeout \n");
				return -1;
			}
		}
	} else {
		return 0;
	}
}

void mshc_data_tran_rst()
{
	mshc_reset(MSHC_SW_RST_CMD_BIT);
	mshc_reset(MSHC_SW_RST_DAT_BIT);
}

unsigned short package_cmd(unsigned int cmdidx, unsigned cmdat, int rtype)
{
	unsigned short cmd = 0;

	cmd |= cmdat;
	cmd |= cmdidx << MSHC_CMD_INDEX_LBIT;

	switch (rtype) {
		case MSHC_CMDAT_RESPONSE_R1:
		case MSHC_CMDAT_RESPONSE_R7:
			cmd |= MSHC_CMD_IDX_CHK_ENABLE_BIT;
			cmd	|= MSHC_CMD_CRC_CHK_ENABLE_BIT; //command index & crc check enable
			cmd &= ~MSHC_RESP_TYPE_SELECT_MASK;
			cmd |= MSHC_RESP_TYPE_SELECT_RESP_LEN_48; //length 48
			break;
		case MSHC_CMDAT_RESPONSE_R1b:
			cmd |= MSHC_CMD_IDX_CHK_ENABLE_BIT;
			cmd	|= MSHC_CMD_CRC_CHK_ENABLE_BIT; //command index & crc check enable
			cmd &= ~MSHC_RESP_TYPE_SELECT_MASK;
			cmd |= MSHC_RESP_TYPE_SELECT_RESP_LEN_48B; //length 48B
			break;
		case MSHC_CMDAT_RESPONSE_R2:
			cmd &= ~MSHC_CMD_IDX_CHK_ENABLE_BIT;
			cmd	&= ~MSHC_CMD_CRC_CHK_ENABLE_BIT; //command index & crc check disable
			cmd &= ~MSHC_RESP_TYPE_SELECT_MASK;
			cmd |= MSHC_RESP_TYPE_SELECT_RESP_LEN_136; //length 136
			break;
		case MSHC_CMDAT_RESPONSE_R3:
		case MSHC_CMDAT_RESPONSE_R6:
			cmd &= ~MSHC_CMD_IDX_CHK_ENABLE_BIT;
			cmd	&= ~MSHC_CMD_CRC_CHK_ENABLE_BIT; //command index & crc check disable
			cmd &= ~MSHC_RESP_TYPE_SELECT_MASK;
			cmd |= MSHC_RESP_TYPE_SELECT_RESP_LEN_48; //length 48
			break;
		default:
			break;
	}

	return cmd;
}

void mshc_interrupt_handler(unsigned int arg)
{
/*	dump_error_status();*/
/*	printf("------process interrupt------\n");*/
	mdelay(1);
	mshc_normal_int_stat_s = mshc_readw(MSHC_BASE, MSHC_NORMAL_INT_STAT_R);
	mshc_error_int_stat_s = mshc_readw(MSHC_BASE, MSHC_ERROR_INT_STAT_R);
	mshc_emmc_cqe_cqis_s = mshc_readl(MSHC_CQE_BASE, MSHC_CQIS);

	mshc_writew(MSHC_BASE, 0xffff, MSHC_NORMAL_INT_STAT_R);
	mshc_writew(MSHC_BASE, 0xffff, MSHC_ERROR_INT_STAT_R);
	mshc_writel(MSHC_CQE_BASE, 0xf, MSHC_CQIS);
}

unsigned char* mshc_cmd_send(unsigned int cmdidx, unsigned int arg, unsigned int cmdat, int rtype)
{
	unsigned char *resp = NULL;
	unsigned short val;
	unsigned char resp_mask[20] = CMD_ERR;

	if(mshc_cmd_dat_line_chk(cmdidx)) {
		printf("line chk timout err \n");
		goto exit;
	}

	mshc_writel(MSHC_BASE, arg, MSHC_ARGUMENT_R);

	val = package_cmd(cmdidx, cmdat, rtype);
#ifdef DEBUG
	printf("cmdidx=%d, val=0x%x, arg=0x%x, response=R%d\n", cmdidx, val, arg, rtype);
#endif
	mshc_writew(MSHC_BASE, val, MSHC_CMD_R);

#ifdef MSHC_USE_INT
	if(wait_cmd_complete_s())
#else
	if(wait_cmd_complete())
#endif
		goto exit;

#ifdef MSHC_USE_INT
	if(cmd_err_stat_check_s())
#else
	if(cmd_err_stat_check())
#endif
		goto exit;

	resp = mshc_response_get();

#ifdef DEBUG
	dump_error_status();
#endif

	return resp;
exit:
	return resp_mask;
}


