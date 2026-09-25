/**
 * @file main.c
 */

#include <inttypes.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/printk.h>


#include "BTN.h"
#include "LED.h"
#include "states.h"

#define SLEEP_MS 1


const struct device *uart = DEVICE_DT_GET(DT_NODELABEL(UART0));

if (!device_id_ready(uart)) {
  return;
}



int main(void) {
  if (0 > BTN_init()) {
    return 0;
  }
  if (0 > LED_init()) {
    return 0;
  }

  crochet_counter_init();
  
  while (1) {
    int ret = crochet_counter_run();
    if (0 > ret) {
      return 0;
    }

    k_msleep(SLEEP_MS);
  }
  return 0;
}
