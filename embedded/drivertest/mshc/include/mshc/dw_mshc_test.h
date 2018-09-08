#ifndef _DW_MSHC_TEST_H_
#define _DW_MSHC_TEST_H_

void data_pre(unsigned int blkcnt);

int pio_transfer_sigblk_blk();
int pio_transfer_mulblk_blk();

int sdma_transfer_sigblk_blk();
int sdma_transfer_mulblk_blk();

int adma2_transfer_sigblk_blk();
int adma2_transfer_mulblk_blk();

int adma3_transfer_sigblk_blk();
int adma3_transfer_mulblk_blk();

int cqe_transfer_sigblk_blk();
int cqe_transfer_mulblk_blk();


#endif /* _DW_MSHC_TEST_H_ */
