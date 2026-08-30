/*
* states file
*/

#include <zephyr/smf.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>


#include "states.h"
#include "BTN.h"
#include "LED.h"



/*-------------------------------
 * Function Prototypes
 *-----------------------------*/
static enum smf_state_result start_run(void* o);

static void btn_check_entry(void* o);
static enum smf_state_result btn_check_run(void* o);

static enum smf_state_result inc_run(void* o);

static enum smf_state_result dec_run(void* o);

static void reset_entry(void* o);
static enum smf_state_result reset_run(void* o);


/*-------------------------------
 * Extra Functions
 *-----------------------------*/
void led_off();
void led_initialize();
void led_reset();

// void transmit_result(int st_count, int rnd_count);


 /*-------------------------------
 * Typedefs
 *-----------------------------*/
 enum states {
    START,
 	BTN_CHECK,          // trigger a flag based on btn
 	INC,                // inc
 	DEC,                // dec
 	RESET               // either st or rnd goes to 0
 };

typedef struct {
    struct smf_ctx ctx;
    int st_count, rnd_count; 

    int st, rnd;

    int time_start;

} cc_objects_t;


/*-------------------------------
 * Local Variables
 *-----------------------------*/
static const struct smf_state states[] = {
    [START] = SMF_CREATE_STATE(NULL, start_run, NULL, NULL, NULL),
    [BTN_CHECK] = SMF_CREATE_STATE(btn_check_entry, btn_check_run, NULL, NULL, NULL),
    [INC] = SMF_CREATE_STATE(NULL, inc_run, NULL, NULL, NULL),
    [DEC] = SMF_CREATE_STATE(NULL, dec_run, NULL, NULL, NULL),
    [RESET] = SMF_CREATE_STATE(reset_entry, reset_run, NULL, NULL, NULL)
};

static cc_objects_t cc_obj;


/*-------------------------------
 * State Definitions
 *-----------------------------*/
void crochet_counter_init() {
    cc_obj.st_count = 0;
    cc_obj.rnd_count = 0;

    cc_obj.st = 0;
    cc_obj.rnd = 0;

    cc_obj.time_start = 0;

    smf_set_initial(SMF_CTX(&cc_obj), &states[START]);
}

int crochet_counter_run() {
    return smf_run_state(SMF_CTX(&cc_obj));
}

// Start
static enum smf_state_result start_run(void *o) {
    led_initialize();
    printk("Starting! \n");

    smf_set_state(SMF_CTX(&cc_obj), &states[BTN_CHECK]);

    return SMF_EVENT_HANDLED;
}
// ensure led is off
static void btn_check_entry(void* o) {
    led_off();
    cc_obj.time_start = 0;

    cc_obj.st = 0;
    cc_obj.rnd = 0;
}

static enum smf_state_result btn_check_run(void* o) {
    if (BTN_is_pressed(BTN0)) {                         // working with the stitch
        cc_obj.st = 1;
        if (cc_obj.time_start == 0) {
            cc_obj.time_start = k_uptime_get();         
        }
    }
    else if (BTN_is_pressed(BTN1)) {                   // working with the round
        cc_obj.rnd = 1;
        if (cc_obj.time_start == 0) {
            cc_obj.time_start = k_uptime_get();
        }
    }

    //check whether the intention is an inc, dec, or reset to zero
    if (((!BTN_is_pressed(BTN0)) && !BTN_is_pressed(BTN1)) && (cc_obj.time_start != 0)) {
        if ((k_uptime_get() - cc_obj.time_start) >= 1000) {
            smf_set_state(SMF_CTX(&cc_obj), &states[RESET]);
        }
        else if ((k_uptime_get() - cc_obj.time_start >= 300) && ((k_uptime_get() - cc_obj.time_start) < 4000)) {
            smf_set_state(SMF_CTX(&cc_obj), &states[DEC]);
        }
        else if ((k_uptime_get() - cc_obj.time_start >= 0) && ((k_uptime_get() - cc_obj.time_start) < 1500)) {
            smf_set_state(SMF_CTX(&cc_obj), &states[INC]);
        }
    }

    return SMF_EVENT_HANDLED;
}


// INC
static enum smf_state_result inc_run(void* o) {
    if (cc_obj.st == 1) {
        cc_obj.st_count++;
        printk("st = %d\n", cc_obj.st_count);
    }
    else if (cc_obj.rnd == 1) {
        cc_obj.rnd_count++;
        printk("rnd = %d\n", cc_obj.rnd_count);
    }
    smf_set_state(SMF_CTX(&cc_obj), &states[BTN_CHECK]);
    return SMF_EVENT_HANDLED;
}


// DEC
static enum smf_state_result dec_run(void* o) {
    if (cc_obj.st == 1) {
        cc_obj.st_count--;
        printk("st = %d\n", cc_obj.st_count);
    }
    else if (cc_obj.rnd == 1) {
        cc_obj.rnd_count--;
        printk("rnd = %d\n", cc_obj.rnd_count);
    }
    smf_set_state(SMF_CTX(&cc_obj), &states[BTN_CHECK]);
    return SMF_EVENT_HANDLED;
}


// RESET
static void reset_entry(void*o) {  
    led_reset();
}

static enum smf_state_result reset_run(void* o) {
    if (cc_obj.st == 1) {
        cc_obj.st_count = 0;
        printk("st cleared\n");
    }
    else if (cc_obj.rnd == 1) {
        cc_obj.rnd_count = 0;
        printk("rnd cleared!\n");

    }
    smf_set_state(SMF_CTX(&cc_obj), &states[BTN_CHECK]);
    return SMF_EVENT_HANDLED;
}



/*-------------------------------
 * Functions
 *-----------------------------*/
void led_off() {
    LED_set (LED0, 0);
    LED_set(LED1, 0);
    LED_set(LED2, 0);
    LED_set(LED3, 0);
}

void led_initialize() {
    LED_set(LED0, 1);
    k_msleep(1500);
    LED_set(LED1, 1);
    k_msleep(1500);
    LED_set(LED2, 1);
    k_msleep(1500);
    LED_set(LED3, 1);
}

void led_reset() {
    LED_set(LED0, 1);
    LED_set(LED1, 1);
    LED_set(LED2, 1);
    LED_set(LED3, 1);
}

// void transmit_result(int st_count, int rnd_count)