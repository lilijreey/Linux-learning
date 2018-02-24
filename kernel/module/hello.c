/**
 * @file    
 * @brief    内核 hello world 模块 
 *
 * @author   lili (), lilijreey@gmail.com
 * @date     04/23/2013 05:04:36 PM
 *  实现一个逆序设备，读入字符，输出逆序
 *
 */



#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>

static int param  =0;
module_param(param, int, S_IRUGO);
MODULE_PARM_DESC(param, "Deriver major number");

int hello_mod_ver = 111;
static int hello_mod_ver_sub = 111;

static int __init hello_init(void)
{

    //current 当前进程对象
	printk(KERN_INFO "current process name:%s pid:%i\n", 
           current->comm, current->pid);

	printk(KERN_INFO "param is:%d\n", param);

	printk(KERN_INFO "Hello world enter\n");
	printk(KERN_INFO "param is:%d\n", param);
	printk(KERN_ALERT "This module:%p==%p/n", &__this_module, THIS_MODULE);
	printk(KERN_ALERT "module: state:%d\n", THIS_MODULE->state);
	printk(KERN_ALERT "module: name:%s\n", THIS_MODULE->name);
	printk("module: name:%s\n", THIS_MODULE->name);
	printk("module refcount:%d\n", module_refcount(THIS_MODULE));
	/// try_module_get inc refcount
	//remod 在hello_exit 之前检查refcount 如果不为1 则无法卸载掉模块
	/*printk("module get:%d\n", module_get(THIS_MODULE));*/
	printk("module refcount:%d\n", module_refcount(THIS_MODULE));
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

//导出一个符号 TODO 在别的模块中访问
EXPORT_SYMBOL(hello_mod_ver); //会出现在/proc/kallsysms 中

