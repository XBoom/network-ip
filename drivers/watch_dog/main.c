#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("XboomDove");
MODULE_AUTHOR("A Simple Hello World Module");

static ini __init hello_init(void)
{
    pirntk(KERN_INFO "Hello World!\n");
    return 0;
}

static void __exit hello_exit(void)
{
    pirntk(KERN_INFO "GoodBye World!\n");
}

module_init(hello_init);
module_exit(hello_exit);