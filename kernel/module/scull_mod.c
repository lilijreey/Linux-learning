/**
 * @file     reverse_mod.c
 *            LDD3 例子
 */


#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/cdev.h>
/*#include <linux/errno.h>*/
#include <asm-generic/errno-base.h>
#include "lock.h"

typedef size_t lock_id_t ;
typedef u8 lock_state_t ;

typedef struct lock_info {
	lock_id_t id;
	lock_state_t state; //UNLOCK, SHARED_LOCK, EXCLUSIVE_LOCK
} lock_info_t;


//内核文件锁, 支持操作
//1. 加共享锁, 多进程可加，进程接收后自动释放
//2. 加独占锁, 进程结束后自动释放
//3. 解锁
//4. 独占变共享
//5. 查询锁状态
//
//. 阻塞加锁版本
//

static uint device_major = 0;
module_param(device_major, uint, S_IRUGO);
MODULE_PARM_DESC(device_major, "device major number");

static uint lock_nr = 1024; //支持的锁个数
module_param(lock_nr, uint, S_IRUGO);
MODULE_PARM_DESC(lock_nr, "lock count");

static const char* device_name = "scull";
static uint device_nr = 1;
//TODO indoe 级别数


typedef struct scull {
	struct cdev cdev; /* Char device structure */
	uint lock_nr;
	u8 lock_slots[0];
	/*link_list owners;*/
} scull_t;

static scull_t *scull;


/*#define	get_scull_from_inode(inode) container_of(inode->i_cdev, scull_t, cdev)*/
/*#define	get_scull_from_file(file) file->private_data                          */


int scull_open(struct inode *inode, struct file *file) {
	// get scull from inode , set in scull_init
	// 
	printk(KERN_INFO "scull_open  %p:%p\n", inode, file);


	//TODO 读写检测权限
	//file 私有数据实现

	return 0;
}

int scull_release(struct inode *inode, struct file *file) {
	//do nothing
	printk(KERN_INFO "scull_close %p:%p\n", inode, file);
	return 0;
}

static int scull_flush(struct file *file, fl_owner_t id) {
	//do nothing
	printk(KERN_INFO "scull_flush\n"); 
	return 0;
}

static inline lock_state_t get_lock_state(lock_id_t id)
{
	return scull->lock_slots[id];
}

//get lock info
static ssize_t scull_read(struct file *file, char __user *buff, size_t op, loff_t *off)
{
	//TODO 直接映射
	/*lock_info_t info;                                   */
	/*if (0 != copy_from_user(&info, buff, sizeof info)) {*/
	/*        printk(KERN_INFO "copy_from_user failed\n");*/
	/*        return -EAGAIN;                             */
	/*}                                                   */

	/*info.state = get_lock_state(info.id);               */
	/*if (0 != copy_to_user(buff, info, sizeof info)) {   */
	/*        printk(KERN_INFO "copy_to_user failed\n");  */
	/*        return -EAGAIN;                             */
	/*}                                                   */
	lock_id_t  id;
	printk(KERN_INFO "do read file:%p buff:%p op:%lu\n", file, buff, op);
	id = (lock_id_t)buff;
	if (id >= lock_nr)
		return -EINVAL;

	return get_lock_state(id);
}


//buff 参数值为锁id, count 用来标记操作
static ssize_t scull_write(struct file *file, const char __user *buff, size_t count, loff_t *off)
{

	/*if (0 != copy_from_user(&op, buff, sizeof op)) {*/
	/*        return -EAGAIN;                         */
	/*}                                               */

	printk(KERN_INFO "do write \n ");
	return 0;
}



static struct file_operations scull_fops = {
	.owner = THIS_MODULE,
	.open = scull_open,
	.release = scull_release, //释放file对象时，就是当打开的引用数为０时调用, dup
	.read = scull_read,
	.write = scull_write,
	.flush = scull_flush, //每次调用close 时被调用
};



static int register_devices(void)
{
	dev_t dev;
	int ret;

	if (device_major == 0)
	{
		//注册设备，主设备号由系统自动分配，也包括次设备号的注册
		ret = alloc_chrdev_region(&dev, 0, device_nr, device_name);
		if (ret < 0) {
			printk(KERN_INFO "alloc_chrdev_region failed %d\n", ret);
		} else {
			device_major = MAJOR(dev);
			printk(KERN_INFO "alloc_chrdev_region ok major:%u\n", device_major);
		}

	} else {
		ret = register_chrdev_region(MKDEV(device_major, 0), device_nr, device_name);
		if (ret < 0) {
			printk(KERN_INFO "register_chrdev_region  major:%u failed %d\n", device_major, ret);
		} else {
			printk(KERN_INFO "register_chrdev_region  major:%u ok \n", device_major);
		}
	}

	return ret;
}

/*static void scull_init(scull_t *scull, uint minor)*/
static void scull_init(void)
{
	int ret;
	scull = kzalloc(sizeof(scull_t) + sizeof(u8)*lock_nr,
			         GFP_KERNEL);
	if (scull == NULL) {
		printk(KERN_INFO "kzalloc failed \n");
		return ;
	}


	//多次打开,产生一个file对象, 关联同一个fops,类似类的成员变量和静态成员变量
	// inode ----1:n -->> file
	// file 打开文件对象
	// inode 文件元数据对象

	cdev_init(&scull->cdev, &scull_fops); //set inode
	scull->cdev.owner = THIS_MODULE;
	scull->lock_nr = lock_nr;

	//注册设备到内核
	ret = cdev_add(&scull->cdev, MKDEV(device_major, 0), 1);
	if (ret < 0) {
		printk(KERN_INFO "cdev_add failed %d\n", ret);
	}
}

static int __init mod_init(void)
{
	int ret;
	printk(KERN_INFO "%s module init this process :%s pid:%i\n",
			THIS_MODULE->name, current->comm, current->pid);

	//1.
	ret = register_devices();

	//2. 注册字符设备实例
	scull_init();


	return 0;
}

static void __exit mod_exit(void)
{
	cdev_del(&scull->cdev); 
	unregister_chrdev_region(MKDEV(device_major, 0), device_nr);
	kfree(scull);
	scull = NULL;
	printk(KERN_INFO "module %s exit \n", THIS_MODULE->name);
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("blog.topspeedsnail.com");
MODULE_DESCRIPTION("lock");

