#include <stdio.h>
#include <mshc/dw_mshc.h>

#define MAX_TASK_SLOT	(32)

#define SET_BIT(n) BIT(n)
#define CLR_BIT(n) (0)

void pre_cmd_for_solt(cqe_cmd_desc_info_t tdl_cmd_info)
{
	unsigned short attr;
	unsigned short cmd;

	attr = ATTR_DEFAULT;
	attr |= tdl_cmd_info.inter;
	attr |= tdl_cmd_info.qbr_bit;

	cmd = tdl_cmd_info.cmd_index;
	cmd |= tdl_cmd_info.cmd_timing;
	cmd |= tdl_cmd_info.reps_type;

	tdl_cmd_info.cmd_task_desc_t_p = (cmd_task_desc_t *)&tdl_cmd_info.task_desc_slot_p[MAX_TASK_SLOT-1];

	tdl_cmd_info.cmd_task_desc_t_p->cmd_field_attr = attr;
	tdl_cmd_info.cmd_task_desc_t_p->cmd_com_param = cmd;
	tdl_cmd_info.cmd_task_desc_t_p->cmd_argument = tdl_cmd_info.cmd_arg;
}

void pre_task_for_slot(cqe_tdl_desc_info_t tdl_info)
{
	unsigned short param_attr;

	param_attr = tdl_info.task_attr;
	param_attr |= tdl_info.inter;
	param_attr |= tdl_info.fp_bit;
	param_attr |= tdl_info.context_id;
	param_attr |= tdl_info.tag_req_bit;
	param_attr |= tdl_info.data_dir_bit;
	param_attr |= tdl_info.priority_bit;
	param_attr |= tdl_info.qbr_bit;
	param_attr |= tdl_info.rel_write_bit;

	tdl_info.task_desc_slot_p[tdl_info.id].data_desc.param_attr = param_attr;
	tdl_info.task_desc_slot_p[tdl_info.id].data_desc.block_count = tdl_info.blkcnt;
	tdl_info.task_desc_slot_p[tdl_info.id].data_desc.block_addr = tdl_info.cardaddr;
#ifdef DEBUG
	printf("_%s_addr=%08x\n", __func__,  &tdl_info.task_desc_slot_p[tdl_info.id]);
	printf("--%d-->0x%08x\n", tdl_info.id, *((unsigned int *)&tdl_info.task_desc_slot_p[tdl_info.id]));
	printf("--%d-->0x%08x\n", tdl_info.id, *((unsigned int *)&tdl_info.task_desc_slot_p[tdl_info.id] + 1));
#endif
}

void pre_data_for_slot(cqe_tdl_desc_info_t tdl_info)
{
	unsigned int length, attr;

	length = tdl_info.length << 16;     //length 16bit
	attr = tdl_info.tran_attr;

	tdl_info.task_desc_slot_p[tdl_info.id].data.addr = tdl_info.buf;
	tdl_info.task_desc_slot_p[tdl_info.id].data.attr = length | attr;
}

void creat_tdl_item(tdl_slot_t *task_desc_slot_p, int id, unsigned int *buf, int opsmode,
		unsigned int cardaddr, unsigned int blksize, unsigned short blkcnt)
{
	cqe_tdl_desc_info_t tdl_info;
	cqe_cmd_desc_info_t tdl_cmd_info;

	unsigned int context = 0; //default

	tdl_info.id = id;
	/* task descriptor structure */
	tdl_info.task_attr = ATTR_DEFAULT;
	tdl_info.inter = SET_BIT(TDA_INT);
	tdl_info.fp_bit = SET_BIT(TDP_FP);
	tdl_info.context_id = context << TDP_CONTEXT_ID;
	tdl_info.tag_req_bit = CLR_BIT(TDP_TAG);
	if(MSHC_DATA_XFER_DIR_RD == opsmode)
		tdl_info.data_dir_bit = SET_BIT(TDP_DATA_DIR);
	else
		tdl_info.data_dir_bit = CLR_BIT(TDP_DATA_DIR);
	tdl_info.priority_bit = CLR_BIT(TDP_PRIORITY);
	tdl_info.qbr_bit = CLR_BIT(TDP_QBR);
	tdl_info.rel_write_bit = SET_BIT(TDP_RELIABLE_W);
	tdl_info.blkcnt = blkcnt;
	tdl_info.cardaddr = cardaddr;

	/* transfer descriptor structure */
	tdl_info.tran_attr = MSHC_ADMA2_DESC_TRAN | MSHC_ADMA2_DESC_VALID_END;
	tdl_info.length = blkcnt*blksize;
	tdl_info.buf = virt_to_phys((void *)buf);

	/* task descriptor for direct command */
	tdl_cmd_info.inter = SET_BIT(TDA_INT);
	tdl_cmd_info.qbr_bit = CLR_BIT(TDP_QBR);
	tdl_cmd_info.cmd_index = MMC_STOP_TRANSMISSION;
	tdl_cmd_info.cmd_timing = SET_BIT(CMD_TIMING);
	tdl_cmd_info.reps_type = MSHC_CMDAT_RESPONSE_R1 << RESP_TYPE;
	tdl_cmd_info.cmd_arg = 0;
	tdl_cmd_info.task_desc_slot_p = task_desc_slot_p;

	tdl_info.task_desc_slot_p = task_desc_slot_p;

	pre_task_for_slot(tdl_info);
	pre_data_for_slot(tdl_info);
/*	pre_cmd_for_solt(tdl_cmd_info);*/
}
unsigned int create_tdl(unsigned int *bufaddr_w, unsigned int *bufaddr_r, unsigned int cardaddr, unsigned int blksize,
		unsigned int blkcnt)
{
	tdl_slot_t task_desc_slot[MAX_TASK_SLOT];
	tdl_slot_t *task_desc_slot_p;

	unsigned int TDL_ADDR = 0xac400000;
	memset((void *)TDL_ADDR, 0, sizeof(task_desc_slot));
	task_desc_slot_p = (tdl_slot_t *)TDL_ADDR;


/*	task_desc_slot_p = (tdl_slot_t *)_ALIGH((unsigned int)task_desc_slot | 0xa0000000, sizeof(tdl_slot_t)*8);*/
/*	memset(task_desc_slot_p, 0, sizeof(tdl_slot_t)*MAX_TASK_SLOT);*/

	creat_tdl_item(task_desc_slot_p, 0, bufaddr_w, MSHC_DATA_XFER_DIR_WR, cardaddr, blksize, blkcnt);
	creat_tdl_item(task_desc_slot_p, 1, bufaddr_r, MSHC_DATA_XFER_DIR_RD, cardaddr, blksize, blkcnt);

#ifdef DEBUG
	dump_emmc_cqe_desc(task_desc_slot_p);
#endif

	return virt_to_phys((void*)task_desc_slot_p);
}

void mshc_emmc_init_cqe(unsigned int cqe_rca)
{
	mshc_emmc_cqe_config_timeout();
	mshc_emmc_cqe_enable_inter();
	mshc_emmc_cqe_set_rca(cqe_rca);
	mshc_emmc_cqe_config_dcmd(MSHC_DCMD_DATA);
	mshc_emmc_cqe_config_task_size(MSHC_TASK_DESC_SIZE_64);
	mshc_emmc_cqe_enable();
}

int do_wait_cqe_complete_int()
{
	int err;

	printf("====>:%s\n", __func__);
#ifdef MSHC_USE_INT
	err = mshc_emmc_cqe_task_complete_int_s();
#else
	err = mshc_emmc_cqe_task_complete_int();
#endif

	if(err)
		printf("emmc cqe task complete fail!!!!\n");

	return err;
}

void mshc_emmc_cqe_complete_task(unsigned int tdl_id)
{
	int err, tdl_index;

/*	mshc_emmc_check_slots();*/
	mshc_emmc_submit_one_task(tdl_id);

	mdelay(30);
#ifdef DEBUG
	dump_error_status();
	dump_emmc_cqe_tran();
#endif

	err = do_wait_cqe_complete_int();
	if(err)
		goto task_fail;

	tdl_index = mshc_emmc_get_task_complete_notified();

	printf("-------------tdl_index=%d-------id=%d\n", tdl_index, tdl_id);
	mshc_emmc_clear_slot_complete(tdl_index);
	mshc_normal_int_cqe_event_detect();

	return 0;

task_fail:
	printf("emmc:: report task faill!!!!\n");
/*	while(1);*/
}

int mshc_emmc_cqe_tran(unsigned int opsmode, unsigned int blksize, unsigned int blkcnt)
{
	mshc_data_pre(opsmode, MSHC_DMA_ENABLE, blksize, blkcnt);
	mshc_xfer_blkcnt_disable();

	mshc_emmc_cqe_complete_task(opsmode);
}

unsigned int mshc_cqe_transfer(unsigned int *bufaddr_w, unsigned int *bufaddr_r, unsigned int cqe_rca,
		unsigned int cardaddr, unsigned int blksize, unsigned int blkcnt)
{
	unsigned int addr;

	printf("\n===>>%s----cqe data transfer start!!!\n", __func__);
	addr = create_tdl(bufaddr_w, bufaddr_r, cardaddr, blksize, blkcnt);
	printf("========addr=%08x\n", addr);
	mshc_emmc_cqe_cqtdlba_setup(addr);

	mshc_emmc_init_cqe(cqe_rca);

	mshc_emmc_cqe_tran(MSHC_DATA_XFER_DIR_WR, blksize, 0);

	udelay(1000);

	printf("--------------------wqshao---------------------------read\n");
	mshc_emmc_cqe_tran(MSHC_DATA_XFER_DIR_RD, blksize, 0);
	printf("===>>%s----cqe data transfer end!!!\n\n", __func__);

	return 0;
}
