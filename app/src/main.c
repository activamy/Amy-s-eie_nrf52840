/*
 * main.c
 */

#include <zephyr/kernel.h>    //includes functions like k_msleep

#include <BTN.h>
#include <LED.h>

#define SLEEP_TIME_MS 100 /*delay known as software debouncing, 
                            sometimes the signal when clicking will bounce, causing
                            the button to 'flicker', so adding a delay will prevent
                            the system to see a double click and will check it once 
                            before sleeping and then retries
                            
                            test for ideal*/
void all_led_off();

// MAIN
int main(void) 
{
  int i = 0;
  
  if (BTN_init() < 0) {
    return 0;
  }

  if (LED_init() < 0) {
    return 0;
  }

  while (1) {
    if (BTN_check_clear_pressed(BTN0)) {
      // Better for counters selectors and toggling, triggers once per press, 
      // ignores how long it's been held down
      // avoids a 'double click'
      
      i++;
      all_led_off();

      // 16th click resets counter, decimal 16 == binary 0
      if (i == 16) {
        all_led_off();
        i = 0;
        printk("0 \n %d \n", i);

      }

      // LSB (Col D of Truth Table)
      if (i % 2 == 1 && i != 0) {
        LED_toggle(LED3);
        printk("D column of truth table \n %d \n", i);
      }

      // (Col C of Truth Table)
      if ( i == 2 || i == 3 || i == 6 || i == 7 || i == 10 || i == 11 || i == 14 || i == 15) {
        LED_toggle(LED2);
        printk("C column of truth table \n %d \n", i);
      }

      // (Col B of Truth Table)
      if (((i >= 4) && (i <= 7)) || ((i >= 12) && (i < 16))) {
        LED_toggle(LED1);
        printk("B col of truth table \n %d \n", i);
      }

      // MSB (Col A of Truth Table)
      if ((i >= 8) && (i < 16)) {
        LED_toggle(LED0);
        printk("Col A of table \n %d \n", i);
      }
  
    k_msleep(SLEEP_TIME_MS);
    }

  }
  return 0;
}



//Functions
void all_led_off()
{
    LED_set(LED0, LED_OFF);
    LED_set(LED1, LED_OFF);    
    LED_set(LED2, LED_OFF);
    LED_set(LED3, LED_OFF);
}