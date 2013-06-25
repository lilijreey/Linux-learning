/**
 * @file     globalfifo.c
 *
 * @brief   FIFO 虚拟设备实现
 *          实现阻塞IO
 *
 * @author   lili <lilijreey@gmail.com>
 * @date     04/25/2013 11:07:24 AM
 *
 */

//Qus. 
//1 count 参数是什么意思？
//2 设备号区域具体是什么？
//

#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/wait.h>
#include <linux/semaphore.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/uaccess.h>

#define globalfifo_SIZE 0x1000 /* 4K */
#define	MEM_CLEAR 0x1			/*  */
//#define	globalfifo_MAJOR	250		/* device major ID */

/// 当已经有相同的majorID，insert到kernel后，在insmod会报 
//    "Device or resource busy"
static int globalfifo_major = 255; 
///支持传递参数来改变globalfifo_major
module_param(globalfifo_major, int, S_IRUGO);
MODULE_PARM_DESC(globalfifo_major, "Deriver major number");

///察看当前所有设备的id cat /proc/devices

/* globalfifo  */
struct globalfifo_dev
{
	struct cdev cdev;
	size_t current_len; //data len
	unsigned char mem[globalfifo_SIZE];
	struct semaphore sem; 
	wait_queue_head_t r_wait;/* read interupt list */ 
	wait_queue_head_t w_wait;/* wirte interupt list */ 

	//支持 signal-drive IO
	struct fasync_struct * async_queue; 
};

//instance
static struct globalfifo_dev *devp;



/*
 * AIP callback
 */

static int globalfifo_open(struct inode *inodep, struct file *filp)
{
	/* set private_data */
	filp->private_data = devp;
	return 0;
}

static int globalfifo_release(struct inode *inodep, struct file *filp)
{
	return 0;
}


static ssize_t globalfifo_read(struct file *filp, char __user *buf,
	       	size_t count, loff_t *ppos)
{
	ssize_t ret = 0;
	struct globalfifo_dev *dev = filp->private_data;

	DECLARE_WAITQUEUE(wait, current); //define wait queue

	//get  sem
	down(&dev->sem);
	add_wait_queue(&dev->r_wait, &wait);

	while (dev->current_len == 0) {
		if (filp->f_flags & O_NONBLOCK) {
			ret = - EAGAIN;
			goto out;
		}
		__set_current_state(TASK_INTERRUPTIBLE); //set task state
		up(&dev->sem);

		schedule();

		if (signal_pending(current)) { /* 是否是因为信号唤醒 */
			ret = - ERESTARTSYS;
			goto out2;
		}	
		down(&dev->sem);
	}

	if (count >= dev->current_len)
		count = dev->current_len;

	if (copy_to_user(buf, (void*)(dev->mem), count)) {
		ret = -EFAULT;
		goto out;
	} else {
		//remove read data
		memcpy(dev->mem, dev->mem + count, dev->current_len - count);
		dev->current_len -= count;
		printk(KERN_INFO "read %lu B current_len %lu\n", count,
				dev->current_len);
		
		wake_up_interruptible(&dev->w_wait);
		ret = count;
	}

out: up(&dev->sem);
out2: remove_wait_queue(&dev->r_wait, &wait);
      set_current_state(TASK_RUNNING); ///Qus why set
      return ret;
}

static ssize_t globalfifo_write(struct file * filp, const char __user *buf, 
		size_t count, loff_t *ppos)
{
	ssize_t ret = 0;
	struct globalfifo_dev *dev = filp->private_data;
	DECLARE_WAITQUEUE(wait, current);

	down(&dev->sem);
	add_wait_queue(&dev->r_wait, &wait);

	while (dev->current_len == globalfifo_SIZE) {
		if (filp->f_flags & O_NONBLOCK) {
			ret = -EAGAIN;
			goto out;
		}

		__set_current_state(TASK_INTERRUPTIBLE); //set task state
		up(&dev->sem);

		schedule();
		if (signal_pending(current)) { /* 是否是因为信号唤醒 */
			ret = - ERESTARTSYS;
			goto out2;
		}
		down(&dev->sem);
	}


	if (count > globalfifo_SIZE - dev->current_len)
		count = globalfifo_SIZE - dev->current_len;

	if (copy_from_user(dev->mem + dev->current_len, buf, count)) {
		ret = -EFAULT;
		goto out;
	} else {
		dev->current_len += count;
		printk(KERN_INFO "write %lu B current_len %lu\n", count,
				dev->current_len);

		wake_up_interruptible(&dev->r_wait);

		if (dev->async_queue) // 发送 POLL_IN 可读
			kill_fasync(&dev->async_queue, SIGIO, POLL_IN);

		ret = count;
	}

out:
	up(&dev->sem);
out2:
	remove_wait_queue(&dev->w_wait, &wait);
	set_current_state(TASK_RUNNING);
	return ret;
}

static int globalfifo_fasync(int fd, struct file *filp, int mode)
{
	struct globalfifo_dev *dev = filp->private_data;
	return fasync_helper(fd, filp, mode, &dev->async_queue);
}


///Qus. 没有设置的回调调用会怎样？
//不会返回错误， 内核的接口函数会检查是否设置
static const struct file_operations globalfifo_fops = {
	.owner   = THIS_MODULE,
	.open    = globalfifo_open,
	.release = globalfifo_release,
	.read    = globalfifo_read,
	.write   = globalfifo_write,
};


/* load globalfifo_dev module */
//return 0 success, or negtive failure
static int __init globalfifo_init(void)
{
	int result;
	int err;
	dev_t devno = MKDEV(globalfifo_major, 0);

	/* applay cdev */
	if (globalfifo_major) /// EE 申请设备号
		result = register_chrdev_region(devno, 1, "globalfifo");
	else {
		/** EE 主动获得主设备号 */
		result = alloc_chrdev_region(&devno, 0, 1, "globalfifo");
		globalfifo_major = MAJOR(devno);
	}

	if (result < 0) 
		return result;

	devp = kmalloc(sizeof(struct globalfifo_dev), GFP_KERNEL);
	if (!devp) {
		result = -ENOMEM;
		goto fail_malloc;
	}
	memset(devp, 0, sizeof(struct globalfifo_dev));

	/* init cdev */
	cdev_init(&devp->cdev, &globalfifo_fops);
	devp->cdev.owner = THIS_MODULE; /* set owner */
	err = cdev_add(&devp->cdev, devno, 1);
	if (err) {
		printk(KERN_INFO "Error %d adding globalfifo", err);	
		return err;

	}

	/// init semaphore
	sema_init(&devp->sem, 1);
	///init wait_queue_head_t
	init_waitqueue_head(&devp->r_wait);
	init_waitqueue_head(&devp->w_wait);
	printk(KERN_INFO "globalfifo init success\n");

fail_malloc:
	unregister_chrdev_region(devno, 1);
	return result;
}

/* unload globalfifo_dev module */
static void __exit globalfifo_exit(void)
{
	cdev_del(&devp->cdev);
	/** EE 注销设备 */
	unregister_chrdev_region(MKDEV(globalfifo_major, 0), 1);
	kfree(devp);
	printk(KERN_INFO "globalfifo exit success\n");
}

MODULE_AUTHOR("Lili <lili.xxx.com>");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("A globalfifo Module");
MODULE_ALIAS("a simplest module");


module_init(globalfifo_init);
module_exit(globalfifo_exit);
