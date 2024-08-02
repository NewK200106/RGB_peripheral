/*
 * Copyright (c) 2023 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <stdio.h>
#include <zephyr/bluetooth/bluetooth.h>

#include <stdint.h>
#include <zephyr/bluetooth/gap.h>
//
static uint32_t die_temp;
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)
#define LED3_NODE DT_ALIAS(led3)
//
static const struct gpio_dt_spec led1 = 
GPIO_DT_SPEC_GET(LED1_NODE, gpios);

static const struct gpio_dt_spec led2 = 
GPIO_DT_SPEC_GET(LED2_NODE, gpios);
static const struct gpio_dt_spec led3 = 
GPIO_DT_SPEC_GET(LED3_NODE, gpios);
//
LOG_MODULE_REGISTER(Lesson2_Exercise1, LOG_LEVEL_INF);

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)



/* STEP 4.1.1 - Declare the advertising packet */
static const struct bt_data ad[] = {
	/* STEP 4.1.2 - Set the advertising flags */
	BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR),
	/* STEP 4.1.3 - Set the advertising packet data  */
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),

};

/* STEP 4.2.2 - Declare the URL data to include in the scan response */
static unsigned char url_data[] = {  0x01, /* URL Scheme Prefix http://www. */
		      'r', 'b', '.', 'g', 'y', '/', 'a', 'w', 'u', 'p', '8', 'b' };

/* STEP 4.2.1 - Declare the scan response packet */
static const struct bt_data sd[] = {
	/* 4.2.3 Include the URL data in the scan response packet */
	BT_DATA(BT_DATA_URI, url_data, sizeof(url_data)),
};
void nrf52_die_temperature_read(void)
{
    NRF_TEMP->TASKS_START = 1;//Start temperature measurement
    while (NRF_TEMP->EVENTS_DATARDY == 0) {}
    NRF_TEMP->EVENTS_DATARDY = 0;//Temperature measurement complete, data ready
    die_temp = NRF_TEMP->TEMP ;
    NRF_TEMP->TASKS_STOP = 1;  //Stop temperature measurement
}
int main(void)
{
	  
	int ret_r;
	int ret_g;
	int ret_b; 

	nrf52_die_temperature_read();
	float temp_c = (float)die_temp/4; 
	char buf[6];
	gcvt(temp_c,6,buf);
    printk("die temperatured = %s \r\n ",buf);
	printk("Elo wale wiadro \n");

	int blink_status = 0;
	int err;

	LOG_INF("Starting Lesson 2 - Exercise 1 \n");

	
	/* STEP 5 - Enable the Bluetooth LE stack */
	err = bt_enable(NULL);
	if (err) {
		LOG_ERR("Bluetooth init failed (err %d)\n", err);
		return -1;
	}

	LOG_INF("Bluetooth initialized\n");

	/* STEP 6 - Start advertising */
	err = bt_le_adv_start(BT_LE_ADV_NCONN, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
		ret_r = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_INACTIVE);

	ret_g = gpio_pin_configure_dt(&led2, GPIO_OUTPUT_INACTIVE);
	
	ret_b= gpio_pin_configure_dt(&led3, GPIO_OUTPUT_INACTIVE);


	if (err) {
		LOG_ERR("Advertising failed to start (err %d)\n", err);
		return -1;
	}

	LOG_INF("Advertising successfully started\n");

	
}
