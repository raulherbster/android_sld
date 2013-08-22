#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/kdev_t.h>
#include <asm/uaccess.h>
#define MY_MACIG 'G'
#define READ_IOCTL _IOR(MY_MACIG, 0, int)
#define WRITE_IOCTL _IOW(MY_MACIG, 1, int)
static int major;
static char msg[200];

static int device_open(struct inode *inode, struct file *file)
{
  printk("at device_open \n");
  return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
  printk("at device_release\n");
  return 0;
}

static ssize_t
device_read (struct file *filp, char __user * buffer, size_t length,
	     loff_t * offset)
{
  return simple_read_from_buffer (buffer, length, offset, msg, 200);
}


static ssize_t
device_write (struct file *filp, const char __user * buff, size_t len,
	      loff_t * off)
{
  if (len > 199)
    return -EINVAL;
  copy_from_user (msg, buff, len);
  msg[len] = '\0';
  return len;
}

char buf[200];
long
device_ioctl (struct file *filep, unsigned int cmd,
	      unsigned long arg)
{
  int len = 200;
  printk ("at device_ioctl\n");
  switch (cmd)
    {
    case READ_IOCTL:
      copy_to_user ((char *) arg, buf, 200);
      break;

    case WRITE_IOCTL:
      copy_from_user (buf, (char *) arg, len);
      break;

    default:
      return -ENOTTY;
    }
  return len;

}

static struct file_operations fops = {
  .read = device_read,
  .write = device_write,
  .open = device_open,
  .release  = device_release,
  .unlocked_ioctl = device_ioctl,
};

static struct class *my_class;

static int __init
cdevexample_module_init (void)
{
  major = register_chrdev (0, "my_device", &fops);
  if (major < 0)
    {
      printk ("Registering the character device failed with %d\n", major);
      return major;
    }
  my_class = class_create(THIS_MODULE, "my_device");
  device_create(my_class, NULL, MKDEV(major,0), NULL, "my_device");

  printk ("registering module...\n");
  printk ("cdev example: assigned major: %d\n", major);
  printk ("create node with mknod /dev/cdev_example c %d 0\n", major);
  return 0;
}

static void __exit
cdevexample_module_exit (void)
{
  device_destroy(my_class,MKDEV(major,0));
  class_unregister(my_class);
  class_destroy(my_class);
  unregister_chrdev (major, "my_device");
  printk ("unregistering module...\n");
}

module_init (cdevexample_module_init);
module_exit (cdevexample_module_exit);
MODULE_LICENSE ("GPL");
