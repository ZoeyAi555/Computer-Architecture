//hardware I/O port in a kernel module


/*
This module requests access to I/O port 0x378 (the standard parallel port), 
reads a byte from the port when the module is loaded, 
and releases the port when the module is unloaded. 

You can load and unload the module with sudo insmod hello.ko and sudo rmmod hello.
*/
#include <linux/init.h>
#include <linux/module.h>
#include <asm/io.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple Linux driver");
MODULE_VERSION("0.1");

#define PORT 0x378 /* Parallel port */

static int __init hello_init(void) {
    unsigned char data;

    if (!request_region(PORT, 1, "hello")) {
        printk(KERN_INFO "hello: cannot reserve port 0x378\n");
        return -ENODEV;
    }

    data = inb(PORT);
    printk(KERN_INFO "hello: read %u from port 0x378\n", data);

    return 0;
}

static void __exit hello_exit(void) {
    release_region(PORT, 1);
    printk(KERN_INFO "Goodbye, world!\n");
}

module_init(hello_init);
module_exit(hello_exit);
