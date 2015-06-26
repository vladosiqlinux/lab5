#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/timer.h>

static struct kobject *timer_kobj;
static struct timer_list timer;

int time = 0;


void timer_callback(unsigned long arg)
{
	printk("Hello, world!\n");
    	mod_timer(&timer, jiffies + msecs_to_jiffies(time)); 
}

static ssize_t timer_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
        return sprintf(buf, "%d\n", time);
}

static ssize_t timer_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
        sscanf(buf, "%d", &time);

       	if (time == 0)
		del_timer_sync(&timer);
	else
		mod_timer(&timer, jiffies + msecs_to_jiffies(time)); 

        return count;
}

static struct kobj_attribute timer_attribute = __ATTR(time, 0666, timer_show, timer_store);
 
static struct attribute *attrs[] = {
        &timer_attribute.attr,
        NULL,
};

static struct attribute_group attr_group = {
        .attrs = attrs,
};
 
static int __init timer_init(void)
{
       int retval;

       timer_kobj = kobject_create_and_add("timer", kernel_kobj);
       if (!timer_kobj)
               return -ENOMEM;

       retval = sysfs_create_group(timer_kobj, &attr_group);
       if (retval)
               kobject_put(timer_kobj);

       setup_timer(&timer, timer_callback, 0);

       return retval;
}

static void __exit timer_exit(void)
{
	del_timer_sync(&timer);
        kobject_put(timer_kobj);
}

module_init(timer_init);
module_exit(timer_exit);
MODULE_LICENSE("GPL");
