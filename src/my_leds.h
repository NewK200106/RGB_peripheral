#include <zephyr/drivers/gpio.h>
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
#define R_ON gpio_pin_configure_dt(&led1, GPIO_OUTPUT_INACTIVE)
#define R_OFF gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE)
#define G_ON gpio_pin_configure_dt(&led2, GPIO_OUTPUT_INACTIVE)
#define G_OFF gpio_pin_configure_dt(&led2, GPIO_OUTPUT_ACTIVE)
#define B_OFF  gpio_pin_configure_dt(&led3, GPIO_OUTPUT_ACTIVE)
#define B_ON gpio_pin_configure_dt(&led3, GPIO_OUTPUT_INACTIVE)