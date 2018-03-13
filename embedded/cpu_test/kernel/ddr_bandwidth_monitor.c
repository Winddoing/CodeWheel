#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/syscore_ops.h>
#include <linux/slab.h>
#include <linux/debugfs.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/spinlock.h>
#define DDR_MONITOR_BASE   0x134F0000
#define DDR_DYNAMIC_CLK    0x13012068

#define DB_MONITOR_BASE_OFF  0x88// DDR bandwidth monitor base offset address.

struct dbw_channel_reg{
	volatile unsigned int write;
	unsigned int reserve1;
	volatile unsigned int read;
	unsigned int reserve2;
};
struct dbw_reg{
	volatile unsigned int dbwcfg;
	unsigned int reserve;
	volatile unsigned int period;
	unsigned int reserver;
	struct dbw_channel_reg chan[4];
};
#define AHB2_CHANNEL 2
#define CPU_CHANNEL 3

struct ddr_statistics{
	struct dentry		*root;
	unsigned int periods;
	unsigned int output;
	int run;

	unsigned int ahb2_read_rate;
	unsigned int ahb2_write_rate;
	unsigned int cpu_read_rate;
	unsigned int cpu_write_rate;

	int poll_periods_ms;
	struct dbw_reg *reg;
	struct mutex		lock;
	struct timer_list timer;
};
static struct ddr_statistics ddr_statistics = {
	.periods = 25 * 1000 * 1000,
	.run = 0,
	.poll_periods_ms = 40,
	.output = 1,
};
#define BW_DONE (1 << 3)
#define BW_INT_EN (1 << 2)
#define BW_CLR  (1 << 1)
#define BW_EN   (1 << 0)

#define DDR_PACKAGE_TO_CYCLE 4
static void ddr_stat_output(struct ddr_statistics *ddr)
{
	uint64_t period = (uint64_t)ddr->reg->period;
#define occupancy_show(title,id) do{									\
		uint64_t w_occupancy = (uint64_t)ddr->reg->chan[id].write * DDR_PACKAGE_TO_CYCLE; \
		uint64_t r_occupancy = (uint64_t)ddr->reg->chan[id].read * DDR_PACKAGE_TO_CYCLE; \
		uint64_t d = 100000 * w_occupancy;								\
		uint64_t hi;													\
		do_div(d,period);												\
		hi = (uint32_t)d / 1000;										\
		printk("%s write occupancy rate:%lld.%03lld%%\n",title,hi,d - hi * 1000); \
		d = 100000 * r_occupancy;										\
		do_div(d,period);												\
		hi = (uint32_t)d / 1000;										\
		printk("%s read occupancy rate:%lld.%03lld%%\n",title,hi,d - hi * 1000); \
	}while(0)
	occupancy_show("ahb2",AHB2_CHANNEL);
	occupancy_show("cpu",CPU_CHANNEL);
#undef occupancy_show
}


static ssize_t ddr_read_run(struct file *file, char __user *user_buf,
							size_t count, loff_t *ppos)
{
	struct ddr_statistics *ddr = file->private_data;
	char *buf;

	mutex_lock(&ddr->lock);
	if(ddr->reg->dbwcfg & BW_EN)
		buf = "Runing\n";
	else
		buf = "Stop\n";
	mutex_unlock(&ddr->lock);
	return simple_read_from_buffer(user_buf, count, ppos, buf, strlen(buf));
}

static ssize_t ddr_write_run(struct file *file, const char __user *user_buf,
							 size_t count, loff_t *ppos)
{
	struct ddr_statistics *ddr = file->private_data;
	char buf[16];
	bool bv;
	int ret = 0;
	unsigned int val;
	if (copy_from_user(buf, user_buf, min(count, (sizeof(buf) - 1))))
		return -EFAULT;

	if (strtobool(buf, &bv) == 0) {
		mutex_lock(&ddr->lock);

		if(bv){
			val = readl((void*)CKSEG1ADDR(DDR_DYNAMIC_CLK));
			val &= ~(1 << 8);
			writel(val,(void*)CKSEG1ADDR(DDR_DYNAMIC_CLK));

			del_timer_sync(&ddr->timer);
			if(ddr->reg->period != ddr->periods)
				ddr->poll_periods_ms = 0;
			ddr->run = 1;
			ddr->reg->period = ddr->periods;
			val = ddr->reg->period;

			ddr->reg->dbwcfg = BW_EN | BW_CLR;
			while((ddr->reg->dbwcfg & (BW_CLR | BW_EN)) != BW_EN);
			printk("run!\n");
			mod_timer(&ddr->timer,jiffies + msecs_to_jiffies(ddr->poll_periods_ms));
		}else{

			del_timer_sync(&ddr->timer);
			ddr->run = 0;
			if(ddr->output){
				ddr_stat_output(ddr);
			}
			ddr->ahb2_write_rate = ddr->reg->chan[AHB2_CHANNEL].write;
			ddr->ahb2_read_rate = ddr->reg->chan[AHB2_CHANNEL].read;
			ddr->cpu_write_rate = ddr->reg->chan[CPU_CHANNEL].write;
			ddr->cpu_read_rate = ddr->reg->chan[CPU_CHANNEL].read;

			ddr->reg->dbwcfg = 0;
			printk("stopping!\n");
		}
		mutex_unlock(&ddr->lock);
	}

	return ret ? ret : count;
}

static const struct file_operations ddr_run_fops = {
	.read	= ddr_read_run,
	.write	= ddr_write_run,
	.open	= simple_open,
	.llseek	= default_llseek,
};
static void ddr_stat_timer_handler(unsigned long data)
{
	struct ddr_statistics *ddr = (struct ddr_statistics *)data;
	unsigned int dbwcfg = ddr->reg->dbwcfg;
	if(dbwcfg & BW_DONE)
	{
		if(ddr->run){
			if(ddr->output){
				ddr_stat_output(ddr);
			}
			if(ddr->reg->dbwcfg != dbwcfg)
				ddr->poll_periods_ms = 0;
			ddr->ahb2_write_rate = ddr->reg->chan[AHB2_CHANNEL].write;
			ddr->ahb2_read_rate = ddr->reg->chan[AHB2_CHANNEL].read;
			ddr->cpu_write_rate = ddr->reg->chan[CPU_CHANNEL].write;
			ddr->cpu_read_rate = ddr->reg->chan[CPU_CHANNEL].read;

			dbwcfg |= BW_CLR;
			ddr->reg->dbwcfg = dbwcfg;

			while(ddr->reg->dbwcfg & BW_CLR);
			mod_timer(&ddr->timer,jiffies + msecs_to_jiffies(ddr->poll_periods_ms));
		}else{
			ddr->reg->dbwcfg = 0;
		}
	} else {
		mod_timer(&ddr->timer,jiffies + msecs_to_jiffies(10));
		ddr->poll_periods_ms += 10;
	}
}
static int __init ddr_stat_init(void)
{
	struct dentry * d;
	struct ddr_statistics *ddr = &ddr_statistics;

	d = debugfs_create_dir("ddr", NULL);
	if (IS_ERR(d)){
		pr_err("create debugfs for ddr failed.\n");
		return PTR_ERR(d);
	}

	ddr->root = d;
	d = debugfs_create_u32("periods", S_IWUSR | S_IRUGO, ddr->root,
						   (u32 *)&ddr->periods);
	if (IS_ERR_OR_NULL(d))
		goto err_node;

	d = debugfs_create_u32("output", S_IWUSR | S_IRUGO, ddr->root,
						   (u32 *)&ddr->output);
	if (IS_ERR_OR_NULL(d))
		goto err_node;

	d = debugfs_create_u32("ahb2_read_rate", S_IWUSR | S_IRUGO, ddr->root,
						   (u32 *)&ddr->ahb2_read_rate);
	if (IS_ERR_OR_NULL(d))
		goto err_node;

	d = debugfs_create_u32("ahb2_write_rate", S_IWUSR | S_IRUGO, ddr->root,
						   (u32 *)&ddr->ahb2_write_rate);
	if (IS_ERR_OR_NULL(d))
		goto err_node;
	d = debugfs_create_u32("cpu_read_rate", S_IWUSR | S_IRUGO, ddr->root,
						   (u32 *)&ddr->cpu_read_rate);
	if (IS_ERR_OR_NULL(d))
		goto err_node;
	d = debugfs_create_u32("cpu_write_rate", S_IWUSR | S_IRUGO, ddr->root,
						   (u32 *)&ddr->cpu_write_rate);
	if (IS_ERR_OR_NULL(d))
		goto err_node;


	d = debugfs_create_file("run", S_IWUSR | S_IRUGO, ddr->root,
							ddr, &ddr_run_fops);
	if (IS_ERR_OR_NULL(d))
		goto err_node;
	mutex_init(&ddr->lock);

	ddr->reg = ioremap(DDR_MONITOR_BASE + DB_MONITOR_BASE_OFF,sizeof(struct dbw_reg));

	init_timer(&ddr->timer);
	ddr->timer.function = ddr_stat_timer_handler;
	ddr->timer.data	= (unsigned long)ddr;

	return 0;
err_node:
	debugfs_remove_recursive(ddr->root);

	return -1;
}

static void __exit ddr_stat_deinit(void)
{
	struct ddr_statistics *ddr = &ddr_statistics;
	debugfs_remove_recursive(ddr->root);
}

late_initcall(ddr_stat_init);
module_exit(ddr_stat_deinit);
