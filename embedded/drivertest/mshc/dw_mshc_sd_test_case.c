#include <stdio.h>
#include <mshc/dw_mshc.h>
#include <mshc/dw_mshc_test.h>
#include <mshc/dw_mshc_sd_test.h>

#include <dma_default.h>

#include <CUnit.h>
#include <TestDB.h>
#include <Console.h>
#include <Basic.h>

#include <CU_module.h>

unsigned int rca;
unsigned int highcap;

void sd_init()
{
	unsigned int err;

	/* host init*/
	err = mshc_host_init();
	if(!err)
		printf("mshc host sd init ok ....\n");

	/* card init */
	rca = mshc_sd_init();

	highcap = mshc_sd_highcap_chk(rca);

	mshc_sd_select_card(rca);
}

void sd_pio_1_low_sigblk_tran()
{
	int msg, ret;

	sd_init();
	mshc_lowclk_enable();
	mshc_sd_chg_bus_width(rca, MSHC_BUS_WIDTH_1);
	ret = pio_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void sd_pio_1_high_sigblk_tran()
{
	int msg, ret;

	sd_init();
	mshc_highclk_enable();
	mshc_sd_chg_bus_width(rca, MSHC_BUS_WIDTH_1);
	ret = pio_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void sd_pio_4_low_sigblk_tran()
{
	int msg, ret;

	sd_init();
	mshc_lowclk_enable();
	mshc_sd_chg_bus_width(rca, MSHC_BUS_WIDTH_4);
	ret = pio_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void sd_pio_4_high_sigblk_tran()
{
	int msg, ret;

	sd_init();
	mshc_highclk_enable();
	mshc_sd_chg_bus_width(rca, MSHC_BUS_WIDTH_4);
	ret = pio_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void sd_pio_1_low_mulblk_tran()
{
	int msg, ret;

	sd_init();
	mshc_lowclk_enable();
	mshc_sd_chg_bus_width(rca, MSHC_BUS_WIDTH_1);
	ret = pio_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void sd_pio_1_high_mulblk_tran()
{
	int msg, ret;

	sd_init();
	mshc_highclk_enable();
	mshc_sd_chg_bus_width(rca, MSHC_BUS_WIDTH_1);
	ret = pio_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void sd_pio_4_low_mulblk_tran()
{
	int msg, ret;

	sd_init();
	mshc_lowclk_enable();
	mshc_sd_chg_bus_width(rca, MSHC_BUS_WIDTH_4);
	ret = pio_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void sd_pio_4_high_mulblk_tran()
{
	int msg, ret;

	sd_init();
	mshc_highclk_enable();
	mshc_sd_chg_bus_width(rca, MSHC_BUS_WIDTH_4);
	ret = pio_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void sd_sdma_1_low_sigblk_tran()
{
	int msg, ret;

	sd_init();
	mshc_lowclk_enable();
	mshc_sd_chg_bus_width(rca, MSHC_BUS_WIDTH_1);
	ret = sdma_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void sd_sdma_1_high_sigblk_tran()
{
	int msg, ret;

	sd_init();
	mshc_highclk_enable();
	mshc_sd_chg_bus_width(rca, MSHC_BUS_WIDTH_1);
	ret = sdma_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void sd_sdma_4_low_sigblk_tran()
{
	int msg, ret;

	sd_init();
	mshc_lowclk_enable();
	mshc_sd_chg_bus_width(rca, MSHC_BUS_WIDTH_4);
	ret = sdma_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void sd_sdma_4_high_sigblk_tran()
{
	int msg, ret;

	sd_init();
	mshc_highclk_enable();
	mshc_sd_chg_bus_width(rca, MSHC_BUS_WIDTH_4);
	ret = sdma_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void sd_sdma_1_low_mulblk_tran()
{
	int msg, ret;

	sd_init();
	mshc_lowclk_enable();
	mshc_sd_chg_bus_width(rca, MSHC_BUS_WIDTH_1);
	ret = sdma_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void sd_sdma_1_high_mulblk_tran()
{
	int msg, ret;

	sd_init();
	mshc_highclk_enable();
	mshc_sd_chg_bus_width(rca, MSHC_BUS_WIDTH_1);
	ret = sdma_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void sd_sdma_4_low_mulblk_tran()
{
	int msg, ret;

	sd_init();
	mshc_lowclk_enable();
	mshc_sd_chg_bus_width(rca, MSHC_BUS_WIDTH_4);
	ret = sdma_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void sd_sdma_4_high_mulblk_tran()
{
	int msg, ret;

	sd_init();
	mshc_highclk_enable();
	mshc_sd_chg_bus_width(rca, MSHC_BUS_WIDTH_4);
	ret = sdma_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void sd_adma2_1_low_sigblk_tran()
{
	int msg, ret;

	sd_init();
	mshc_lowclk_enable();
	mshc_sd_chg_bus_width(rca, MSHC_BUS_WIDTH_1);
	ret = adma2_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void sd_adma2_1_high_sigblk_tran()
{
	int msg, ret;

	sd_init();
	mshc_highclk_enable();
	mshc_sd_chg_bus_width(rca, MSHC_BUS_WIDTH_1);
	ret = adma2_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void sd_adma2_4_low_sigblk_tran()
{
	int msg, ret;

	sd_init();
	mshc_lowclk_enable();
	mshc_sd_chg_bus_width(rca, MSHC_BUS_WIDTH_4);
	ret = adma2_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void sd_adma2_4_high_sigblk_tran()
{
	int msg, ret;

	sd_init();
	mshc_highclk_enable();
	mshc_sd_chg_bus_width(rca, MSHC_BUS_WIDTH_4);
	ret = adma2_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void sd_adma2_1_low_mulblk_tran()
{
	int msg, ret;

	sd_init();
	mshc_lowclk_enable();
	mshc_sd_chg_bus_width(rca, MSHC_BUS_WIDTH_1);
	ret = adma2_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void sd_adma2_1_high_mulblk_tran()
{
	int msg, ret;

	sd_init();
	mshc_highclk_enable();
	mshc_sd_chg_bus_width(rca, MSHC_BUS_WIDTH_1);
	ret = adma2_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void sd_adma2_4_low_mulblk_tran()
{
	int msg, ret;

	sd_init();
	mshc_lowclk_enable();
	mshc_sd_chg_bus_width(rca, MSHC_BUS_WIDTH_4);
	ret = adma2_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void sd_adma2_4_high_mulblk_tran()
{
	int msg, ret;

	sd_init();
	mshc_highclk_enable();
	mshc_sd_chg_bus_width(rca, MSHC_BUS_WIDTH_4);
	ret = adma2_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void sd_adma3_1_low_sigblk_tran()
{
	int msg, ret;

	sd_init();
	mshc_host2_ver4_enable();
	mshc_lowclk_enable();
	mshc_sd_chg_bus_width(rca, MSHC_BUS_WIDTH_1);
	ret = adma3_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void sd_adma3_1_high_sigblk_tran()
{
	int msg, ret;

	sd_init();
	mshc_host2_ver4_enable();
	mshc_highclk_enable();
	mshc_sd_chg_bus_width(rca, MSHC_BUS_WIDTH_1);
	ret = adma3_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void sd_adma3_4_low_sigblk_tran()
{
	int msg, ret;

	sd_init();
	mshc_host2_ver4_enable();
	mshc_lowclk_enable();
	mshc_sd_chg_bus_width(rca, MSHC_BUS_WIDTH_4);
	ret = adma3_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void sd_adma3_4_high_sigblk_tran()
{
	int msg, ret;

	sd_init();
	mshc_host2_ver4_enable();
	mshc_highclk_enable();
	mshc_sd_chg_bus_width(rca, MSHC_BUS_WIDTH_4);
	ret = adma3_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void sd_adma3_1_low_mulblk_tran()
{
	int msg, ret;

	sd_init();
	mshc_host2_ver4_enable();
	mshc_lowclk_enable();
	mshc_sd_chg_bus_width(rca, MSHC_BUS_WIDTH_1);
	ret = adma3_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void sd_adma3_1_high_mulblk_tran()
{
	int msg, ret;

	sd_init();
	mshc_host2_ver4_enable();
	mshc_highclk_enable();
	mshc_sd_chg_bus_width(rca, MSHC_BUS_WIDTH_1);
	ret = adma3_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void sd_adma3_4_low_mulblk_tran()
{
	int msg, ret;

	sd_init();
	mshc_host2_ver4_enable();
	mshc_lowclk_enable();
	mshc_sd_chg_bus_width(rca, MSHC_BUS_WIDTH_4);
	ret = adma3_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void sd_adma3_4_high_mulblk_tran()
{
	int msg, ret;

	sd_init();
	mshc_host2_ver4_enable();
	mshc_highclk_enable();
	mshc_sd_chg_bus_width(rca, MSHC_BUS_WIDTH_4);
	ret = adma3_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}


