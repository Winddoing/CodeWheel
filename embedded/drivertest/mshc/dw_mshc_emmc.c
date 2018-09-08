#include <stdio.h>
#include <mshc/dw_mshc.h>


void mshc_emmc_conf_ext_csd(unsigned index, unsigned int ext_csd_value)
{
	unsigned int switch_arg;

	/**
	 * CMD6 -- SWITCH arg desc
	 * [31:26] Set to 0
	 * [25:24] Access
	 * [23:16] Index
	 * [15:8] Value
	 * [7:3] Set to 0
	 * [2:0] Cmd Set
	 */

	/**
	 * Access
	 * 00 ----  command set
	 * 01 ----  set bits
	 * 10 ----  clear bits
	 * 11 ----  write byte
	 */
	switch_arg |= 0x3 << 24;
	switch_arg |= index << 16;
	switch_arg |= ext_csd_value << 8;
	switch_arg |= 0x0;

	mshc_cmd_send(MMC_SWITCH, switch_arg, 0x0, MSHC_CMDAT_RESPONSE_R1);
}

void mshc_emmc_set_bus_with(unsigned int buswidth)
{
	unsigned switch_arg;

	switch(buswidth) {
		case MSHC_BUS_WIDTH_1:
			switch_arg = 0x3 << 24 | 183 << 16 | 0 << 8 | 0x0;  // 1-bit mode
			break;
		case MSHC_BUS_WIDTH_4:
			switch_arg = 0x3 << 24 | 183 << 16 | 1 << 8 | 0x0;  // 4-bit mode
			break;
		case MSHC_BUS_WIDTH_8:
			switch_arg = 0x3 << 24 | 183 << 16 | 2 << 8 | 0x0;  // 8-bit mode
			break;
		default:
			switch_arg = 0x3 << 24 | 183 << 16 | 0 << 8 | 0x0;  // 1-bit mode
			break;
	}
	mshc_cmd_send(MMC_SWITCH, switch_arg, 0x0, MSHC_CMDAT_RESPONSE_R1);
}

void mshc_emmc_chg_bus_width(unsigned int buswidth)
{
	mshc_emmc_set_bus_with(buswidth);
	mshc_host1_dat_xfer_width_setup(buswidth);
}

void mshc_emmc_chg_bus_speedmode()
{

}

void mshc_emmc_set_rca(unsigned int rca)
{
	mshc_cmd_send(MMC_ALL_SEND_CID, 0, 0, MSHC_CMDAT_RESPONSE_R2);
	mshc_cmd_send(SD_SEND_RELATIVE_ADDR, rca << 16, 0, MSHC_CMDAT_RESPONSE_R6);
}


unsigned char* mshc_cmd_send_op_cond(unsigned int vol, unsigned int s18r, unsigned int access_mode)
{
	unsigned char *resp;
	unsigned int arg;
	unsigned int timeout = 1000;


	/**
	 * cmd 1 argument
	 * 7		1.70~1.95V
	 * 14:8		2.0~2.6V
	 * 23:15	2.7~3.6V
	 * 30:29	Access_mode
	 *	00 --- byte mode
	 *	10 --- sector mode
	 */
	arg |= access_mode << 30;
	arg |= s18r << 7;
	arg |= vol & 0xffffff;

	resp = mshc_cmd_send(MMC_SEND_OP_COND, arg, 0, MSHC_CMDAT_RESPONSE_R3);

	return resp;
}
unsigned int mshc_emmc_init()
{
	unsigned char* resp;
	unsigned int timeout = 1000;

	mshc_cmd_send(MMC_GO_IDLE_STATE, 0, 0, MSHC_CMDAT_RESPONSE_NONE);
op_cond:
	resp = mshc_cmd_send_op_cond(0xff8000, 0, 1);
	while(!(resp[4] & 0x80) && --timeout){
		udelay(1000);
		goto op_cond;
	}

	if(timeout)
		return 0;
	else
		return -1;
}
