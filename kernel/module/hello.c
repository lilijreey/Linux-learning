/**
 * @file    
 * @brief    内核 hello world 模块 
 *
 * @author   lili (), lilijreey@gmail.com
 * @date     04/23/2013 05:04:36 PM
 *
 */



#include <linux/init.h>
#include <linux/module.h>

static int __init hello_init(void)
{
	printk(KERN_INFO " Hello world enter\n");
	return 0;
}

static void __exit hello_exit(void)
{
	printk(KERN_INFO "Hello world exit\n");
}

/* init and exit mode */
module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("Willaim <vv@gmail.com>");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("A simple Hello World Module");
MODULE_ALIAS("a simplest module");


