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

#include <linux/sched.h>

MODULE_LICENSE ("Dual BSD/GPL");

void __user *sample;

void **sys_call_table;

/* Back up pointer towards orginal sys_seal() method */

asmlinkage long (*original_call_seal)();

asmlinkage long (*original_call_is_sealed)();

asmlinkage long (*original_call_read)();

asmlinkage long (*original_call_write)();

asmlinkage long (*original_call_open)();

asmlinkage long (*original_call_close)();

/* Hooked sys_read() method definition */

asmlinkage long our_sys_seal(void) {
	printk("At our_sys_seal\n");

	if (!current->sealed) {
		current->sealed = 1;
	}

	return 53;
}

/* Init module */

asmlinkage long our_sys_is_sealed(void) {
	printk("At our_sys_is_sealed\n");

	return current->sealed;
}

asmlinkage long
our_sys_read (unsigned int fd, char __user *buf, size_t count)
{
	printk ("At our_sys_read\n");

	if (current->sealed) {
		printk("Read data in safe mode");
		return 0;
	} else {
		return original_call_read(fd,buf,count);
	}

	return 0;
}

asmlinkage long
our_sys_write (unsigned int fd, const char __user *buf, size_t count)
{
	printk ("At our_sys_write\n");

	if (current->sealed) {
		printk("Write data in safe mode");
		return 0;
	} else {
		return original_call_write(fd,buf,count);
	}
}

asmlinkage long
our_sys_open (const char __user *filename, int flags, int mode)
{
	printk ("At our_sys_open\n");

	if (current->sealed) {
		printk("Read data in safe mode");
		return 0;
	} else {
		return original_call_open(filename,flags,mode);
	}

}

asmlinkage long our_sys_close(unsigned int fd) {
	printk("At our_sys_close\n");

	if (current->sealed) {
		printk("Read data in safe mode");
		return 0;
	} else {
		return original_call_close(fd);
	}
}

int init_module() {
	// sys_call_table address might change. Check on System.map
	sys_call_table = (void *) 0xc0027f84;

	// save original calls to set them back
	original_call_seal = sys_call_table[__NR_seal];
	original_call_is_sealed = sys_call_table[__NR_is_sealed];
	original_call_read = sys_call_table[__NR_read];
	original_call_write = sys_call_table[__NR_write];
	original_call_open = sys_call_table[__NR_open];
	original_call_close = sys_call_table[__NR_close];

	// change the syscall table to include the new functions
	sys_call_table[__NR_seal] = our_sys_seal;
	sys_call_table[__NR_is_sealed] = our_sys_is_sealed;
	sys_call_table[__NR_open] = our_sys_open;
	sys_call_table[__NR_close] = our_sys_close;
	sys_call_table[__NR_read] = our_sys_read;
	sys_call_table[__NR_write] = our_sys_write;

	printk("New functions inserted!!\n");

	return 0;
}

/* CLEAN_UP Module */

void cleanup_module() {
	// write back the original functions into the syscall table
	sys_call_table[__NR_seal] = original_call_seal;
	sys_call_table[__NR_is_sealed] = original_call_is_sealed;
	sys_call_table[__NR_read] = original_call_read;
	sys_call_table[__NR_write] = original_call_write;
	sys_call_table[__NR_open] = original_call_open;
	sys_call_table[__NR_close] = original_call_close;

	printk("Clean up!!\n");
}
