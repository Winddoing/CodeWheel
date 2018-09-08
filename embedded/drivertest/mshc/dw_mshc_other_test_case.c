#include <stdio.h>
#include <mshc/dw_mshc.h>
#include <mshc/dw_mshc_test.h>
#include <mshc/dw_mshc_other.h>

#include <dma_default.h>

#include <CUnit.h>
#include <TestDB.h>
#include <Console.h>
#include <Basic.h>

#include <CU_module.h>


void mshc_test_host_version()
{
	printf("\n\n================ normal use reset reg status ==================\n");
	mshc_reset(MSHC_SW_RST_ALL_BIT);

	dump_reg();

	printf("\n\n============================ set HV4=1 =========================\n");
	mshc_host2_ver4_enable();

	dump_reg();
	printf("\n\n========== Host version 4 enable   reset reg status============\n");
	mshc_reset(MSHC_SW_RST_ALL_BIT);

	dump_reg();

	printf("\n\n==================== start test  ============================\n");
	printf("================ set reg and HV4=1 ==========================\n");
	mshc_host2_ver4_enable();
/*	mshc_int_mask();*/
	mshc_writew(MSHC_BASE, 0xf, MSHC_XFER_MODE_R);
/*	mshc_writeb(MSHC_BASE, 0xff, MSHC_HOST_CTRL1_R);*/
/*	mshc_writew(MSHC_BASE, 0x1 << 13, MSHC_HOST_CTRL2_R);*/
/*	mshc_writew(MSHC_BASE, 0xefff, MSHC_HOST_CTRL2_R);*/
/*	mshc_writel(MSHC_BASE, 0xffffffff, MSHC_ADMA_SA_LOW_R);*/
/*	mshc_writel(MSHC_BASE, 0xffffffff, MSHC_SDMASA_R);*/
/*	mshc_writew(MSHC_BASE, 0xffff, MSHC_BLOCKCOUNT_R);*/
/*	mshc_writew(MSHC_BASE, 0xffff, MSHC_CLK_CTRL_R);*/
/*	mshc_writew(MSHC_BASE, 0x1 << 3, MSHC_CLK_CTRL_R);*/
	mshc_writew(MSHC_BASE, 0x1 << 3 | 0x1, MSHC_CLK_CTRL_R);

	dump_reg();
	printf("\n\n================ set reg CLK_CTRL_R reset after status ===================\n");

	mshc_reset(MSHC_SW_RST_ALL_BIT);

	dump_reg();

}

void mshc_test_clk_pll_reset_ok()
{
/*	mshc_host2_ver4_enable();*/

/*	mshc_writew(MSHC_BASE, 0xffff, MSHC_CLK_CTRL_R); // OK*/
/*	mshc_writew(MSHC_BASE, 0xf, MSHC_CLK_CTRL_R);    //OK*/
/*	mshc_writew(MSHC_BASE, 0x1 << 3 | 0x1 << 2, MSHC_CLK_CTRL_R); //clk_ctrl_r=0xe*/
/*	mshc_writew(MSHC_BASE, 0x1 << 3 | 0x1 << 1, MSHC_CLK_CTRL_R); //clk_ctrl_r=0xa*/
/*	mshc_writew(MSHC_BASE, 0x1 << 3 | 0x1, MSHC_CLK_CTRL_R); //OK*/
	mshc_writew(MSHC_BASE, 0x1 << 3, MSHC_CLK_CTRL_R); //fail

	printf("\n\n====================== reset =====================\n");
	mshc_reset(MSHC_SW_RST_ALL_BIT);

	printf("mshc reset after, MSHC_CLK_CTRL_R: %08x\n",  mshc_readw(MSHC_BASE, MSHC_CLK_CTRL_R));

}


void mshc_emmc_cqe_reg()
{
	int i;

	printf("\nmshc CQE register test: MSHC_CQTDBR\n");
	mshc_reset(MSHC_SW_RST_ALL_BIT);

	mshc_emmc_cqe_enable();
	for(i=0; i<32; i++) {
		printf("test%02d CQTBR write 0x%08x", i, BIT(i));

		mshc_writel(MSHC_CQE_BASE, BIT(i), MSHC_CQTDBR);
		printf("\tread MSHC_CQTDBR: 0x%08x\n", mshc_readl(MSHC_CQE_BASE, MSHC_CQTDBR));
	}

}

void mshc_other()
{
	unsigned int val;

	mshc_reset(MSHC_SW_RST_ALL_BIT);

	printf("MSHC_HOST_CTRL2_R: %08x\n", mshc_readw(MSHC_BASE, MSHC_HOST_CTRL2_R));
	val = mshc_readw(MSHC_BASE, MSHC_HOST_CTRL2_R);
	printf("val=%08x\n", val);
/*	val |= MSHC_SAMPLE_CLK_SEL_BIT;*/
	mshc_writew(MSHC_BASE, 0xffff, MSHC_HOST_CTRL2_R);

	printf("MSHC_HOST_CTRL2_R: %08x\n", mshc_readw(MSHC_BASE, MSHC_HOST_CTRL2_R));
}

void mshc_hardware_voltage_1V8_test()
{
	printf("MSC IO hardware voltage is 1V8 ......\n");

	do{
		printf("------ 1V8 ------\n");
		mshc_voltage_set(MSHC_BUS_VOL_1V8);
		udelay(100);
		printf("------ 3V3 ------\n");
		mshc_voltage_set(MSHC_BUS_VOL_3V3);
		udelay(100);
	}while(1);
}
