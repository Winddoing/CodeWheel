/*#############################################################
 *     File Name	: hello.c
 *     Author		: winddoing
 *     Created Time	: 2020年09月02日 星期三 11时10分15秒
 *     Description	:
 *############################################################*/

#include <linux/module.h>   //needed for KERN_INFO
#include <linux/kernel.h>   //needed for KERN_INFO
#include <linux/init.h>     //needed for the macros

#include <linux/sched.h>

int __init hw_init(void) {
    printk(KERN_INFO"Hello World\n");
    printk("task_struct sizeof=%ld\n", sizeof(struct task_struct));
    return 0;
}

void __exit hw_exit(void) {
    printk(KERN_INFO"Bye World\n");
}

MODULE_LICENSE("GPL");

module_init(hw_init);
module_exit(hw_exit);
