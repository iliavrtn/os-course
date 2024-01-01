#include <linux/kernel.h> // kernal work generate
#include <linux/module.h> // to specific module
#include <linux/uaccess.h> // for get_user and put_user
#include <linux/fs.h> // for register_chrdev
MODULE_LICENSE("GPL");
#include "iocdev.h"
#define DEVICE_RANGE_NAME "hw5"
#define buffer_len 100


static int N;
static char message[buffer_len] = { 0 };
static int Mj;


// function declarations
void write_bytes(char* buff);
int read_bytes(char* buff, int len, int offset);


// output for user
static ssize_t device_read(struct file* file,
	char __user* buffer,
	size_t length,
	loff_t* offset)
{
	int i, r_Val;
	printk("Invoking device read(%p,%ld): %s\n", file, length, message);
	r_Val = read_bytes(buffer, length, *offset);
	if (r_Val == -1)
	{
		printk("Error reading.\n");
		return -1;
	}
	else {
		*offset += i;
	}
	return 0;
}

// reads from user
static ssize_t device_write(struct file* file,
	const char __user* buffer,
	size_t length,
	loff_t* offset)
{
	int i;
	printk("Invoking device write(%p,%ld)\n", file, length);
	if (length > N)
	{
		printk("Writing more then possible\n");
		return -1;
	}
	write_bytes(buffer);
	N = N - length; // how many bytes left
	return 0;
}

static long device_ioctl(struct file* file,
	unsigned int ioctl_command_id,
	unsigned long ioctl_param)
{
	// user can change N by using ioctl command
	N = ioctl_param;
	return 0;
}

int device_open(struct inode* inode, struct file* filp)
{
	filp->private_data = message;
	// as requested: N initialized with 100
	N = 100;
	return 0;
}

// struct
struct file_operations fops = {
 .owner = THIS_MODULE,
 .read = device_read,
 .write = device_write,
 .unlocked_ioctl = device_ioctl,
 .open = device_open,
 .release = NULL,
};

int init_module(void)
{
	Mj = register_chrdev(0, DEVICE_RANGE_NAME, &fops);
	if (Mj < 0) {
		printk(KERN_ALERT "%s registartion failed is %d\n",
			DEVICE_RANGE_NAME, Mj);
		return Mj;
	}
	// prints to kernal
	printk("Registration was successful\n");
	printk("Major device number is: %d\n", Mj);
	printk("For talking to the device driver please create a device file\n");
	printk("mknod /dev/<name> c %d 0\n", Mj);
	printk("Use echo/cat to/form device file\n");
	printk("Please do not forget to rm the device file and also rmmod the module\n");
	return 0;
}

void cleanup_module(void)
{
	unregister_chrdev(Mj, DEVICE_RANGE_NAME);
	printk("Module is %s unregistered\n", DEVICE_RANGE_NAME);
}
