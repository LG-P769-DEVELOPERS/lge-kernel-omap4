/*
 * Author: Artur Załęski (artas182x) <artas182x@gmail.com>
 *
 * Copyright 2014 artas182x
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>

#include <linux/mutex.h>

static DEFINE_MUTEX(battery_mutex);

bool battery_friend_active __read_mostly = false;


static ssize_t battery_friend_active_show(struct kobject *kobj,
		struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%u\n", (battery_friend_active ? 1 : 0));
}


static ssize_t battery_friend_active_store(struct kobject *kobj,
		struct kobj_attribute *attr, const char *buf, size_t count)
{
	unsigned int data;

	if(sscanf(buf, "%u\n", &data) == 1) {
		if (data == 1) {
			battery_friend_active = true;
		}
		else if (data == 0) {
			battery_friend_active = false;
		}
	} 


	return count;
}


static struct kobj_attribute battery_friend_active_attribute = 
	__ATTR(battery_friend_active, 0666,
		battery_friend_active_show,
		battery_friend_active_store);



static struct attribute *battery_friend_active_attrs[] =
	{
		&battery_friend_active_attribute.attr,
		NULL,
	};

static struct attribute_group battery_friend_active_attr_group =
	{
		.attrs = battery_friend_active_attrs,
	};

static struct kobject *battery_friend_kobj;



static int battery_friend_init(void)
{
	int sysfs_result;


	battery_friend_kobj = kobject_create_and_add("battery_friend", kernel_kobj);
	if (!battery_friend_kobj) {
		return -ENOMEM;
        }

	sysfs_result = sysfs_create_group(battery_friend_kobj,
			&battery_friend_active_attr_group);

        if (sysfs_result) {
		kobject_put(battery_friend_kobj);
	}
	return sysfs_result;
}

static void battery_friend_exit(void)
{

	if (battery_friend_kobj != NULL)
		kobject_put(battery_friend_kobj);
}

module_init(battery_friend_init);
module_exit(battery_friend_exit);
