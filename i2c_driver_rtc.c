/*
 * Name: Yogesh
 * Date: 23/1/24
 * Code: i2c driver for rtc
 */

#include<linux/module.h>
#include<linux/init.h>
#include<linux/slab.h>
#include<linux/i2c.h>
#include<linux/delay.h>
#include<linux/kernel.h>
#include<linux/rtc.h>



#define I2C_AVAILABLE 1
#define SLAVE_DEVICE_NAME "RTC"
#define RTC_SLAVE_ADDR 0xd0


static struct i2c_adapter *rtc_i2c_adapter = NULL;

static struct i2c_client *rtc_i2c_client = NULL;






static int rtc_probe(struct i2c_client *client,const struct i2c_device_id *id)
{
	pr_info("Rtc probed\n");
	return 0;
}

static int rtc_remove(struct i2c_client *client)
{
	pr_info("rtc removed\n");
	return 0;
}


static const struct i2c_device_id rtc_id[] = {
	{SLAVE_DEVICE_NAME,0},
	{}
};

MODULE_DEVICE_TABLE(i2c,rtc_id);


static struct i2c_driver rtc_driver = {
	.driver = {
		.name = SLAVE_DEVICE_NAME,
		.owner = THIS_MODULE,
	},
	.probe = rtc_probe,
	.remove = rtc_remove,
	.id_table = rtc_id,
};



static struct i2c_board_info rtc_i2c_board_info = {
	I2C_BOARD_INFO(SLAVE_DEVICE_NAME,RTC_SLAVE_ADDR)
};

static int __init rtc_driver_init(void)
{
	int ret = -1;
	rtc_i2c_adapter = i2c_get_adapter(I2C_AVAILABLE);

	if(rtc_i2c_adapter!=NULL)
	{
		rtc_i2c_client = i2c_new_client_device(rtc_i2c_adapter,&rtc_i2c_board_info);

		if(rtc_i2c_client!=NULL)
		{
			i2c_add_driver(&rtc_driver);
			ret = 0;
		}
		i2c_put_adapter(rtc_i2c_adapter);
	}


	pr_info("RTC driver with I2C support registered\n");
	


	pr_info("RTC device created\n");

	return ret;

}



static void __exit rtc_driver_exit(void)
{
	i2c_del_driver(&rtc_driver);
	i2c_unregister_device(rtc_i2c_client);

	pr_info("Driver removed\n");

}

module_init(rtc_driver_init);
module_exit(rtc_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yogesh");
MODULE_DESCRIPTION("Simple RTC Driver");


