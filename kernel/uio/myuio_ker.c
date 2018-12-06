/**
 * @file     myuio-driver.c
 *           
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     12/05/2018 11:48:29 PM
 *
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h> 
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/uio_driver.h> 
#include <linux/slab.h> /*  kmalloc, kfree */

struct uio_info uio_virtual_device_info =
{ .name = "myuio", .version = "1.0", .irq = UIO_IRQ_NONE, };



static int uio_virtual_device_drv_probe(struct platform_device *pdev) 
{ 
    printk("uio_virtual_device_probe( %p)\n", pdev);

    uio_virtual_device_info.mem[0].addr = (unsigned long)kmalloc(1024,GFP_KERNEL);

    if(uio_virtual_device_info.mem[0].addr == 0) return -ENOMEM;

    uio_virtual_device_info.mem[0].memtype = UIO_MEM_LOGICAL; //逻辑内存
    uio_virtual_device_info.mem[0].size = 1024;

    printk("[%s,%d] uio_virtual_device_info.mem[0].addr:0x%llx,.size :%llu\n", __func__,__LINE__,
           uio_virtual_device_info.mem[0].addr, uio_virtual_device_info.mem[0].size);

    if(uio_register_device(&pdev->dev, &uio_virtual_device_info)) return -ENODEV;

    return 0;
}

static int uio_virtual_device_drv_remove(struct platform_device *pdev) {
    uio_unregister_device(&uio_virtual_device_info);
    return 0;
} 

static struct platform_driver virtual_device_drv = {
    .probe = uio_virtual_device_drv_probe,
    .remove = uio_virtual_device_drv_remove, 
    .driver = { .name = "VIRTUAL_DEVICE", .owner = THIS_MODULE, } 
};

static void virtual_device_remove(struct device *dev) { }

static struct platform_device virtual_device = { 
    .name = "VIRTUAL_DEVICE", 
    .id = -1, 
    .dev = { .release = virtual_device_remove, }, };

static int __init uio_virtual_device_init(void) { 
    printk("virtual_device init ok!\n");
    platform_device_register(&virtual_device);
    printk("virtual_device_drv init ok!\n");
    return platform_driver_register(&virtual_device_drv);
} 

static void __exit uio_virtual_device_exit(void) {

    //((char*)uio_virtual_device_info.mem[0].addr)[100] = '\0';
    printk("Virtual_device remove ok! %llx %s\n",
           uio_virtual_device_info.mem[0].addr,
           (char*)uio_virtual_device_info.mem[0].addr+1);//去读用户侧设置 //读不到??
    /*//是否有泄露??                               */
    /*kfree((void*)uio_virtual_device_info.mem[0].addr);*/
    platform_device_unregister(&virtual_device);
    printk("virtual_device_drv remove ok!\n");
    platform_driver_unregister(&virtual_device_drv);
} 

module_init(uio_virtual_device_init);
module_exit(uio_virtual_device_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ZP1015");
MODULE_DESCRIPTION("Demon of UIO");



