#ifndef _DW_MSHC_H_
#define _DW_MSHC_H_

#include <includes.h>
#include <jz_gpio.h>
#include <io.h>
#include <jz_intc.h>

#include <mshc/mmc.h>
#include <mshc/sd.h>
#include <mshc/dw_mshc_regs.h>
#include <mshc/dw_mshc_config.h>

#include <dma_default.h>

#define CPM_MSC0_CLK_R			(0xB0000068) //CPM MSC0 device clk Only on switch
#define CPM_MSC1_CLK_R			(0xB00000a4) //CPM MSC1 device clk Only on switch

#ifdef MSHC_4785_TEST
#define MSHC_BASE				(0xB3050000)
#define MSC_CLK_H_FREQ			(0x1 << 15)
#else
  #if defined(MSHC_TEST_X2000_MSHC1)
	#define MSHC_BASE			(0xB3460000)
    #define MSHC_CLK_CTRL_SOURCE		CPM_MSC1_CLK_R
  #else
	#define MSHC_BASE			(0xB3450000)
    #define MSHC_CLK_CTRL_SOURCE		CPM_MSC0_CLK_R
  #endif
  #define MSC_CLK_H_FREQ		(0x1 << 20)
#endif

#define MSHC_GET_CMD(c) ((c>>8) & 0x3f)

#define MSHC_EMBEDDED_BASE				\
	((mshc_readw(MSHC_BASE, MSHC_P_EMBEDDED_CNTRL) & MSHC_EMBEDDED_CNTRL_REG_OFFSET_ADDR_MASK) + MSHC_BASE)
#define MSHC_CQE_BASE					\
	((mshc_readw(MSHC_BASE, MSHC_P_VENDOR2_SPECIFIC_AREA) & MSHC_CQE_REG_OFFSET_ADDR_MASK) + MSHC_BASE)
#define MSHC_VENDOR_BASE				\
	((mshc_readw(MSHC_BASE, MSHC_P_VENDOR_SPECIFIC_AREA) & MSHC_VENDOR_SPECIFIC_REG_OFFSET_ADDR_MASK) + MSHC_BASE)

#define MSHC_CMDAT_RESPONSE_NONE	(0x0) /* No response */
#define MSHC_CMDAT_RESPONSE_R1		(0x1) /* Format R1 , R3, R6, R7 length 48 */
#define MSHC_CMDAT_RESPONSE_R1b		(0x2) /* Format R1b length 48 */
#define MSHC_CMDAT_RESPONSE_R2		(0x3) /* Format R2 length 136 */
#define MSHC_CMDAT_RESPONSE_R3		(0x4) /* Format R3 length 48 */
#define MSHC_CMDAT_RESPONSE_R4		(0x5) /* Format R4 length 48 */
#define MSHC_CMDAT_RESPONSE_R5		(0x6) /* Format R5 length 48 */
#define MSHC_CMDAT_RESPONSE_R6		(0x7) /* Format R6 length 48 */
#define MSHC_CMDAT_RESPONSE_R7		(0x8) /* Format R7 length 48 */

/* ADMA2 */
#define MSHC_ADMA2_DESC_NOP          (0x0 << 3)
#define MSHC_ADMA2_DESC_TRAN         (0x4 << 3)
#define MSHC_ADMA2_DESC_LINK         (0x6 << 3)

#define MSHC_ADMA2_DESC_VALID        (0x1 << 0)
#define MSHC_ADMA2_DESC_END          (0x1 << 1)
#define MSHC_ADMA2_DESC_VALID_END    MSHC_ADMA2_DESC_VALID | MSHC_ADMA2_DESC_END
#define MSHC_ADMA2_DESC_INT          (0x1 << 2)


/* emmc EXT_CSD */
#define MSHC_CMDQ_MODE_EN			(15)
#define MSHC_CQE_ENABLE				(1)
#define MSHC_CQE_DISENABLE			(0)

/* bus width */
#define MSHC_BUS_WIDTH_1			0
#define MSHC_BUS_WIDTH_4			2
#define MSHC_BUS_WIDTH_8			8


#define MSHC_BUS_VOL_1V8			0
#define MSHC_BUS_VOL_3V3			1

#define mshc_readb(ctrl, offset)	\
	readb(ctrl + offset)
#define mshc_writeb(ctrl, val, offset)	\
	writeb((val), ctrl + offset)
#define mshc_readw(ctrl, offset)	\
	readw(ctrl + offset)
#define mshc_writew(ctrl, val, offset)	\
	writew((val), ctrl + offset)
#define mshc_readl(ctrl, offset)	\
	readl(ctrl + offset)
#define mshc_writel(ctrl, val, offset)	\
	writel((val), ctrl + offset)

#define _ALIGH(x, align) (((x) + (align - 1)) & (~(align - 1)))
#define MY_UNCAC_ADDR(addr) ((unsigned int)((unsigned int)(addr) | 0xa0000000))

/* dma desc */
typedef union adma2_attr {
	unsigned int d32;
	struct {
		unsigned attr:6;
		unsigned len_10:10;
		unsigned len_16:16;
	} b;
} adma2_attr_t;

typedef union adma3_attr {
	unsigned int d32;
	struct {
		unsigned attr:6;
		unsigned rsd6_15:10;
		unsigned rsd16_31:16;
	} b;
} adma3_attr_t;

typedef struct adma2_desc {
	adma2_attr_t attr;
	unsigned int addr;
} adma2_desc_t;

typedef struct cmd_desc_item {
	adma3_attr_t attr;
#define CMD_HAVE_DATA		(0x9)
#define CMD_NOT_HAVE_DATA	(0xb)
	unsigned int reg;
} cmd_desc_item_t;

typedef struct cmd_desc {
	cmd_desc_item_t block_count;
	cmd_desc_item_t block_size;
#define BLKCNT_HIGH_16  16
#define BLKSIE_LOW_16	0
	cmd_desc_item_t argument;
	cmd_desc_item_t cmd_tran_mode;
#define CMD_HIGH_16		16
#define XFER_LOW_16		0
} cmd_desc_t;

typedef struct adma3_desc {
	cmd_desc_t one_cmd_desc;
	adma2_desc_t one_data_desc;
} adma3_desc_t;

typedef struct adma3_integrated_desc {
	adma3_attr_t attr;
#define DATA_TRAN	(0x39)
#define DATA_END	(0x3b)
	unsigned int one_desc_addr;
} adma3_integrated_desc_t;

typedef struct adma3_info{
	int id;
	unsigned int index;
	unsigned int is_rw;;
	unsigned int argument;
	unsigned int block_count;
	unsigned int block_size;
	unsigned int no_data;
#define CMD_NO_DATA   0
#define CMD_DATA      1

	unsigned int adma2_attr_act;
	unsigned int adma2_attr_IVE;

	unsigned char* buf;

	unsigned int is_end;
#define ADMA3_NO_END 0
#define ADMA3_END  1
	struct adma3_desc *desc_p;
}adma3_conf_info;

/* CQE desc */

typedef struct data_task_desc{
	unsigned short param_attr;
#define ATTR_DEFAULT	(0x2b)
#define TDA_INT			(2) //Task Desc Attr
#define TDP_FP			(6) //Task Desc Attr
#define TDP_CONTEXT_ID  (7)        //10:7 --4bit
#define TDP_TAG         (11)
#define TDP_DATA_DIR    (12)
#define TDP_PRIORITY    (13)
#define TDP_QBR         (14)
#define TDP_RELIABLE_W  (15)
	unsigned short block_count;
	unsigned int block_addr;
#ifdef EMMC_TASK_DESC_SIZE_128BIT
	unsigned int reserved;
	unsigned int vendor_spec;
#endif
}data_task_desc_t;

/*32bit addressing mode*/
typedef struct tran_desc{
	unsigned int attr;
	unsigned int addr;
} tran_desc_t;

typedef struct cmd_task_desc{
	unsigned short cmd_field_attr;
	unsigned short cmd_com_param;
#define CMD_TIMING  (22 - 16)
#define RESP_TYPE	(23 - 16)
	unsigned int cmd_argument;
} cmd_task_desc_t;

typedef struct tdl_slot{
	struct data_task_desc data_desc;
	struct tran_desc data;
} tdl_slot_t;

typedef struct cqe_tdl_desc_info{
	int id;

	/* task descriptor structure */
	unsigned int task_attr;
	unsigned int inter;
	unsigned int fp_bit;
	unsigned int context_id;
	unsigned int tag_req_bit;
	unsigned int data_dir_bit;
	unsigned int priority_bit;
	unsigned int qbr_bit;
	unsigned int rel_write_bit;
	unsigned int blkcnt;
	unsigned int cardaddr;

	/* transfer descriptor structure */
	unsigned int tran_attr;
	unsigned int length;
	unsigned int *buf;


	tdl_slot_t *task_desc_slot_p;
} cqe_tdl_desc_info_t;

typedef struct cqe_cmd_desc_info{

	/* task descriptor for direct command */
	unsigned int inter;
	unsigned int qbr_bit;
	unsigned short cmd_index;
	unsigned short cmd_timing;
	unsigned short reps_type;
	unsigned int cmd_arg;

	tdl_slot_t *task_desc_slot_p;
	cmd_task_desc_t *cmd_task_desc_t_p;
} cqe_cmd_desc_info_t;
/* function declaration*/

/* host function */
void mshc_host1_card_detect_sig_lvl_test();
void mshc_host1_card_detect_sig_lvl_normal();
void mshc_host1_card_detect_test_lvl_card_insert();
void mshc_host1_card_detect_test_lvl_no_card();
void mshc_host1_ext_dat_xfer_width_8();
void mshc_host1_ext_dat_xfer_width_default();
void mshc_host1_dma_sel(unsigned int type);
void mshc_host1_high_speed_enable();
void mshc_host1_normal_speed_enable();
void mshc_host1_dat_xfer_width_setup(unsigned int buswidth);
void mshc_pwr_vdd1_setup(unsigned int voltage);
void mshc_pwr_vdd1_on();
void mshc_pwr_vdd1_off();
void mshc_clk_freq_sel(unsigned int value);
void mshc_clk_upper_freq_sel(unsigned int value);
void mshc_clk_gen_sel_div_clk_mode();
void mshc_clk_gen_sel_prog_clk_mode();
void mshc_clk_pll_enable();
void mshc_clk_pll_low_power_mode();
void mshc_gpio_clk_enable();
void mshc_clk_sd_clk_enable();
void mshc_clk_sd_clk_disable();
void mshc_gpio_clk_disable();
unsigned int mshc_check_data_line(unsigned int mark);
void mshc_clk_internal_clk_enable();
void mshc_clk_internal_clk_disable();
void mshc_tout_cnt_setup(unsigned int value);
void mshc_sw_reset_dat();
void mshc_sw_reset_cmd();
void mshc_sw_reset_all();
void mshc_sw_reset_clear();
void mshc_normal_int_cqe_event_no();
void mshc_normal_int_cqe_event_detect();
void mshc_normal_int_card_remove();
void mshc_normal_int_card_remove_stable();
void mshc_normal_int_card_insert();
void mshc_normal_int_card_insert_stable();
void mshc_normal_int_buf_rd_ready();
void mshc_normal_int_buf_rd_ready_s();
void mshc_normal_int_buf_wr_ready();
void mshc_normal_int_buf_wr_ready_s();
void mshc_normal_int_buf_wr_not_ready();
void mshc_normal_int_dma_no_interrupt();
void mshc_normal_int_dma_interrupt_generated();
void mshc_normal_int_no_bgap_event();
void mshc_normal_int_bgap_event_stop();
void mshc_normal_int_xfer_complete();
void mshc_normal_int_xfer_complete_s();
void mshc_normal_int_cmd_complete();
void mshc_normal_int_cmd_complete_s();
void mshc_normal_int_en_card_remove();
void mshc_normal_int_en_card_insert();
void mshc_normal_int_sig_en_card_remove();
void mshc_normal_int_sig_en_card_insert();
void mshc_err_int_adma_err_clear();
void mshc_err_int_adma_err_clear_s();
void mshc_host2_preset_val_enable();
void mshc_host2_uhs_mode_sel(unsigned int speedmode);
void mshc_host2_ver4_enable();
void mshc_host2_4_bit_interface_enable();
void mshc_host2_adma2_len_sel(unsigned int mode);
void mshc_host2_adma_addr_len_sel(unsigned int len);
/*unsigned int mshc_get_host_support_voltage();*/
/*void mshc_host2_1V8_signaling_enable();*/
/*unsigned int mshc_host2_check_1V8_signaling();*/
void mshc_emmc_card_is_emmc();
void mshc_timeout_setup(unsigned int khz, unsigned int freq, unsigned int value);
void mshc_gpio_init();
void mshc_reset();
void mshc_voltage_set(int level);
unsigned int mshc_single_voltage_switch(unsigned int s18r, unsigned int voltage);
void mshc_lowclk_enable();
void mshc_highclk_enable();
unsigned int mshc_card_detect();
void mshc_clk_setup(unsigned int freq);
void mshc_card_clk_supply();
void mshc_card_clk_stop();
void mshc_sd_clk_freq_chg(unsigned int speedmode, unsigned int freq);
void mshc_host_sd_setup();
void mshc_host_emmc_setup();
void mshc_card_interface_detect();
unsigned int mshc_host_init();

/* device fuction */
unsigned short mshc_sd_get_rca();
unsigned char *mshc_sd_get_ocr(unsigned int arg);
unsigned char *mshc_sd_get_cid(unsigned short rca);
unsigned char *mshc_sd_get_csd(unsigned short rca);
unsigned int mshc_cmd_sd_send_if_cond(unsigned int vol);
unsigned char *mshc_cmd_app_op_cond(unsigned int vol, unsigned int s18r, unsigned int xpc, unsigned int hcs);
void mshc_sd_select_card(unsigned short rca);
unsigned int mshc_sd_highcap_chk(unsigned short rca);
void mshc_sd_chg_bus_width(unsigned short rca, unsigned int buswidth);
void mshc_sd_chg_bus_speedmode(unsigned int speedmode);
unsigned int mshc_sd_init();
unsigned int mshc_emmc_init();
void mshc_emmc_set_rca(unsigned int rca);
void mshc_emmc_chg_bus_width(unsigned int buswidth);
void mshc_emmc_conf_ext_csd(unsigned index, unsigned int ext_csd_value);

/* transfer set function */
void mshc_sdma_sa_setup(unsigned int addr);
void mshc_blksize_setup(unsigned int blksize);
void mshc_blksize_sdma_boundary_setup(unsigned int boundary);
void mshc_blkcnt_setup(unsigned int blkcnt);
void mshc_argument_setup(unsigned int arg);
void mshc_xfer_resp_int_enable();
void mshc_xfer_resp_int_disable();
void mshc_xfer_resp_err_chk_enable();
void mshc_xfer_resp_err_chk_disable();
void mshc_xfer_resp_type_setup(unsigned int type);
void mshc_xfer_sigblk_setup();
void mshc_xfer_mulblk_setup();
void mshc_xfer_mode_setup(unsigned int opsmode);
void mshc_xfer_auto_cmd_enable_setup(unsigned int cmd);
void mshc_xfer_blkcnt_enable();
void mshc_xfer_blkcnt_disable();
void mshc_xfer_dma_enable();
void mshc_xfer_pio_enable();
void mshc_data_pre(int ops, int is_dma, unsigned int blksize, unsigned int blkcnt);
void mshc_cmd_index_setup(unsigned int index);
void mshc_cmd_type_setup(unsigned int type);
void mshc_cmd_present_data_select(unsigned int sel);
void mshc_cmd_idx_chk_enable();
void mshc_cmd_idx_chk_disable();
void mshc_cmd_crc_chk_enable();
void mshc_cmd_crc_chk_disable();
void mshc_cmd_sub_cmd_flag_setup(unsigned int flag);
void mshc_cmd_resp_type_select(unsigned int type);
void mshc_adma_sa_low_setup(unsigned int addr);
void mshc_adma_id_low_setup(unsigned int addr);
void mshc_emmc_cqe_cqtdlba_setup(unsigned int addr);
void mshc_emmc_cqe_config_timeout();
void mshc_emmc_cqe_enable_inter();
void mshc_emmc_cqe_set_rca(unsigned int cqe_rca);
void mshc_emmc_cqe_config_dcmd(unsigned int is_cmd);
void mshc_emmc_cqe_config_task_size(unsigned int task_size);
void mshc_emmc_cqe_enable();
unsigned int mshc_emmc_cqe_task_complete_int();
unsigned int mshc_emmc_cqe_task_complete_int_s();
unsigned int mshc_emmc_get_task_complete_notified();
void mshc_emmc_clear_slot_complete(unsigned int tdl_index);
void mshc_emmc_cqe_task_complete_clear();
void mshc_emmc_cqe_task_complete_clear_s();
void mshc_emmc_submit_one_task(unsigned int id);
unsigned int mshc_buf_data_read();
void mshc_buf_data_write(unsigned int data);
unsigned int mshc_card_addr_adjust(unsigned int cardtype, unsigned int addr, unsigned int blksize);
unsigned int wait_buf_read();
unsigned int wait_buf_read_s();
unsigned int wait_buf_write();
unsigned int wait_buf_write_s();
unsigned int wait_cmd_complete();
unsigned int wait_cmd_complete_s();
unsigned int wait_xfer_complete();
unsigned int wait_xfer_complete_s();
unsigned int do_wait_dma_interrupt();
unsigned int do_wait_dma_interrupt_s();
unsigned int do_wait_xfer_complete();
unsigned int do_wait_xfer_complete_s();
unsigned int cmd_err_stat_check();
unsigned int cmd_err_stat_check_s();
unsigned int xfer_err_stat_check();
unsigned int xfer_err_stat_check_s();
void mshc_sync_abort();

/* interrupt */
volatile unsigned short mshc_normal_int_stat_s;
volatile unsigned short mshc_error_int_stat_s;
volatile unsigned short mshc_emmc_cqe_cqis_s;

void mshc_interrupt_handler(unsigned int arg);

/* debug function */
void dump_error_status();
void dump_adma3_desc(adma3_integrated_desc_t *integrated_desc_p);
void dump_adma3_tran();
void dump_cmd_cfg();
void dump_response();

/* cmd send function */
unsigned char *mshc_cmd_send(unsigned int cmdidx, unsigned int arg, unsigned int cmdat, int rtype);

/* pio */
unsigned int mshc_pio_transfer(unsigned int *bufaddr, unsigned int cardaddr,
		unsigned int blksize, unsigned int blkcnt, unsigned int opsmode);
/* sdma */
unsigned int mshc_sdma_transfer(unsigned int *bufaddr, unsigned int cardaddr,
		unsigned int blksize, unsigned int blkcnt, unsigned int opsmode);
/* adma2 */
unsigned int mshc_adma2_transfer(unsigned int *bufaddr, unsigned int cardaddr,
		unsigned int blksize, unsigned int blkcnt, unsigned int opsmode);
/* adma3 */
unsigned int mshc_adma3_transfer(unsigned int *bufaddr_w, unsigned int *bufaddr_r,
		unsigned int cardaddr, unsigned int blksize, unsigned int blkcnt);
/* cqe */
unsigned int mshc_cqe_transfer(unsigned int *bufaddr_w, unsigned int *bufaddr_r, unsigned int cqe_rca,
		unsigned int cardaddr, unsigned int blksize, unsigned int blkcnt);

#endif /* _DW_MSHC_H_ */
