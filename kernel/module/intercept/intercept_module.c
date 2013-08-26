#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/unistd.h>
#include <linux/sched.h>
#include <asm/uaccess.h>
#include <asm/stat.h>

#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/netlink.h>
#include <linux/init.h>
#include <linux/rcupdate.h>
#include <linux/fdtable.h>
#include <linux/fs.h>
#include <linux/fs_struct.h>
#include <linux/dcache.h>
#include <linux/slab.h>

#include <linux/fs.h>
#include <asm/segment.h>
#include <linux/buffer_head.h>

MODULE_LICENSE ("Dual BSD/GPL");

void __user *sample;

void **sys_call_table;

/* Back up pointer towards orginal sys_seal() method */

asmlinkage long (*original_call_seal) ();



/* Hooked sys_read() method definition */

asmlinkage long
our_sys_seal (unsigned int fd, char *buf, size_t count)
{
  printk ("At our_sys_seal\n");

  return 53;
}

/* Init module */

int
init_module ()
{
  // sys_call_table address might change. Check on System.map
  sys_call_table = (void *) 0xc0027f84;

  original_call_seal = sys_call_table[__NR_seal];

  sys_call_table[__NR_seal] = our_sys_seal;

  printk ("Inserted!!\n");

  return 0;
}

/* CLEAN_UP Module */

void
cleanup_module ()
{
  printk ("Clean up!!\n");

  sys_call_table[__NR_seal] = original_call_seal;

}
