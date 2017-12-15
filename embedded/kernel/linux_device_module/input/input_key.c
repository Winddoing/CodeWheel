#include <linux/input.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
/*#include <mach/irqs.h>*/
/*#include <plat/irqs.h>*/

static struct input_dev *button_dev;

static irqreturn_t button_interrupt(int irq, void *dummy)
{
	input_report_key(button_dev, KEY_0, 1);
	input_report_key(button_dev, KEY_0, 0);
	input_sync(button_dev);
	return IRQ_HANDLED;
}

static int __init button_init(void)
{
	int error;

	if (request_irq(IRQ_EINT(16), button_interrupt, IRQF_DISABLED | IRQF_TRIGGER_FALLING, "button", NULL)) {
                printk(KERN_ERR "button.c: Can't allocate irq %d\n", IRQ_EINT(16));
                return -EBUSY;
        }

	button_dev = input_allocate_device();
	if (!button_dev) {
		printk(KERN_ERR "button.c: Not enough memory\n");
		error = -ENOMEM;
		goto err_free_irq;
	}

	button_dev->evbit[0] = BIT_MASK(EV_KEY);  //事件使能位
	button_dev->keybit[BIT_WORD(KEY_0)] = BIT_MASK(KEY_0); //按键范围使能

	error = input_register_device(button_dev);
	if (error) {
		printk(KERN_ERR "button.c: Failed to register device\n");
		goto err_free_dev;
	}

	return 0;

 err_free_dev:
	input_free_device(button_dev);
 err_free_irq:
	free_irq(IRQ_EINT(16), NULL);
	return error;
}

static void __exit button_exit(void)
{
    input_unregister_device(button_dev);
	free_irq(IRQ_EINT(16), NULL);
}

module_init(button_init);
module_exit(button_exit);

MODULE_LICENSE("GPL");
