#include <stdio.h>
#include <mshc/dw_mshc.h>

void dump_reg()
{
	//state
	printf("PSTATE_REG					= %08x\n", mshc_readl(MSHC_BASE, MSHC_PSTATE_REG));
	printf("NORMAL_INT_SIGNAL_EN_R		= %08x\n", mshc_readw(MSHC_BASE, MSHC_NORMAL_INT_SIGNAL_EN_R));
	printf("NORMAL_INT_STAT_EN_R		= %08x\n", mshc_readw(MSHC_BASE, MSHC_NORMAL_INT_STAT_EN_R));
	printf("NORMAL_INT_STAT_R			= %08x\n", mshc_readw(MSHC_BASE, MSHC_NORMAL_INT_STAT_R));
	printf("ERROR_INT_SIGNAL_EN_R		= %08x\n", mshc_readw(MSHC_BASE, MSHC_ERROR_INT_SIGNAL_EN_R));
	printf("ERROR_INT_STAT_EN_R			= %08x\n", mshc_readw(MSHC_BASE, MSHC_ERROR_INT_STAT_EN_R));
	printf("ERROR_INT_STAT_R			= %08x\n", mshc_readw(MSHC_BASE, MSHC_ERROR_INT_STAT_R));

	//clk
	printf("CAPABILITIES1_R		= %08x\n", mshc_readl(MSHC_BASE, MSHC_CAPABILITIES1_R));//[23:16] 00
	printf("CAPABILITIES2_R		= %08x\n", mshc_readl(MSHC_BASE, MSHC_CAPABILITIES2_R));//[15:8] c8--200MHZ ?
	printf("PRESET_INIT_R		= %08x\n", mshc_readw(MSHC_BASE, MSHC_PRESET_INIT_R));
	printf("CLK_CTRL_R			= %08x\n", mshc_readw(MSHC_BASE, MSHC_CLK_CTRL_R));

	printf("HOST_CTRL1_R		= %08x\n", mshc_readb(MSHC_BASE, MSHC_HOST_CTRL1_R));
	printf("HOST_CTRL2_R		= %08x\n", mshc_readw(MSHC_BASE, MSHC_HOST_CTRL2_R));
	printf("PWR_CTRL_R			= %08x\n", mshc_readb(MSHC_BASE, MSHC_PWR_CTRL_R));

	printf("XFER_MODE_R			= %08x\n", mshc_readw(MSHC_BASE, MSHC_XFER_MODE_R));
	printf("ARGUMENT_R			= %08x\n", mshc_readl(MSHC_BASE, MSHC_ARGUMENT_R));
	printf("CMD_R				= %08x\n", mshc_readw(MSHC_BASE, MSHC_CMD_R));
	printf("TOUT_CTRL_R			= %08x\n", mshc_readb(MSHC_BASE, MSHC_TOUT_CTRL_R));
	printf("BLOCKCOUNT_R		= %08x\n", mshc_readw(MSHC_BASE, MSHC_BLOCKCOUNT_R));
	printf("BLOCKSIZE_R			= %08x\n", mshc_readw(MSHC_BASE, MSHC_BLOCKSIZE_R));

	printf("RESP67_R			= %08x\n", mshc_readl(MSHC_BASE, MSHC_RESP67_R));
	printf("RESP45_R			= %08x\n", mshc_readl(MSHC_BASE, MSHC_RESP45_R));
	printf("RESP23_R			= %08x\n", mshc_readl(MSHC_BASE, MSHC_RESP23_R));
	printf("RESP01_R			= %08x\n", mshc_readl(MSHC_BASE, MSHC_RESP01_R));

	printf("SDMASA_R			= %08x\n", mshc_readl(MSHC_BASE, MSHC_SDMASA_R));
	printf("ADMA_ID_LOW_R  = %08x\n", mshc_readl(MSHC_BASE, MSHC_ADMA_ID_LOW_R));
}

void dump_error_status()
{
	printf("PSTATE_REG				= %08x\n", mshc_readl(MSHC_BASE, MSHC_PSTATE_REG));
	printf("XFER_MODE_R				= %08x\n", mshc_readw(MSHC_BASE, MSHC_XFER_MODE_R));
	printf("NORMAL_INT_STAT_EN_R	= %08x\n", mshc_readw(MSHC_BASE, MSHC_NORMAL_INT_STAT_EN_R));
	printf("NORMAL_INT_STAT_R   	= %08x\n", mshc_readw(MSHC_BASE, MSHC_NORMAL_INT_STAT_R));
	printf("ERROR_INT_STAT_EN_R 	= %08x\n", mshc_readw(MSHC_BASE, MSHC_ERROR_INT_STAT_EN_R));
	printf("ERROR_INT_STAT_R    	= %08x\n", mshc_readw(MSHC_BASE, MSHC_ERROR_INT_STAT_R));
	printf("ADMA_ERR_STAT_R     	= %08x\n", mshc_readw(MSHC_BASE, MSHC_ADMA_ERR_STAT_R));
	printf("HOST_CTRL1_R        	= %08x\n", mshc_readb(MSHC_BASE, MSHC_HOST_CTRL1_R));
	printf("HOST_CTRL2_R        	= %08x\n", mshc_readw(MSHC_BASE, MSHC_HOST_CTRL2_R));
	printf("MSHC_TOUT_CTRL_R       	= %08x\n", mshc_readb(MSHC_BASE, MSHC_TOUT_CTRL_R));
	printf("MSHC_CLK_CTRL_R         = %08x\n", mshc_readw(MSHC_BASE, MSHC_CLK_CTRL_R));
	printf("clock freq   :  [%s]\n", mshc_readl(MSHC_CLK_CTRL_SOURCE, 0) & MSC_CLK_H_FREQ ? "high" : "low");
}

void dump_adma3_desc(adma3_integrated_desc_t *integrated_desc_p)
{
	int i, j;
	unsigned int *addr_tmp;

	printf("wqshao__integrated_desc_head_addr=%08x-------content=%08x\n", integrated_desc_p,
			*((unsigned int*)integrated_desc_p));
	for(i=0; i<10; i++)
	{
		printf("%d___====%08x----:%08x\n",i, integrated_desc_p+i, *((unsigned int*)integrated_desc_p+i));
	}

	addr_tmp = (unsigned int *)phys_to_virt(integrated_desc_p[0].one_desc_addr);
	addr_tmp = MY_UNCAC_ADDR(addr_tmp);
	printf("adma3_desc_head_addr=%08x\n", addr_tmp);
	for(j=0; j<31; j++){
		printf("==============%08x-->:%08x\n",addr_tmp+j, *((unsigned int*)addr_tmp + j));
	}
	printf("\n");
}

void dump_emmc_cqe_desc(tdl_slot_t *task_desc_slot_p)
{
	int j;
	for(j=0; j<128; j++)
	{
		printf("=====%02d===addr=%08x=:0x%08x\n", j/4, (unsigned int*)task_desc_slot_p+j,
				*((unsigned int*)task_desc_slot_p+j));
		printf("          addr=%08x=:0x%08x\n", (unsigned int*)task_desc_slot_p+(j+1),
				*((unsigned int*)task_desc_slot_p+(j+1)));
		printf("          addr=%08x=:0x%08x\n", (unsigned int*)task_desc_slot_p+(j+2),
				*((unsigned int*)task_desc_slot_p+(j+2)));
		printf("          addr=%08x=:0x%08x\n", (unsigned int*)task_desc_slot_p+(j+3),
				*((unsigned int*)task_desc_slot_p+(j+3)));
		j=j+3;
	}
}

void dump_adma3_tran()
{
	printf("ADMA_ID_LOW_R  = %08x\n", mshc_readl(MSHC_BASE, MSHC_ADMA_ID_LOW_R));
	printf("ADMA_SA_LOW_R  = %08x\n", mshc_readl(MSHC_BASE, MSHC_ADMA_SA_LOW_R));
}

void dump_emmc_cqe_tran()
{
	printf("MSHC_CQTDBR=0x%08x\n", mshc_readl(MSHC_CQE_BASE, MSHC_CQTDBR));
	printf("MSHC_CQCFG =0x%08x\n", mshc_readl(MSHC_CQE_BASE, MSHC_CQCFG));
	printf("device stata-------MSHC_CQDPT=0x%08x\n", mshc_readl(MSHC_CQE_BASE, MSHC_CQDPT));
	printf("device stata-------MSHC_CQDQS=0x%08x\n", mshc_readl(MSHC_CQE_BASE, MSHC_CQDQS));

	printf("--------->>>>:MSHC_CQTERRI=0x%08x\n", mshc_readl(MSHC_CQE_BASE, MSHC_CQTERRI));
	printf("--------->>>>:MSHC_CQCRI  =0x%08x\n", mshc_readl(MSHC_CQE_BASE, MSHC_CQCRI));
	printf("--------->>>>:MSHC_CQIS   =0x%08x\n", mshc_readl(MSHC_CQE_BASE, MSHC_CQIS));

	printf("xxxxxxxx=MSHC_CQTCN  =0x%08x\n", mshc_readl(MSHC_CQE_BASE, MSHC_CQTCN));
	printf("xxxxxxxx=MSHC_CQRMEM =0x%08x\n", mshc_readl(MSHC_CQE_BASE, MSHC_CQRMEM));

	printf("device stata-------MSHC_CQDPT=0x%08x\n", mshc_readl(MSHC_CQE_BASE, MSHC_CQDPT));
	printf("device stata-------MSHC_CQDQS=0x%08x\n", mshc_readl(MSHC_CQE_BASE, MSHC_CQDQS));

}

void dump_response()
{
	printf("RESP67_R = %08x\n", mshc_readl(MSHC_BASE, MSHC_RESP67_R));
	printf("RESP45_R = %08x\n", mshc_readl(MSHC_BASE, MSHC_RESP45_R));
	printf("RESP23_R = %08x\n", mshc_readl(MSHC_BASE, MSHC_RESP23_R));
	printf("RESP01_R = %08x\n", mshc_readl(MSHC_BASE, MSHC_RESP01_R));
}

void dump_cmd_cfg()
{
	printf("SDMASA_R			= %08x\n", mshc_readl(MSHC_BASE, MSHC_SDMASA_R));
	printf("BLOCKSIZE_R			= %08x\n", mshc_readw(MSHC_BASE, MSHC_BLOCKSIZE_R));
	printf("BLOCKCOUNT_R		= %08x\n", mshc_readw(MSHC_BASE, MSHC_BLOCKCOUNT_R));
	printf("ARGUMENT_R			= %08x\n", mshc_readl(MSHC_BASE, MSHC_ARGUMENT_R));
	printf("XFER_MODE_R			= %08x\n", mshc_readw(MSHC_BASE, MSHC_XFER_MODE_R));
	printf("CMD_R				= %08x\n", mshc_readw(MSHC_BASE, MSHC_CMD_R));
}

int check_WR_data(unsigned int *src, unsigned int *dst, unsigned int len)
{
	unsigned int i, e_count = 0;

	printf("===>>%s----check data start!!!\n", __func__);
	printf("\tbus width  : %d-bit\n", mshc_readb(MSHC_BASE, MSHC_HOST_CTRL1_R) & MSHC_DAT_XFER_WIDTH_BIT ? 4 : 1);
	printf("\tclock freq : %s\n", mshc_readl(MSHC_CLK_CTRL_SOURCE, 0) & MSC_CLK_H_FREQ ? "high" : "low");
	printf("check src = 0x%08x, dst = 0x%08x, length = %d Byte\n",src, dst, len);

	for(i=0; i < len/4; i++){
/*		printf("src_w[%d] = %x, dst_r[%d] = %x\n", i, src[i], i, dst[i]);*/
		udelay(10);
		if(src[i] != dst[i]){
			printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX src_w[%d] = 0x%08x, dst_r[%d] = 0x%08x\n",
					i, src[i], i, dst[i]);
			e_count++;
		}
	}
#ifdef DEBUG_DUMP_READ_DATA
	for(i=0; i<512; i++)
	{
		printf("%.8x ", dst[i]);
		if(!((i+1)%8))
			printf("\n");

	}
	printf("\n");
#endif

	if(0 != e_count){
		printf("check error data length: %d Byte !!!!!!\n", e_count * 4);
		return -1;
	}

	printf("***********************************************\n");
	printf("******        write and read OK          ******\n");
	printf("***********************************************\n");

	return 0;
}


