#include <linux/module.h>

int init_module(void)
{
	printk("<0>Hello World!\n");
	return 0;
}


void cleanup_module(void)
{
	printk("<0>Goodbye!\n");
}

MODULE_LICENSE("GPL");
