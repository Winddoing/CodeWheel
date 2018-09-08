#ifndef __DW_MSHC_emmc_TEST__
#define __DW_MSHC_emmc_TEST__

void emmc_init();

void emmc_pio_1_low_sigblk_tran();
void emmc_pio_1_high_sigblk_tran();
void emmc_pio_4_low_sigblk_tran();
void emmc_pio_4_high_sigblk_tran();
void emmc_pio_1_low_mulblk_tran();
void emmc_pio_1_high_mulblk_tran();
void emmc_pio_4_low_mulblk_tran();
void emmc_pio_4_high_mulblk_tran();

void emmc_sdma_1_low_sigblk_tran();
void emmc_sdma_1_high_sigblk_tran();
void emmc_sdma_4_low_sigblk_tran();
void emmc_sdma_4_high_sigblk_tran();
void emmc_sdma_1_low_mulblk_tran();
void emmc_sdma_1_high_mulblk_tran();
void emmc_sdma_4_low_mulblk_tran();
void emmc_sdma_4_high_mulblk_tran();

void emmc_adma2_1_low_sigblk_tran();
void emmc_adma2_1_high_sigblk_tran();
void emmc_adma2_4_low_sigblk_tran();
void emmc_adma2_4_high_sigblk_tran();
void emmc_adma2_1_low_mulblk_tran();
void emmc_adma2_1_high_mulblk_tran();
void emmc_adma2_4_low_mulblk_tran();
void emmc_adma2_4_high_mulblk_tran();

void emmc_adma3_1_low_sigblk_tran();
void emmc_adma3_1_high_sigblk_tran();
void emmc_adma3_4_low_sigblk_tran();
void emmc_adma3_4_high_sigblk_tran();
void emmc_adma3_1_low_mulblk_tran();
void emmc_adma3_1_high_mulblk_tran();
void emmc_adma3_4_low_mulblk_tran();
void emmc_adma3_4_high_mulblk_tran();

void emmc_cqe_1_low_sigblk_tran();
void emmc_cqe_1_high_sigblk_tran();
void emmc_cqe_4_low_sigblk_tran();
void emmc_cqe_4_high_sigblk_tran();
void emmc_cqe_1_low_mulblk_tran();
void emmc_cqe_1_high_mulblk_tran();
void emmc_cqe_4_low_mulblk_tran();
void emmc_cqe_4_high_mulblk_tran();


#endif //__DW_MSHC_emmc_TEST__
