/* 
* dw axi dmac
* author: helb
* date: 2018-08-08
*/


#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <asm/uaccess.h>
#include <linux/dmaengine.h>
#include <linux/poll.h>
#include <linux/cdev.h>
#include <linux/slab.h>

#include <linux/timekeeping.h>
#include <linux/seq_file.h>
#include <linux/signal.h>
#include <linux/proc_fs.h>

#define DRIVER_NAME         "dma"
#define AXIDMA_IOC_MAGIC            'A'
#define DMA_PROC_NAME      "driver/dma"

#define AXIDMA_IOCCFGANDSTART_AHB       _IO(AXIDMA_IOC_MAGIC, 1)
#define AXIDMA_IOCCFGANDSTART_AXI       _IO(AXIDMA_IOC_MAGIC, 2)


#define AXIDMA_NAME "f6500000.dma-controller"

#define DMA_CHN_UNUSED      0
#define DMA_CHN_USED        1

struct dma_chncfg {
    unsigned int src_addr;
    unsigned int dst_addr;
    unsigned int len;
};


struct dma_chncfg_sg{
    unsigned int src_addr;
    unsigned int dst_addr;
    unsigned int len;
    unsigned int src_stride;
	unsigned int dst_stride;
    unsigned int high;
};

static struct proc_dir_entry * dma_proc = NULL;

static void dma_complete_func(void *completion)
{
    complete(completion);
}
bool find_axidma_chan(struct dma_chan *chan , void *name)
{
	return !strncmp(dev_name(chan->device->dev), name, strlen(name));
}

//#define AXDMA_TIME_TEST

static long gv_dma_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    struct dma_device *dma_dev;
    struct dma_async_tx_descriptor *tx = NULL;
    dma_cap_mask_t mask;
    dma_cookie_t cookie;
    enum dma_ctrl_flags flags;
    int i;
    struct dma_chncfg chncfg;
    int ret = 0;
    struct completion irq_raised;
    unsigned long timeout;
    struct dma_chan *dma_chan = NULL;
    struct dma_chncfg_sg chnCfgSg;
    struct scatterlist *tx_sg = NULL;
    struct scatterlist *rx_sg = NULL;
    //unsigned int         sg_cnt;
#ifdef AXDMA_TIME_TEST
	struct timeval start_tv;
	struct timeval end_tv;

	do_gettimeofday(&start_tv);
#endif	
    init_completion( &irq_raised );

    switch(cmd)
    {
        case AXIDMA_IOCCFGANDSTART_AHB:
        {
            ret = copy_from_user(&chncfg, (void __user *)arg, sizeof(struct dma_chncfg));
            if(ret){
                pr_err("Copy from user failed\n");
                goto error;
            }

            dma_cap_zero(mask);
            dma_cap_set(DMA_MEMCPY, mask);
			dma_chan = __dma_request_channel(&mask, find_axidma_chan, AXIDMA_NAME);
            if(!dma_chan){
                pr_err("dma request channel failed\n");
               goto error;
            }
            //printk("channed id = %d\n", dma_chan->chan_id);
            //printk("chan device=%s\n", dev_name(dma_chan->device->dev));
            dma_dev = dma_chan->device;
            flags = DMA_CTRL_ACK | DMA_PREP_INTERRUPT;
            tx = dma_dev->device_prep_dma_memcpy(dma_chan, chncfg.dst_addr, chncfg.src_addr, chncfg.len, flags);
            if(!tx){
                pr_err("Failed to prepare DMA memcpy\n");
                goto chan_err;
            }
            tx->callback = dma_complete_func;

            tx->callback_param = &irq_raised;
            cookie =  tx->tx_submit(tx);
            if(dma_submit_error(cookie)){
                pr_err("Failed to dma tx_submit\n");
                goto chan_err;
            }
            dma_async_issue_pending(dma_chan);

            //wait_for_completion(&irq_raised);
            timeout = wait_for_completion_timeout(&irq_raised, msecs_to_jiffies(60000));
            if (!timeout) {
                pr_err("wait for completion timeout\n");
                dmaengine_terminate_async(dma_chan);
                goto chan_err;
            }
            //printk("++++++++++++channed id = %d++++++++++++++++\n", dma_chan->chan_id);
            dma_release_channel(dma_chan);
        }
        break;

        case AXIDMA_IOCCFGANDSTART_AXI:
        {
            ret = copy_from_user(&chnCfgSg, (void __user *)arg, sizeof(struct dma_chncfg_sg));
            if(ret){
                pr_err("Copy from user failed\n");
                goto error;
            }
            tx_sg =  kmalloc(sizeof(struct scatterlist) * chnCfgSg.high, GFP_KERNEL);
            rx_sg =  kmalloc(sizeof(struct scatterlist) * chnCfgSg.high, GFP_KERNEL);
            if(tx_sg ==NULL || rx_sg == NULL)
            {
                pr_err("ioctl AXIDMA_IOCCFGANDSTART_AXI kmalloc failed\n");
                goto error;
            }
            sg_init_table(tx_sg, chnCfgSg.high);
            sg_init_table(rx_sg, chnCfgSg.high);
            for (i = 0; i < chnCfgSg.high ; i++) {
                sg_dma_address(&rx_sg[i]) = chnCfgSg.src_addr + (i * chnCfgSg.src_stride);
                sg_dma_address(&tx_sg[i]) = chnCfgSg.dst_addr + (i * chnCfgSg.dst_stride);
                sg_dma_len(&tx_sg[i]) = chnCfgSg.len;
                sg_dma_len(&rx_sg[i]) = chnCfgSg.len;
            }
            

            dma_cap_zero(mask);
            dma_cap_set(DMA_SG, mask); 
			dma_chan = __dma_request_channel(&mask, find_axidma_chan, AXIDMA_NAME);
            //dma_chan = dma_request_channel(mask, NULL, NULL);
            if(!dma_chan){
                pr_err("dma request channel failed\n");
               goto error;
            }
            //printk("channed id = %d\n", dma_chan->chan_id);
            //printk("chan device=%s\n", dev_name(dma_chan->device->dev));
            dma_dev = dma_chan->device;
            flags = DMA_CTRL_ACK | DMA_PREP_INTERRUPT;
            
            tx = dma_dev->device_prep_dma_sg(dma_chan, tx_sg, chnCfgSg.high, rx_sg, chnCfgSg.high, flags);
            if(!tx){
                pr_err("Failed to prepare DMA memcpy\n");
                goto chan_err;
            }
            tx->callback = dma_complete_func;

            tx->callback_param = &irq_raised;
            cookie =  tx->tx_submit(tx);
            if(dma_submit_error(cookie)){
                pr_err("Failed to dma tx_submit\n");
                goto chan_err;
            }
            dma_async_issue_pending(dma_chan);

            //wait_for_completion(&irq_raised);
            timeout = wait_for_completion_timeout(&irq_raised, msecs_to_jiffies(60000));
            if (!timeout) {
                pr_err("wait for completion timeout\n");
                dmaengine_terminate_async(dma_chan);
                goto chan_err;
            }
            //printk("++++++++++++channed id = %d++++++++++++++++\n", dma_chan->chan_id);
            dma_release_channel(dma_chan);
			if(tx_sg != NULL){
				kfree(tx_sg);
				tx_sg = NULL;
			}
			if(rx_sg != NULL){
				kfree(rx_sg);
				rx_sg = NULL;
			}
			break;
        }
        default:
            printk("Don't support cmd [%d]\n", cmd);
            break;
    }
#ifdef AXDMA_TIME_TEST
	do_gettimeofday(&end_tv);
	printk("current tid: %d,chan id:%d, spend time:%d\n", current->pid, dma_chan->chan_id, (int)(end_tv.tv_sec * 1000000 + end_tv.tv_usec  - start_tv.tv_sec * 1000000 - start_tv.tv_usec));
#endif
    return 0;
chan_err:
    dma_release_channel(dma_chan);
error:
    if(tx_sg != NULL)
    {
        kfree(tx_sg);
        tx_sg = NULL;
    }
    if(rx_sg != NULL)
    {
        kfree(rx_sg);
        rx_sg = NULL;
    }

    return -EFAULT;
}

/*************************************************
	gvdma proc file system
*************************************************/

static int gvdma_show_dmachan(struct seq_file *m, void *v)
{
	list_dma_chan();
	
	return 0;
}
static int gvdma_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, gvdma_show_dmachan, NULL);
}

static const struct file_operations gvdma_proc_operation = {
	.owner		= THIS_MODULE,
	.open		= gvdma_proc_open,
	.read		= seq_read,
	.llseek 	= seq_lseek,
	.release	= seq_release,
};

void gvdma_proc_init(void)
{
	dma_proc = proc_create(DMA_PROC_NAME, 0400, NULL, &gvdma_proc_operation);
}

void gvdma_proc_exit(void)
{
	if(dma_proc != NULL){
		proc_remove(dma_proc);
		dma_proc = NULL;
	}
}
/********************************************/
/*
 *    Kernel Interfaces
 */

static struct file_operations gv_dma_fops = {
    .owner        = THIS_MODULE,
    .llseek        = no_llseek,
    .unlocked_ioctl = gv_dma_unlocked_ioctl,
};

static struct miscdevice gv_dma_miscdev = {
    .minor        = MISC_DYNAMIC_MINOR,
    .name        = DRIVER_NAME,
    .fops        = &gv_dma_fops,
};

static int __init gv_dma_init(void)
{
    int ret = 0;
	
    gvdma_proc_init();
	
    ret = misc_register(&gv_dma_miscdev);
    if(ret) {
        printk (KERN_ERR "cannot register miscdev (err=%d)\n", ret);
        return ret;
    }
    return 0;
}

static void __exit gv_dma_exit(void)
{   
    misc_deregister(&gv_dma_miscdev);

	gvdma_proc_exit();
}

module_init(gv_dma_init);
module_exit(gv_dma_exit);

MODULE_AUTHOR("tao.wang@byavs.com");
MODULE_DESCRIPTION("Byavs dma Driver include axi and ahb");
MODULE_LICENSE("GPL");

