#include <linux/module.h>
#include <linux/blkdev.h>
#include <linux/freezer.h>
#include <linux/kthread.h>
#include <linux/scatterlist.h>

static struct platform_device test_device = {
	.name = "test",
	.id = 0,
};

static struct platform_driver test_driver = {
	.probe = test_probe,
	.remove = test_remove,
	.driver = {
		.name = "test",
	},
};

static int test_probe(struct platform_device *pdev)
{
	init_waitqueue_head();

	return 0;
}

static int test_remove(struct platform_device *pdev)
{

	return 0;
}



static init __init test_init(void)
{
	int ret = 0;

	ret = platform_driver_register(&test_driver);
	if(ret)
		return ret;

	ret = platform_device_register(&test_device);
	if(ret)
		goto driver_unregister;
	return 0;

driver_unregister:
	platform_driver_unregister(&test_driver);
	return ret;
}
static void __exit test_exit(void)
{
	platform_device_unregister(&test_device);
	platform_driver_unregister(&test_driver);
}

module_init(test_init);

module_exit(test_exit);

MODULE_DESCRIPTION("test wait queue");
MODULE_LICENSE("GPL");

