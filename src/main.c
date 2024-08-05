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
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/addr.h>
//
static uint32_t die_temp;
//Deklaracja LEDów
int ret_r;
	int ret_g;
	int ret_b; 
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)
#define LED3_NODE DT_ALIAS(led3)
static const struct gpio_dt_spec led1 = 
GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec led2 = 
GPIO_DT_SPEC_GET(LED2_NODE, gpios);
static const struct gpio_dt_spec led3 = 
GPIO_DT_SPEC_GET(LED3_NODE, gpios);
// Komunikaty do CMD
LOG_MODULE_REGISTER(Lesson2_Exercise1, LOG_LEVEL_INF);
//Deklaracja Bluetooth
#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)
//Struktura połączeń
struct bt_conn *my_conn = NULL;

//Struktura rozgłaszania
static struct bt_le_adv_param *adv_param = BT_LE_ADV_PARAM(
	(BT_LE_ADV_OPT_CONNECTABLE |
	 BT_LE_ADV_OPT_USE_IDENTITY), /* Connectable advertising and use identity address */
	BT_GAP_ADV_FAST_INT_MIN_1, /* 0x30 units, 48 units, 30ms */
	BT_GAP_ADV_FAST_INT_MAX_1, /* 0x60 units, 96 units, 60ms */
	NULL); /* Set to NULL for undirected advertising */


/* STEP 4.1.1 - Declare the advertising packet */
static const struct bt_data ad[] = {
	/* STEP 4.1.2 - Set the advertising flags */
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	/* STEP 4.1.3 - Set the advertising packet data  */
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),

};

/* STEP 4.2.2 - Declare the URL data to include in the scan response */
// static unsigned char url_data[] = {  0x01, /* URL Scheme Prefix http://www. */
// 		      'r', 'b', '.', 'g', 'y', '/', 'a', 'w', 'u', 'p', '8', 'b' };

/* STEP 4.2.1 - Declare the scan response packet */
static const struct bt_data sd[] = {
	
	BT_DATA_BYTES(BT_DATA_UUID128_ALL,
		      BT_UUID_128_ENCODE(0x00001523, 0x1212, 0xefde, 0x1523, 0x785feabcd123)),
};
//Jak połączono to zrób to
void on_connected(struct bt_conn *conn, uint8_t err)
{
	  
	
    if (err) {
        LOG_ERR("Connection error %d", err);
        return;
    }
    LOG_INF("Connected");
    my_conn = bt_conn_ref(conn);
	ret_r = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_INACTIVE);
	ret_g = gpio_pin_configure_dt(&led2, GPIO_OUTPUT_INACTIVE);
	ret_b= gpio_pin_configure_dt(&led3, GPIO_OUTPUT_INACTIVE);

    /* STEP 3.2  Turn the connection status LED on */
}
//Jak odłączono to
void on_disconnected(struct bt_conn *conn, uint8_t reason)
{
    LOG_INF("Disconnected. Reason %d", reason);
    bt_conn_unref(my_conn);
		ret_r = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
	ret_g = gpio_pin_configure_dt(&led2, GPIO_OUTPUT_ACTIVE);
	ret_b= gpio_pin_configure_dt(&led3, GPIO_OUTPUT_ACTIVE);

    /* STEP 3.3  Turn the connection status LED off */
}
struct bt_conn_cb connection_callbacks = {
    .connected              = on_connected,
    .disconnected           = on_disconnected,
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
	

	nrf52_die_temperature_read();
	float temp_c = (float)die_temp/4; 
	char buf[6];
	gcvt(temp_c,6,buf);
    printk("die temperatured = %s \r\n ",buf);
	printk("Elo wale wiadro \n");

	int blink_status = 0;
	int err;

	LOG_INF("Starting Lesson 3 - Exercise 1 \n");
	//Rejestracja wywołania
	/* STEP 6 - Start advertising */

	


	bt_conn_cb_register(&connection_callbacks);

	err = bt_enable(NULL);
	if (err) {
		LOG_ERR("Bluetooth init failed (err %d)", err);
		return -1;
	}

	LOG_INF("Bluetooth initialized");
	err = bt_le_adv_start(adv_param, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
	if (err) {
		LOG_ERR("Advertising failed to start (err %d)", err);
		return -1;
	}

	LOG_INF("Advertising successfully started");

	
}
