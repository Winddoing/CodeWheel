#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/seq_file.h>
#include <linux/cdev.h>
#include <linux/device.h>

#include <linux/interrupt.h>
#include <linux/pwm.h>

#include <mach/jztcu.h>

#define PWM_CHL_NUM_MAX	7
struct jz_pwm_tst_dev {
	int major;
	int minor;
	int nr_devs;

	struct class *class;
	struct cdev cdev;
	struct device *dev;

	struct pwm_device *pwm[PWM_CHL_NUM_MAX];
	int pwm_test_bit;
	int pwm_test_irq_bit;
	int duty;
	int period;
	int test_num;
	spinlock_t lock;
};

#define PWM_CHL0 (0x1 << 0)
#define PWM_CHL1 (0x1 << 1)
#define PWM_CHL2 (0x1 << 2)
#define PWM_CHL3 (0x1 << 3)
#define PWM_CHL4 (0x1 << 4)
#define PWM_CHL5 (0x1 << 5)
#define PWM_CHL6 (0x1 << 6)
#define PWM_CHL7 (0x1 << 7)

static unsigned int pwm_test_num = 0;

#define PWM_TEST_PERIOD	  6000 //ns
#define PWM_TEST_INIT_DUTY	(PWM_TEST_PERIOD / 2)
#define PWM_TEST_CHANNEL (PWM_CHL3|PWM_CHL4)
#define PWM_TEST_USE_IRQ (PWM_CHL3)

/*#define PWM_TEST_CHANNEL (PWM_CHL4)*/
/*#define PWM_TEST_USE_IRQ PWM_TEST_CHANNEL*/

static unsigned int pwm_test_half[] = {
	(PWM_TEST_PERIOD * 1) / 10,
	(PWM_TEST_PERIOD * 2) / 10,
	(PWM_TEST_PERIOD * 3) / 10,
	(PWM_TEST_PERIOD * 4) / 10,
	(PWM_TEST_PERIOD * 5) / 10,
	(PWM_TEST_PERIOD * 6) / 10,
	(PWM_TEST_PERIOD * 7) / 10,
	(PWM_TEST_PERIOD * 8) / 10,
	(PWM_TEST_PERIOD * 9) / 10,
	(PWM_TEST_PERIOD * 8) / 10,
	(PWM_TEST_PERIOD * 7) / 10,
	(PWM_TEST_PERIOD * 6) / 10,
	(PWM_TEST_PERIOD * 5) / 10,
	(PWM_TEST_PERIOD * 4) / 10,
	(PWM_TEST_PERIOD * 3) / 10,
	(PWM_TEST_PERIOD * 2) / 10,
};

static unsigned int pwm_irq[] = {
	TCU_FFLAG0,
	TCU_FFLAG1,
	TCU_FFLAG2,
	TCU_FFLAG3,
	TCU_FFLAG4,
	TCU_FFLAG5,
	TCU_FFLAG6,
	TCU_FFLAG7,
};

static irqreturn_t jz_pwm_irq(int irq, void *data)
{
	struct jz_pwm_tst_dev *jz_pwm_tst = data;
	int i;

	for (i = 0; i < PWM_CHL_NUM_MAX; i++) {
		if (jz_pwm_tst->pwm_test_bit & BIT(i)) {
			if (!pwm_test_half[pwm_test_num]) {
				printk("%s: error num=%d, Duty=%d\n", __func__, pwm_test_num, pwm_test_half[pwm_test_num]);
				while(1);
			}
			pwm_config(jz_pwm_tst->pwm[i], pwm_test_half[pwm_test_num],jz_pwm_tst->period);
		}
	}
/*	printk("==> func: %s, num=%d, total num=%d, duty=%d, Duty cycle: %d\n",*/
/*			__func__, pwm_test_num, jz_pwm_tst->test_num, pwm_test_half[pwm_test_num],*/
/*			(pwm_test_half[pwm_test_num]*100)/jz_pwm_tst->period);*/
	pwm_test_num ++;
	if (pwm_test_num > (jz_pwm_tst->test_num - 1))
		pwm_test_num = 0;

#if 0
	if (pwm_test_num == 10) {
		printk("TCSR: 0x%08x\n", readl(0xb0002000 + 0x8c));
		printk("TDFR: 0x%08x\n", readl(0xb0002000 + 0x80));
		printk("TDHR: 0x%08x\n", readl(0xb0002000 + 0x84));
		printk("TCNT: 0x%08x\n", readl(0xb0002000 + 0x88));
		printk("TER : 0x%08x\n", readl(0xb0002000 + 0x10));
		printk("TFR : 0x%08x\n", readl(0xb0002000 + 0x20));
		printk("TMR : 0x%08x\n", readl(0xb0002000 + 0x30));
		printk("TSR : 0x%08x\n", readl(0xb0002000 + 0x1c));
		while(1);

	}
#endif
	return IRQ_HANDLED;
}

static int jz_pwm_tst_open(struct inode *inode, struct file *filp)
{
	struct cdev *cdev = inode->i_cdev;
	struct jz_pwm_tst_dev *jz_pwm_tst = container_of(cdev, struct jz_pwm_tst_dev, cdev);
	int i;

	printk("func: %s, Duty cycle=%d\n", __func__, (jz_pwm_tst->duty * 100)/jz_pwm_tst->period);
	for (i = 0; i < PWM_CHL_NUM_MAX; i++) {
		if (jz_pwm_tst->pwm_test_bit & BIT(i))
			pwm_config(jz_pwm_tst->pwm[i], jz_pwm_tst->duty,jz_pwm_tst->period);
	}

	spin_lock_irq(&jz_pwm_tst->lock);
	for (i = 0; i < PWM_CHL_NUM_MAX; i++) {
		if (jz_pwm_tst->pwm_test_bit & BIT(i))
			pwm_enable(jz_pwm_tst->pwm[i]);
	}
	spin_unlock_irq(&jz_pwm_tst->lock);
	return 0;
}

static int jz_pwm_tst_close(struct inode *inode, struct file *filp)
{
	struct cdev *cdev = inode->i_cdev;
	struct jz_pwm_tst_dev *jz_pwm_tst = container_of(cdev, struct jz_pwm_tst_dev, cdev);
	int i;

	printk("========> func: %s, line: %d\n", __func__, __LINE__);
	spin_lock_irq(&jz_pwm_tst->lock);
	for (i = 0; i < PWM_CHL_NUM_MAX; i++) {
		if (jz_pwm_tst->pwm_test_bit & BIT(i)) {
			pwm_disable(jz_pwm_tst->pwm[i]);
		}
	}
	spin_unlock_irq(&jz_pwm_tst->lock);
	return 0;
}

static struct file_operations jz_pwm_tst_ops = {
	.owner = THIS_MODULE,
	.open = jz_pwm_tst_open,
	.release = jz_pwm_tst_close,
};

static int __init jz_pwm_tst_init(void)
{
	struct jz_pwm_tst_dev *jz_pwm_tst;
	char pwm_lable[10];
	dev_t dev = 0;
	int ret, dev_no, i;

	jz_pwm_tst = kmalloc(sizeof(struct jz_pwm_tst_dev), GFP_KERNEL);
	if(!jz_pwm_tst) {
		printk("pwm test dev alloc failed\n");
		goto __err_jz_pwm_tst;
	}
	jz_pwm_tst->class = class_create(THIS_MODULE, "jz-pwm-tst");
	jz_pwm_tst->minor = 0;
	jz_pwm_tst->nr_devs = 1;
	ret = alloc_chrdev_region(&dev, jz_pwm_tst->minor, jz_pwm_tst->nr_devs, "jz-pwm-tst");
	if(ret) {
		printk("alloc chrdev failed\n");
		goto __err_chrdev;
	}
	jz_pwm_tst->major = MAJOR(dev);

	dev_no = MKDEV(jz_pwm_tst->major, jz_pwm_tst->minor);
	cdev_init(&jz_pwm_tst->cdev, &jz_pwm_tst_ops);
	jz_pwm_tst->cdev.owner = THIS_MODULE;
	cdev_add(&jz_pwm_tst->cdev, dev_no, 1);

	jz_pwm_tst->dev = device_create(jz_pwm_tst->class, NULL, dev_no, NULL, "jz-pwm-tst");

	jz_pwm_tst->pwm_test_bit = PWM_TEST_CHANNEL;
	jz_pwm_tst->pwm_test_irq_bit = PWM_TEST_USE_IRQ;
	jz_pwm_tst->period = PWM_TEST_PERIOD;
	jz_pwm_tst->duty = PWM_TEST_INIT_DUTY;
	jz_pwm_tst->test_num = ARRAY_SIZE(pwm_test_half);

	spin_lock_init(&jz_pwm_tst->lock);
	for (i = 0; i < PWM_CHL_NUM_MAX; i++) {
		if (jz_pwm_tst->pwm_test_bit & BIT(i)) {
			memset(pwm_lable, 0, sizeof(pwm_lable));
			sprintf(pwm_lable, "pwm%d test", i);
			jz_pwm_tst->pwm[i]= pwm_request(i, pwm_lable);
			if (jz_pwm_tst->pwm_test_irq_bit & BIT(i)) {
				ret = request_irq(pwm_irq[i], jz_pwm_irq, IRQF_SHARED, "jz_pwm_test", jz_pwm_tst);
				if (ret < 0) {
					kfree(jz_pwm_tst);
					printk("%s, %d: PWM irq request fail..\n", __func__, __LINE__);
					return -1;
				}
			}
		}
	}
	dev_info(jz_pwm_tst->dev, "jz_pwm_test register success...\n");
	return 0;

__err_chrdev:
	kfree(jz_pwm_tst);
__err_jz_pwm_tst:
	return -EFAULT;
}
static void __exit jz_pwm_tst_exit(void)
{

}

module_init(jz_pwm_tst_init);
module_exit(jz_pwm_tst_exit);

