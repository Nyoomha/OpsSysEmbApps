#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kern_levels.h>

MODULE_LICENSE("GPL");

int ece_init(void){
    printk(KERN_INFO "ECE4310: Start Here \n");
    return 0;
}

void ece_end(void){
    printk(KERN_INFO "ECE4310: End Here \n");
}

module_init(ece_init);
module_exit(ece_end);