#include <stdio.h>
#include <mshc/dw_mshc.h>

void mshc_host1_dma_sel(unsigned int type)
{
	unsigned int val;

	val = mshc_readb(MSHC_BASE, MSHC_HOST_CTRL1_R);
	val &= ~MSHC_DMA_SEL_MASK;

	switch (type) {
	case MSHC_DMA_SEL_SDMA:
		val |= MSHC_DMA_SEL_SDMA;
		break;
	case MSHC_DMA_SEL_ADMA2:
		val |= MSHC_DMA_SEL_ADMA2;
		break;
	case MSHC_DMA_SEL_ADMA2_3:
		val |= MSHC_DMA_SEL_ADMA2_3;
		break;
	default:
		break;
	}

	mshc_writeb(MSHC_BASE, val, MSHC_HOST_CTRL1_R);
}

void mshc_host1_high_speed_enable()
{
	unsigned int val;

	val = mshc_readb(MSHC_BASE, MSHC_HOST_CTRL1_R);
	val |= MSHC_HIGH_SPEED_EN_BIT;
	mshc_writeb(MSHC_BASE, val, MSHC_HOST_CTRL1_R);
}

void mshc_host1_normal_speed_enable()
{
	unsigned int val;

	val = mshc_readb(MSHC_BASE, MSHC_HOST_CTRL1_R);
	val &= ~MSHC_HIGH_SPEED_EN_BIT;
	mshc_writeb(MSHC_BASE, val, MSHC_HOST_CTRL1_R);
}

void mshc_host1_dat_xfer_width_setup(unsigned int buswidth)
{
	unsigned int val;

	/* bus width */
	val = mshc_readb(MSHC_BASE, MSHC_HOST_CTRL1_R);

	switch (buswidth) {
	case MSHC_BUS_WIDTH_1:
		val &= ~MSHC_DAT_XFER_WIDTH_BIT;  //1bit mode
		break;
	case MSHC_BUS_WIDTH_4:
		val |= MSHC_DAT_XFER_WIDTH_BIT;  //4bit mode
		break;
	case MSHC_BUS_WIDTH_8:
		val |= MSHC_EXT_DAT_XFER_BIT;  //8bit mode
		break;
	default:
		val &= ~MSHC_DAT_XFER_WIDTH_BIT;  //1bit mode
		break;
	}

	mshc_writeb(MSHC_BASE, val, MSHC_HOST_CTRL1_R);
}

void mshc_clk_sd_clk_enable()
{
	unsigned int val;

	val = mshc_readw(MSHC_BASE, MSHC_CLK_CTRL_R);
	val |= MSHC_SD_CLK_EN_BIT;
	mshc_writew(MSHC_BASE, val, MSHC_CLK_CTRL_R);
}

void mshc_clk_sd_clk_disable()
{
	unsigned int val;

	val = mshc_readw(MSHC_BASE, MSHC_CLK_CTRL_R);
	val &= ~MSHC_SD_CLK_EN_BIT;
	mshc_writew(MSHC_BASE, val, MSHC_CLK_CTRL_R);
}

void mshc_tout_cnt_setup(unsigned int value)
{
	/* value < 0xf */
	mshc_writeb(MSHC_BASE, value, MSHC_TOUT_CTRL_R);
}

void mshc_sw_reset_dat()
{
	unsigned int val;

	val = mshc_readb(MSHC_BASE, MSHC_SW_RST_R);
	val |= MSHC_SW_RST_DAT_BIT;
	mshc_writeb(MSHC_BASE, val, MSHC_SW_RST_R);
}

void mshc_sw_reset_cmd()
{
	unsigned int val;

	val = mshc_readb(MSHC_BASE, MSHC_SW_RST_R);
	val |= MSHC_SW_RST_CMD_BIT;
	mshc_writeb(MSHC_BASE, val, MSHC_SW_RST_R);
}

void mshc_normal_int_cqe_event_detect()
{
	unsigned int val;

	val = MSHC_CQE_EVENT_STAT_BIT;
	mshc_writew(MSHC_BASE, val, MSHC_NORMAL_INT_STAT_R);
}

void mshc_normal_int_card_remove()
{
	unsigned int val;

	val = MSHC_CARD_REMOVAL_STAT_BIT;
	mshc_writew(MSHC_BASE, val, MSHC_NORMAL_INT_STAT_R);
}

void mshc_normal_int_card_insert()
{
	unsigned int val;

	val = MSHC_CARD_INSERTION_STAT_BIT;
	mshc_writew(MSHC_BASE, val, MSHC_NORMAL_INT_STAT_R);
}

void mshc_normal_int_buf_rd_ready()
{
	unsigned int val;

	val = MSHC_BUF_RD_READY_STAT_BIT;
	mshc_writew(MSHC_BASE, val, MSHC_NORMAL_INT_STAT_R);
}

void mshc_normal_int_buf_rd_ready_s()
{
	mshc_normal_int_stat_s &= ~MSHC_BUF_RD_READY_STAT_BIT;
}


void mshc_normal_int_buf_wr_ready()
{
	unsigned int val;

	val = MSHC_BUF_WR_READY_STAT_BIT;
	mshc_writew(MSHC_BASE, val, MSHC_NORMAL_INT_STAT_R);
}

void mshc_normal_int_buf_wr_ready_s()
{
	mshc_normal_int_stat_s &= ~MSHC_BUF_WR_READY_STAT_BIT;
}


void mshc_normal_int_dma_interrupt_generated()
{
	unsigned int val;

	val = MSHC_DMA_INTERRUPT_STAT_BIT;
	mshc_writew(MSHC_BASE, val, MSHC_NORMAL_INT_STAT_R);
}

void mshc_normal_int_dma_interrupt_generated_s()
{
	mshc_normal_int_stat_s &= ~MSHC_DMA_INTERRUPT_STAT_BIT;
}

void mshc_normal_int_bgap_event_stop()
{
	unsigned int val;

	val = MSHC_BGAP_EVENT_STAT_BIT;
	mshc_writew(MSHC_BASE, val, MSHC_NORMAL_INT_STAT_R);
}

void mshc_normal_int_xfer_complete()
{
	unsigned int val;

	val = MSHC_XFER_COMPLETE_STAT_BIT;
	mshc_writew(MSHC_BASE, val, MSHC_NORMAL_INT_STAT_R);
}

void mshc_normal_int_xfer_complete_s()
{
	mshc_normal_int_stat_s &= ~MSHC_XFER_COMPLETE_STAT_BIT;
}

void mshc_normal_int_cmd_complete()
{
	unsigned int val;

	val = MSHC_CMD_COMPLETE_STAT_BIT;  //can not or
	mshc_writew(MSHC_BASE, val, MSHC_NORMAL_INT_STAT_R);
}

void mshc_normal_int_cmd_complete_s()
{
	mshc_normal_int_stat_s &= ~MSHC_CMD_COMPLETE_STAT_BIT;  //can not or
}
void mshc_normal_int_en_card_remove()
{
	unsigned int val;

	val = mshc_readw(MSHC_BASE, MSHC_NORMAL_INT_STAT_EN_R);
	val |= MSHC_CARD_REMOVAL_STAT_EN_BIT;
	mshc_writew(MSHC_BASE, val, MSHC_NORMAL_INT_STAT_EN_R);
}

void mshc_normal_int_en_card_insert()
{
	unsigned int val;

	val = mshc_readw(MSHC_BASE, MSHC_NORMAL_INT_STAT_EN_R);
	val |= MSHC_CARD_INSERTION_STAT_EN_BIT;
	mshc_writew(MSHC_BASE, val, MSHC_NORMAL_INT_STAT_EN_R);
}

void mshc_normal_int_sig_en_card_remove()
{
	unsigned int val;

	val = mshc_readw(MSHC_BASE, MSHC_NORMAL_INT_SIGNAL_EN_R);
	val |= MSHC_CARD_REMOVAL_SIGNAL_EN_BIT;
	mshc_writew(MSHC_BASE, val, MSHC_NORMAL_INT_SIGNAL_EN_R);
}

void mshc_normal_int_sig_en_card_insert()
{
	unsigned int val;

	val = mshc_readw(MSHC_BASE, MSHC_NORMAL_INT_SIGNAL_EN_R);
	val |= MSHC_CARD_INSERTION_SIGNAL_EN_BIT;
	mshc_writew(MSHC_BASE, val, MSHC_NORMAL_INT_SIGNAL_EN_R);
}

void mshc_err_int_adma_err_clear()
{
	unsigned int val;

	val = MSHC_ADMA_ERR_STAT_BIT;
	mshc_writew(MSHC_BASE, val, MSHC_ERROR_INT_STAT_R);
}

void mshc_err_int_adma_err_clear_s()
{
	mshc_error_int_stat_s &= ~MSHC_ADMA_ERR_STAT_BIT;
}

void mshc_host2_preset_val_enable()
{
	unsigned int val;

	val = mshc_readw(MSHC_BASE, MSHC_HOST_CTRL2_R);
	val |= MSHC_PRESET_VAL_ENABLE_BIT;
	mshc_writew(MSHC_BASE, val, MSHC_HOST_CTRL2_R);
}

void mshc_host2_uhs_mode_sel(unsigned int speedmode)
{
	unsigned int val;

	val = mshc_readw(MSHC_BASE, MSHC_HOST_CTRL2_R);
	val &= ~MSHC_UHS_MODE_SEL_MASK;

	switch (speedmode) {
	case MSHC_UHS_MODE_SEL_SDR12:
		val |= MSHC_UHS_MODE_SEL_SDR12;
		break;
	case MSHC_UHS_MODE_SEL_SDR25:
		val |= MSHC_UHS_MODE_SEL_SDR25;
		break;
	case MSHC_UHS_MODE_SEL_SDR50:
		val |= MSHC_UHS_MODE_SEL_SDR50;
		break;
	case MSHC_UHS_MODE_SEL_SDR104:
		val |= MSHC_UHS_MODE_SEL_SDR104;
		break;
	case MSHC_UHS_MODE_SEL_DDR50:
		val |= MSHC_UHS_MODE_SEL_DDR50;
		break;
	default:
		val |= MSHC_UHS_MODE_SEL_SDR12;
		break;
	}

	mshc_writew(MSHC_BASE, val, MSHC_HOST_CTRL2_R);
}

void mshc_host2_ver4_enable()
{
	unsigned int val;

	val = mshc_readw(MSHC_BASE, MSHC_HOST_CTRL2_R);
	val |= MSHC_HOST_VER4_ENABLE_BIT;
	mshc_writew(MSHC_BASE, val, MSHC_HOST_CTRL2_R);
}


void mshc_host2_ver4_disable()
{
	unsigned int val;

	val = mshc_readw(MSHC_BASE, MSHC_HOST_CTRL2_R);
	val &= ~MSHC_HOST_VER4_ENABLE_BIT;
	mshc_writew(MSHC_BASE, val, MSHC_HOST_CTRL2_R);
}

void mshc_host2_adma2_len_sel(unsigned int mode)
{
	unsigned int val;

	val = mshc_readw(MSHC_BASE, MSHC_HOST_CTRL2_R);

	if(mode == MSHC_ADMA2_LEN_MODE_26)
		val |= MSHC_ADMA2_LEN_MODE_BIT;
	else
		val &= ~MSHC_ADMA2_LEN_MODE_BIT;

	mshc_writew(MSHC_BASE, val, MSHC_HOST_CTRL2_R);
}

void mshc_host2_adma_addr_len_sel(unsigned int len)
{
	unsigned int val;

	val = mshc_readw(MSHC_BASE, MSHC_HOST_CTRL2_R);

	if(len == MSHC_ADDRESSING_64)
		val |= MSHC_ADDRESSING_BIT;
	else
		val &= ~MSHC_ADDRESSING_BIT;

	mshc_writew(MSHC_BASE, val, MSHC_HOST_CTRL2_R);
}

void mshc_emmc_card_is_emmc()
{
	unsigned int val;

	val = mshc_readw(MSHC_VENDOR_BASE, MSHC_EMMC_CTRL_R);
	val |= MSHC_CARD_IS_EMMC_BIT;
	mshc_writew(MSHC_VENDOR_BASE, val, MSHC_EMMC_CTRL_R);
}

void mshc_emmc_cqe_task_complete_clear()
{
	printf("-wqshao--%s,   %d---MSHC_CQIS=%08x\n",  __func__, __LINE__, mshc_readl(MSHC_CQE_BASE, MSHC_CQIS));
	mshc_writel(MSHC_CQE_BASE, MSHC_TCC_BIT, MSHC_CQIS);
	printf("-wqshao--%s,   %d---MSHC_CQIS=%08x\n",  __func__, __LINE__, mshc_readl(MSHC_CQE_BASE, MSHC_CQIS));
}

void mshc_emmc_cqe_task_complete_clear_s()
{
	mshc_emmc_cqe_cqis_s &= ~MSHC_TCC_BIT;
}

static void mshc_set_f4785_gpio()
{
	REG_GPIO_PXINTC(4)  = 0x30f00000;
	REG_GPIO_PXMASKC(4) = 0x30f00000;
	REG_GPIO_PXPAT1C(4) = 0x30f00000;
	REG_GPIO_PXPAT0S(4) = 0x30f00000;
}

static void mshc_set_x2000_gpio_func0(int group, unsigned int pins)
{
	REG_GPIO_PXINTC(group)  = pins;
	REG_GPIO_PXMASKC(group) = pins;
	REG_GPIO_PXPAT1C(group) = pins;
	REG_GPIO_PXPAT0C(group) = pins;
}

void mshc_gpio_init()
{
#ifdef MSHC_4785_TEST
	mshc_set_f4785_gpio();
#else
 #if defined(MSHC_TEST_X2000_MSHC1)
	mshc_set_x2000_gpio_func0(4, 0x3f << 11);
 #else
	mshc_set_x2000_gpio_func0(4, 0x3ff);
 #endif
#endif
}

void mshc_reset(unsigned int mask)
{
	unsigned long timeout;

	/* Wait max 100 ms */
	timeout = 100;
	mshc_writeb(MSHC_BASE, mask, MSHC_SW_RST_R);
	while (mshc_readb(MSHC_BASE, MSHC_SW_RST_R) & mask) {
		if (timeout == 0) {
			printf("Reset 0x%x never completed.\n", (int)mask);
			return;
		}
		timeout--;
		udelay(1000);
	}
}

#ifdef MSHC_4785_TEST
/* voltage set level 1/0 = 3.3/1.8v */
void mshc_voltage_set(int level)
{
	REG_GPIO_PXINTC(0)  = 0x00000100;
	REG_GPIO_PXMASKS(0) = 0x00000100;
	REG_GPIO_PXPAT1C(0) = 0x00000100;
	if(level){
		REG_GPIO_PXPAT0S(0) = 0x00000100;
	}else{
		REG_GPIO_PXPAT0C(0) = 0x00000100;
	}
}
#else
void mshc_voltage_set(int level)
{
	REG_GPIO_PXINTC(0)  = 0x1 << 0;
	REG_GPIO_PXMASKS(0) = 0x1 << 0;
	REG_GPIO_PXPAT1C(0) = 0x1 << 0;
	if(level){
		REG_GPIO_PXPAT0S(0) = 0x1 << 0;
	}else{
		REG_GPIO_PXPAT0C(0) = 0x1 << 0;
	}
}
#endif

unsigned int mshc_check_data_line(unsigned int mark)
{
	int data;

	data = (mshc_readl(MSHC_BASE, MSHC_PSTATE_REG) & \
		   MSHC_DAT_3_0_MASK) >> MSHC_DAT_3_0_LBIT;

	if(data == mark)
		return 0;
	else
		return -1;
}

static void mshc_clk_adjust()
{
	int val;
	val = mshc_readw(MSHC_BASE, MSHC_CLK_CTRL_R);
	/*
	 * Host Version 4 enable
	 * clock control changes
	 * */
	if(mshc_readw(MSHC_BASE, MSHC_HOST_CTRL2_R) & MSHC_HOST_VER4_ENABLE_BIT) {
		val |= MSHC_PLL_ENABLE_BIT;
		mshc_writew(MSHC_BASE, val, MSHC_CLK_CTRL_R);
	} else {
		val |= MSHC_INTERNAL_CLK_EN_BIT;
		mshc_writew(MSHC_BASE, val, MSHC_CLK_CTRL_R);
	}
}
void mshc_lowclk_enable()
{
	unsigned int val;

	/* set low clk */
	val = mshc_readl(MSHC_CLK_CTRL_SOURCE, 0);
	val &= ~MSC_CLK_H_FREQ;
	mshc_writel(MSHC_CLK_CTRL_SOURCE, val, 0);

	/* Enable card_clk_en pin */
	mshc_clk_adjust();
}

void mshc_highclk_enable()
{
	unsigned int val;

	/* set high clk */
	val = mshc_readl(MSHC_CLK_CTRL_SOURCE, 0);
	val |= MSC_CLK_H_FREQ;
	mshc_writel(MSHC_CLK_CTRL_SOURCE, val, 0);

	mshc_clk_adjust();
}

unsigned int is_card_inserted()
{
	if((mshc_readl(MSHC_BASE, MSHC_PSTATE_REG) & MSHC_CARD_INSERTED_BIT)) {
		printf("card is instert\n");
		return 0;
	} else {
		printf("card not instert\n");
		return -1;
	}
}

unsigned int mshc_card_detect()
{
	unsigned int val;
	unsigned int ret;

	/* enable interrupt for card detect */
	mshc_normal_int_en_card_insert();
	mshc_normal_int_en_card_remove();

	mshc_normal_int_sig_en_card_insert();
	mshc_normal_int_sig_en_card_remove();

	/* clear card detect interrupt status */
	if(mshc_readw(MSHC_BASE, MSHC_NORMAL_INT_STAT_R) & MSHC_CARD_INTERRUPT_STAT_BIT)
		mshc_normal_int_card_insert();
	else
		mshc_normal_int_card_remove();

	/* check if card insert */
	ret = is_card_inserted();

	return ret;
}

void mshc_card_clk_supply()
{
	mshc_clk_sd_clk_enable();
}

void mshc_card_clk_stop()
{
	mshc_clk_sd_clk_disable();
}

void mshc_int_mask()
{
	mshc_writew(MSHC_BASE, 0xffff, MSHC_NORMAL_INT_STAT_EN_R);
	mshc_writew(MSHC_BASE, 0xffff, MSHC_ERROR_INT_STAT_EN_R);
	mshc_writew(MSHC_BASE, 0xffff, MSHC_NORMAL_INT_SIGNAL_EN_R);
}

void mshc_clk_ctrl()
{
	mshc_lowclk_enable();
	/* Enable clk2card_on pin*/
	mshc_card_clk_supply();
}

unsigned int mshc_host_init()
{
	unsigned int timeout = 1000;
	unsigned int val;

	mshc_gpio_init();
	mshc_reset(MSHC_SW_RST_ALL_BIT);
	mshc_clk_ctrl();
	mshc_int_mask();
	mshc_tout_cnt_setup(0xe);

#ifdef MSHC_USE_INT
	printf("%s: use interrupt!!!\n", __func__);
	request_irq(IRQ_MSC0, mshc_interrupt_handler, 0);
#endif
	return 0;
}
