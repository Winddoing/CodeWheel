#include <stdio.h>
#include <task_module.h>
#include <mshc/dw_mshc_sd_test.h>
#include <mshc/dw_mshc_emmc_test.h>
#include <mshc/dw_mshc_other.h>

#include <CUnit.h>
#include <TestDB.h>
#include <Console.h>
#include <Basic.h>
#include <CU_module.h>

/* SD test case */
static TM_TestInfo  TM_sd_pio[] = {
	{"pio_1_low_sigblk_tran",	sd_pio_1_low_sigblk_tran, TM_ACTIVE_YES},
	{"pio_1_high_sigblk_tran",	sd_pio_1_high_sigblk_tran, TM_ACTIVE_YES},
	{"pio_4_low_sigblk_tran",	sd_pio_4_low_sigblk_tran, TM_ACTIVE_YES},
	{"pio_4_high_sigblk_tran",	sd_pio_4_high_sigblk_tran, TM_ACTIVE_YES},
	{"pio_1_low_mulblk_tran",	sd_pio_1_low_mulblk_tran, TM_ACTIVE_YES},
	{"pio_1_high_mulblk_tran",	sd_pio_1_high_mulblk_tran, TM_ACTIVE_YES},
	{"pio_4_low_mulblk_tran",	sd_pio_4_low_mulblk_tran, TM_ACTIVE_YES},
	{"pio_4_high_mulblk_tran",	sd_pio_4_high_mulblk_tran, TM_ACTIVE_YES},
	TM_TEST_INFO_NULL,
};

static TM_TestInfo  TM_sd_sdma[] = {
	{"sdma_1_low_sigblk_tran",	sd_sdma_1_low_sigblk_tran, TM_ACTIVE_YES},
	{"sdma_1_high_sigblk_tran", sd_sdma_1_high_sigblk_tran, TM_ACTIVE_YES},
	{"sdma_4_low_sigblk_tran",	sd_sdma_4_low_sigblk_tran, TM_ACTIVE_YES},
	{"sdma_4_high_sigblk_tran",	sd_sdma_4_high_sigblk_tran, TM_ACTIVE_YES},
	{"sdma_1_low_mulblk_tran",	sd_sdma_1_low_mulblk_tran, TM_ACTIVE_YES},
	{"sdma_1_high_mulblk_tran",	sd_sdma_1_high_mulblk_tran, TM_ACTIVE_YES},
	{"sdma_4_low_mulblk_tran",	sd_sdma_4_low_mulblk_tran, TM_ACTIVE_YES},
	{"sdma_4_high_mulblk_tran",	sd_sdma_4_high_mulblk_tran, TM_ACTIVE_YES},
	TM_TEST_INFO_NULL,
};

static TM_TestInfo  TM_sd_adma2[] = {
	{"adma2_1_low_sigblk_tran",	 sd_adma2_1_low_sigblk_tran, TM_ACTIVE_YES},
	{"adma2_1_high_sigblk_tran", sd_adma2_1_high_sigblk_tran, TM_ACTIVE_YES},
	{"adma2_4_low_sigblk_tran",  sd_adma2_4_low_sigblk_tran, TM_ACTIVE_YES},
	{"adma2_4_high_sigblk_tran", sd_adma2_4_high_sigblk_tran, TM_ACTIVE_YES},
	{"adma2_1_low_mulblk_tran",  sd_adma2_1_low_mulblk_tran, TM_ACTIVE_YES},
	{"adma2_1_high_mulblk_tran", sd_adma2_1_high_mulblk_tran, TM_ACTIVE_YES},
	{"adma2_4_low_mulblk_tran",  sd_adma2_4_low_mulblk_tran, TM_ACTIVE_YES},
	{"adma2_4_high_mulblk_tran", sd_adma2_4_high_mulblk_tran, TM_ACTIVE_YES},
	TM_TEST_INFO_NULL,
};

static TM_TestInfo  TM_sd_adma3[] = {
	{"adma3_1_low_sigblk_tran",	 sd_adma3_1_low_sigblk_tran, TM_ACTIVE_YES},
	{"adma3_1_high_sigblk_tran", sd_adma3_1_high_sigblk_tran, TM_ACTIVE_YES},
	{"adma3_4_low_sigblk_tran",  sd_adma3_4_low_sigblk_tran, TM_ACTIVE_YES},
	{"adma3_4_high_sigblk_tran", sd_adma3_4_high_sigblk_tran, TM_ACTIVE_YES},
	{"adma3_1_low_mulblk_tran",  sd_adma3_1_low_mulblk_tran, TM_ACTIVE_YES},
	{"adma3_1_high_mulblk_tran", sd_adma3_1_high_mulblk_tran, TM_ACTIVE_YES},
	{"adma3_4_low_mulblk_tran",  sd_adma3_4_low_mulblk_tran, TM_ACTIVE_YES},
	{"adma3_4_high_mulblk_tran", sd_adma3_4_high_mulblk_tran, TM_ACTIVE_YES},
	TM_TEST_INFO_NULL,
};



/* emmc test case */
static TM_TestInfo  TM_emmc_pio[] = {
	{"pio_1_low_sigblk_tran",	emmc_pio_1_low_sigblk_tran, TM_ACTIVE_YES},
	{"pio_1_high_sigblk_tran",	emmc_pio_1_high_sigblk_tran, TM_ACTIVE_YES},
	{"pio_4_low_sigblk_tran",	emmc_pio_4_low_sigblk_tran, TM_ACTIVE_YES},
	{"pio_4_high_sigblk_tran",	emmc_pio_4_high_sigblk_tran, TM_ACTIVE_YES},
	{"pio_1_low_mulblk_tran",	emmc_pio_1_low_mulblk_tran, TM_ACTIVE_YES},
	{"pio_1_high_mulblk_tran",	emmc_pio_1_high_mulblk_tran, TM_ACTIVE_YES},
	{"pio_4_low_mulblk_tran",	emmc_pio_4_low_mulblk_tran, TM_ACTIVE_YES},
	{"pio_4_high_mulblk_tran",	emmc_pio_4_high_mulblk_tran, TM_ACTIVE_YES},
	TM_TEST_INFO_NULL,
};

static TM_TestInfo  TM_emmc_sdma[] = {
	{"sdma_1_low_sigblk_tran",  emmc_sdma_1_low_sigblk_tran, TM_ACTIVE_YES},
	{"sdma_1_high_sigblk_tran", emmc_sdma_1_high_sigblk_tran, TM_ACTIVE_YES},
	{"sdma_4_low_sigblk_tran",  emmc_sdma_4_low_sigblk_tran, TM_ACTIVE_YES},
	{"sdma_4_high_sigblk_tran", emmc_sdma_4_high_sigblk_tran, TM_ACTIVE_YES},
	{"sdma_1_low_mulblk_tran",  emmc_sdma_1_low_mulblk_tran, TM_ACTIVE_YES},
	{"sdma_1_high_mulblk_tran", emmc_sdma_1_high_mulblk_tran, TM_ACTIVE_YES},
	{"sdma_4_low_mulblk_tran",  emmc_sdma_4_low_mulblk_tran, TM_ACTIVE_YES},
	{"sdma_4_high_mulblk_tran", emmc_sdma_4_high_mulblk_tran, TM_ACTIVE_YES},
	TM_TEST_INFO_NULL,
};

static TM_TestInfo  TM_emmc_adma2[] = {
	{"adma2_1_low_sigblk_tran",  emmc_adma2_1_low_sigblk_tran, TM_ACTIVE_YES},
	{"adma2_1_high_sigblk_tran", emmc_adma2_1_high_sigblk_tran, TM_ACTIVE_YES},
	{"adma2_4_low_sigblk_tran",  emmc_adma2_4_low_sigblk_tran, TM_ACTIVE_YES},
	{"adma2_4_high_sigblk_tran", emmc_adma2_4_high_sigblk_tran, TM_ACTIVE_YES},
	{"adma2_1_low_mulblk_tran",  emmc_adma2_1_low_mulblk_tran, TM_ACTIVE_YES},
	{"adma2_1_high_mulblk_tran", emmc_adma2_1_high_mulblk_tran, TM_ACTIVE_YES},
	{"adma2_4_low_mulblk_tran",  emmc_adma2_4_low_mulblk_tran, TM_ACTIVE_YES},
	{"adma2_4_high_mulblk_tran", emmc_adma2_4_high_mulblk_tran, TM_ACTIVE_YES},
	TM_TEST_INFO_NULL,
};

static TM_TestInfo  TM_emmc_adma3[] = {
	{"adma3_1_low_sigblk_tran",  emmc_adma3_1_low_sigblk_tran, TM_ACTIVE_YES},
	{"adma3_1_high_sigblk_tran", emmc_adma3_1_high_sigblk_tran, TM_ACTIVE_YES},
	{"adma3_4_low_sigblk_tran",  emmc_adma3_4_low_sigblk_tran, TM_ACTIVE_YES},
	{"adma3_4_high_sigblk_tran", emmc_adma3_4_high_sigblk_tran, TM_ACTIVE_YES},
	{"adma3_1_low_mulblk_tran",  emmc_adma3_1_low_mulblk_tran, TM_ACTIVE_YES},
	{"adma3_1_high_mulblk_tran", emmc_adma3_1_high_mulblk_tran, TM_ACTIVE_YES},
	{"adma3_4_low_mulblk_tran",  emmc_adma3_4_low_mulblk_tran, TM_ACTIVE_YES},
	{"adma3_4_high_mulblk_tran", emmc_adma3_4_high_mulblk_tran, TM_ACTIVE_YES},
	TM_TEST_INFO_NULL,
};

static TM_TestInfo  TM_emmc_cqe[] = {
	{"cqe_1_low_sigblk_tran",  emmc_cqe_1_low_sigblk_tran, TM_ACTIVE_YES},
	{"cqe_1_high_sigblk_tran", emmc_cqe_1_high_sigblk_tran, TM_ACTIVE_YES},
	{"cqe_4_low_sigblk_tran",  emmc_cqe_4_low_sigblk_tran, TM_ACTIVE_YES},
	{"cqe_4_high_sigblk_tran", emmc_cqe_4_high_sigblk_tran, TM_ACTIVE_YES},
	{"cqe_1_low_mulblk_tran",  emmc_cqe_1_low_mulblk_tran, TM_ACTIVE_YES},
	{"cqe_1_high_mulblk_tran", emmc_cqe_1_high_mulblk_tran, TM_ACTIVE_YES},
	{"cqe_4_low_mulblk_tran",  emmc_cqe_4_low_mulblk_tran, TM_ACTIVE_YES},
	{"cqe_4_high_mulblk_tran", emmc_cqe_4_high_mulblk_tran, TM_ACTIVE_YES},
	TM_TEST_INFO_NULL,
};

static TM_TestInfo  TM_mshc_other[] = {
	{"Other Test",					  mshc_other, TM_ACTIVE_YES},
	{"test host version 4 relevant",  mshc_test_host_version, TM_ACTIVE_YES},
	{"mshc_test_clk_pll_reset_ok",    mshc_test_clk_pll_reset_ok, TM_ACTIVE_YES},
	{"test cqe register CQTDBR",	  mshc_emmc_cqe_reg, TM_ACTIVE_YES},
	{"test hardware voltage 1V8 switch", mshc_hardware_voltage_1V8_test, TM_ACTIVE_YES},
	TM_TEST_INFO_NULL,
};

static TM_SuiteInfo TM_suites[] = {
	{"sd.pio", NULL, NULL, NULL, NULL, TM_sd_pio, TM_ACTIVE_YES},
	{"sd.sdma", NULL, NULL, NULL, NULL, TM_sd_sdma, TM_ACTIVE_YES},
	{"sd.adma2", NULL, NULL, NULL, NULL, TM_sd_adma2, TM_ACTIVE_YES},
	{"sd.adma3", NULL, NULL, NULL, NULL, TM_sd_adma3, TM_ACTIVE_YES},

	{"emmc.pio", NULL, NULL, NULL, NULL, TM_emmc_pio, TM_ACTIVE_YES},
	{"emmc.sdma", NULL, NULL, NULL, NULL, TM_emmc_sdma, TM_ACTIVE_YES},
	{"emmc.adma2", NULL, NULL, NULL, NULL, TM_emmc_adma2, TM_ACTIVE_YES},
	{"emmc.adma3", NULL, NULL, NULL, NULL, TM_emmc_adma3, TM_ACTIVE_YES},
	{"emmc.cqe", NULL, NULL, NULL, NULL, TM_emmc_cqe, TM_ACTIVE_YES},

	{"other", NULL, NULL, NULL, NULL, TM_mshc_other, TM_ACTIVE_YES},

	TM_SUITE_INFO_NULL,
};

static int __init mshc_tests_init (void){
	return TM_register_suites(TM_suites);
}
module_init(mshc_tests_init);


