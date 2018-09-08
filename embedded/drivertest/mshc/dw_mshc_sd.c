#include <stdio.h>
#include <mshc/dw_mshc.h>

unsigned short mshc_sd_get_rca()
{
	unsigned char *resp;
	unsigned short rca;

	mshc_cmd_send(MMC_ALL_SEND_CID, 0, 0, MSHC_CMDAT_RESPONSE_R2);
	resp = mshc_cmd_send(SD_SEND_RELATIVE_ADDR, 0, 0, MSHC_CMDAT_RESPONSE_R6);
	rca = resp[4] << 8 | resp[3];

	printf("===>::sd get rca = %x\n", rca);
	return rca;
}

unsigned char *mshc_sd_get_cid(unsigned short rca)
{
	unsigned char *resp;

	resp = mshc_cmd_send(MMC_SEND_CID, rca << 16, 0, MSHC_CMDAT_RESPONSE_R2);

	return resp;
}

unsigned char *mshc_sd_get_csd(unsigned short rca)
{
	unsigned char *resp;

	resp = mshc_cmd_send(MMC_SEND_CSD, rca << 16, 0, MSHC_CMDAT_RESPONSE_R2);

	return resp;
}

unsigned int mshc_cmd_sd_send_if_cond(unsigned int vol)
{
	unsigned char *resp;
	unsigned int val;
	unsigned int arg;
	unsigned char chk_pattern = 0xaa;

	/* cmd8 arg bits desc:
	 * 0 ~ 7: check pattern
	 * 8 ~ 11: VHS
	 */

	arg = chk_pattern;
	arg |= vol << 8;

	resp = mshc_cmd_send(SD_SEND_IF_COND, arg, 0, MSHC_CMDAT_RESPONSE_R7);

	if(resp[1] != chk_pattern)
		return -1;
	else
		return 0;
}

unsigned char *mshc_cmd_app_op_cond(unsigned int vol, unsigned int s18r, unsigned int xpc, unsigned int hcs)
{
	unsigned char *resp;
	unsigned int arg;
	unsigned int timeout = 1000;

	/* acmd41 arg bits desc:
	 * 0 ~ 23: vdd voltage
	 * 24	 : s18r
	 * 28	 : xpc
	 * 30	 : hcs
	 */

	arg |= hcs << 30;
	arg |= xpc << 28;
	arg |= s18r << 24;
	arg |= vol & 0xffffff;

	mshc_cmd_send(MMC_APP_CMD, 0, 0, MSHC_CMDAT_RESPONSE_R1);
	resp = mshc_cmd_send(SD_APP_OP_COND, arg, 0, MSHC_CMDAT_RESPONSE_R3);

	return resp;
}

void mshc_sd_select_card(unsigned short rca)
{
	mshc_cmd_send(MMC_SELECT_CARD, rca << 16, 0, MSHC_CMDAT_RESPONSE_R1);
}

unsigned int mshc_sd_highcap_chk(unsigned short rca)
{
	unsigned char *resp;

	resp = mshc_sd_get_csd(rca);

	if((resp[15] & 0xc0) >> 6) {
		printf("===>::card is highcap.\n");
		return 1;
	}

	return 0;
}

void mshc_sd_chg_bus_width(unsigned short rca, unsigned int buswidth)
{
	unsigned int val;

	/* disable card interrupt */
	val = mshc_readw(MSHC_BASE, MSHC_NORMAL_INT_STAT_EN_R);
	val &= ~ MSHC_CARD_INTERRUPT_STAT_EN_BIT;
#ifdef USE_SDIO
	val &= ~ MSHC_CARD_INTERRUPT_STAT_EN_BIT; //??
#endif
	mshc_writew(MSHC_BASE, val, MSHC_NORMAL_INT_STAT_EN_R);


#ifdef USE_SDIO
	mshc_cmd_send(52, 0, 0, MSHC_CMDAT_RESPONSE_R1);//??
#else
	mshc_cmd_send(MMC_APP_CMD, rca << 16, 0, MSHC_CMDAT_RESPONSE_R1);
	mshc_cmd_send(SD_APP_SET_BUS_WIDTH, buswidth, 0, MSHC_CMDAT_RESPONSE_R1);
#endif

	mshc_host1_dat_xfer_width_setup(buswidth);

#ifdef USE_SDIO
	mshc_cmd_send(MMC_APP_CMD, rca << 16, 0, MSHC_CMDAT_RESPONSE_R1);
	mshc_cmd_send(SD_APP_SET_BUS_WIDTH, buswidth, 0, MSHC_CMDAT_RESPONSE_R1);
#endif

}

void mshc_sd_chg_bus_speedmode(unsigned int speedmode)
{
	unsigned char *resp;
	unsigned int val;
	unsigned int arg;

	/* cmd6 arg bits desc:
	 * 0 ~ 3: access mode
	 * 7 ~ 4: command system
	 * 8 ~ 11: driver length
	 * 12 ~ 15: power limit
	 * 31 : mode0/mode1 = 0/1
	 */

	/* chg bus speedmode issue CMD6 with mode1 */
	arg |= (0x1 << 31) | 0x00ffffff;
	arg &= ~0xf;
	arg |= speedmode;
	resp = mshc_cmd_send(SD_SWITCH, arg, 0, MSHC_CMDAT_RESPONSE_R1);

	/* check issue CMD6 with mode0 */
	arg &= ~(0x1 << 31);
	resp = mshc_cmd_send(SD_SWITCH, arg, 0, MSHC_CMDAT_RESPONSE_R1);

	/* host controller set speedmode */
	mshc_host2_uhs_mode_sel(speedmode);
}

unsigned int mshc_singl_voltage_switch(unsigned int s18r, unsigned int voltage)
{
	unsigned char* resp;

	if(!(s18r | voltage))
		return -1;

	printf("------%s-----%d-------voltage=%d\n", __func__, __LINE__, voltage);
	resp = mshc_cmd_send(SD_SWITCH_VOLTAGE, 0, 0, MSHC_CMDAT_RESPONSE_R1);


	if(resp[2] & 0x80)  //19
		goto stop;

	mshc_clk_sd_clk_disable();

	udelay(1000);
	if(mshc_check_data_line(0))
		goto stop;

/*	mshc_host2_1V8_signaling_enable();*/
	mshc_voltage_set(voltage);

	udelay(7000);

	mshc_clk_sd_clk_enable();

	udelay(3000);

	if(mshc_check_data_line(15))
		goto stop;

	printf("------%s==>signal voltage switch OK\n", __func__);
	return 0;
stop:
	printf("signal voltage switch fail!!!\n");
	mshc_voltage_set(1);

}


void mshc_sync_abort()
{
	u32 timeout = 1000;

	mshc_writeb(MSHC_BASE, MSHC_STOP_BG_REQ_BIT, MSHC_BGAP_CTRL_R);

	wait_xfer_complete();

	mshc_cmd_send(MMC_STOP_TRANSMISSION, 0, MSHC_CMD_TYPE_ABORT_CMD, MSHC_CMDAT_RESPONSE_R1b);

	mshc_reset(MSHC_SW_RST_CMD_BIT);
	mshc_reset(MSHC_SW_RST_DAT_BIT);
}

unsigned int mshc_sd_init()
{
	unsigned char *resp;
	unsigned int val;
	unsigned short rca;
	unsigned int err;
	unsigned int timeout = 1000;
	unsigned int s18r = 0;

#ifdef MSHC_VOLT_SWOTCH_180
	s18r = 1;
#endif

/*	mshc_sync_abort();*/
	mshc_cmd_send(MMC_GO_IDLE_STATE, 0, 0, MSHC_CMDAT_RESPONSE_NONE);

	err = mshc_cmd_sd_send_if_cond(1);
	if(err)
		printf("cmd8 err ...");

op_cond:
	resp = mshc_cmd_app_op_cond(0xff8000, s18r, 0, 1);
	while(!(resp[4] & (0x1 << 7)) && --timeout){
		udelay(10000);
		goto op_cond;
	}

	if(resp[4] & (0x1 << 6))
		printf("card ccs = 1, highcap\n");

	if(resp[4] & 0x1)
		printf("card s18r = 1, 1.8v\n");

	mshc_singl_voltage_switch(resp[4] & 0x1, MSHC_BUS_VOL_1V8);

	if(!timeout)
		printf("SD Init Fail!\n");
	else
		printf("SD Init Ok!\n");

	rca = mshc_sd_get_rca();

	return rca;
}
