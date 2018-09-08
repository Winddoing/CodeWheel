#include <stdio.h>
#include <mshc/dw_mshc.h>
#include <mshc/dw_mshc_test.h>
#include <mshc/dw_mshc_emmc_test.h>

#include <dma_default.h>

#include <CUnit.h>
#include <TestDB.h>
#include <Console.h>
#include <Basic.h>

#include <CU_module.h>

unsigned int rca = 0xabc;
unsigned int highcap = 1;

void emmc_init()
{
	unsigned int err;

	unsigned char *resp;
	/* host init*/
	err = mshc_host_init();
	if(!err)
		printf("mshc host init ok ....\n");

/*	mshc_voltage_set(MSHC_BUS_VOL_1V8);*/

	/* card init */
	err = mshc_emmc_init();
	if(!err)
		printf("emmc card init ok!!!!\n");

	mshc_emmc_set_rca(rca);
/*	mshc_sd_highcap_chk(rca);*/

	mshc_sd_select_card(rca);
}

void emmc_pio_1_low_sigblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_lowclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_1);
	ret = pio_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_pio_1_high_sigblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_highclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_1);
	ret = pio_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_pio_4_low_sigblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_lowclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_4);
	ret = pio_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_pio_4_high_sigblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_highclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_4);
	ret = pio_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_pio_1_low_mulblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_lowclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_1);
	ret = pio_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_pio_1_high_mulblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_highclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_1);
	ret = pio_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_pio_4_low_mulblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_lowclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_4);
	ret = pio_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_pio_4_high_mulblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_highclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_4);
	ret = pio_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_sdma_1_low_sigblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_lowclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_1);
	ret = sdma_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_sdma_1_high_sigblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_highclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_1);
	ret = sdma_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_sdma_4_low_sigblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_lowclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_4);
	ret = sdma_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_sdma_4_high_sigblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_highclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_4);
	ret = sdma_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_sdma_1_low_mulblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_lowclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_1);
	ret = sdma_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_sdma_1_high_mulblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_highclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_1);
	ret = sdma_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_sdma_4_low_mulblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_lowclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_4);
	ret = sdma_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_sdma_4_high_mulblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_highclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_4);
	ret = sdma_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_adma2_1_low_sigblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_lowclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_1);
	ret = adma2_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_adma2_1_high_sigblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_highclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_1);
	ret = adma2_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_adma2_4_low_sigblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_lowclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_4);
	ret = adma2_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_adma2_4_high_sigblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_highclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_4);
	ret = adma2_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_adma2_1_low_mulblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_lowclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_1);
	ret = adma2_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_adma2_1_high_mulblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_highclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_1);
	ret = adma2_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_adma2_4_low_mulblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_lowclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_4);
	ret = adma2_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_adma2_4_high_mulblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_highclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_4);
	ret = adma2_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_adma3_1_low_sigblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_host2_ver4_enable();
	mshc_lowclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_1);
	ret = adma3_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_adma3_1_high_sigblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_host2_ver4_enable();
	mshc_highclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_1);
	ret = adma3_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_adma3_4_low_sigblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_host2_ver4_enable();
	mshc_lowclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_4);
	ret = adma3_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_adma3_4_high_sigblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_host2_ver4_enable();
	mshc_highclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_4);
	ret = adma3_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_adma3_1_low_mulblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_host2_ver4_enable();
	mshc_lowclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_1);
	ret = adma3_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_adma3_1_high_mulblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_host2_ver4_enable();
	mshc_highclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_1);
	ret = adma3_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_adma3_4_low_mulblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_host2_ver4_enable();
	mshc_lowclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_4);
	ret = adma3_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_adma3_4_high_mulblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_host2_ver4_enable();
	mshc_highclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_4);
	ret = adma3_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_cqe_1_low_sigblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_emmc_conf_ext_csd(MSHC_CMDQ_MODE_EN, MSHC_CQE_ENABLE);
	mshc_lowclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_1);
	ret = cqe_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_cqe_1_high_sigblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_emmc_conf_ext_csd(MSHC_CMDQ_MODE_EN, MSHC_CQE_ENABLE);
	mshc_highclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_1);
	ret = cqe_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_cqe_4_low_sigblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_emmc_conf_ext_csd(MSHC_CMDQ_MODE_EN, MSHC_CQE_ENABLE);
	mshc_lowclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_4);
	ret = cqe_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_cqe_4_high_sigblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_emmc_conf_ext_csd(MSHC_CMDQ_MODE_EN, MSHC_CQE_ENABLE);
	mshc_highclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_4);
	ret = cqe_transfer_sigblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_cqe_1_low_mulblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_emmc_conf_ext_csd(MSHC_CMDQ_MODE_EN, MSHC_CQE_ENABLE);
	mshc_lowclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_1);
	ret = cqe_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_cqe_1_high_mulblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_emmc_conf_ext_csd(MSHC_CMDQ_MODE_EN, MSHC_CQE_ENABLE);
	mshc_highclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_1);
	ret = cqe_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_cqe_4_low_mulblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_emmc_conf_ext_csd(MSHC_CMDQ_MODE_EN, MSHC_CQE_ENABLE);
	mshc_lowclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_4);
	ret = cqe_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}

void emmc_cqe_4_high_mulblk_tran()
{
	int msg, ret;

	emmc_init();
	mshc_emmc_conf_ext_csd(MSHC_CMDQ_MODE_EN, MSHC_CQE_ENABLE);
	mshc_highclk_enable();
	mshc_emmc_chg_bus_width(MSHC_BUS_WIDTH_4);
	ret = cqe_transfer_mulblk_blk();

	if(!ret) {
		CU_PASS(msg);
	} else {
		CU_FAIL(msg);
	}
}


