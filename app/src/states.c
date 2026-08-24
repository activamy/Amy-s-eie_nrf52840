/*
* states file
*/

#include <zephyr/smf.h>

#include "states.h"
#include "BTN.h"
#include "LED.h"



/*-------------------------------
 * Function Prototypes
 *-----------------------------*/
static void start_entry(void* o);
static enum smf_state_result start_run(void* o);

static void st_inc_entry(void* o);
static enum smf_state_result st_inc_run(void* o);

// might not need
// static void st_dec_entry(void* o);
// static enum smf_state_result st_dec_run(void* o);

static void rnd_inc_entry(void* o);
static enum smf_state_result rnd_inc_run(void* o);

static void rnd_dec_entry(void* o);
static enum smf_state_result rnd_dec_run(void* o);

static enum smf_state_result reset_run(void* o);


/*-------------------------------
 * Extra Functions
 *-----------------------------*/
void led_off();
void led_initialize();

void transmit_result(int st_count, int rnd_count);


 /*-------------------------------
 * Typedefs
 *-----------------------------*/
 enum states {
 	START,              //everything is at 0 
 	ST_INC,             // st++
 	// ST_DEC,          // st--
 	RND_INC,            // rnd++
 	RND_DEC,            //rnd--
 	RESET               // either st or rnd goes to 0
 };

typedef struct {
    struct smf_ctx ctx;
    int st_count, rnd_count; 
    int reset_flag;
} cc_objects_t;


/*-------------------------------
 * Local Variables
 *-----------------------------*/
static const struct smf_state states[] = {
    [START] = SMF_CREATE_STATE(start_entry, start_run, NULL, NULL, NULL),
    [ST_INC] = SMF_CREATE_STATE(st_inc_entry, st_inc_run, NULL, NULL, NULL),
    // [ST_DEC] = SMF_CREATE_STATE(st_dec_entry, st_dec_run, NULL, NULL, NULL),
    [RND_INC] = SMF_CREATE_STATE(rnd_inc_entry, rnd_inc_run, NULL, NULL, NULL),
    [RND_DEC] = SMF_CREATE_STATE(rnd_dec_entry, rnd_dec_run, NULL, NULL, NULL),
    [RESET] = SMF_CREATE_STATE(NULL, reset_run, NULL, NULL, NULL)
};

static cc_objects_t cc_obj;


/*-------------------------------
 * State Definitions
 *-----------------------------*/
void crochet_counter_init() {
    cc_obj.st_count = 0;
    cc_obj.rnd_count = 0;
    cc_obj.reset_flag = 0;
    led_off();
    smf_set_initial(SMF_CTX(&cc_obj), &states[START])
}

int crochet_counter_run() {
    return smf_run_state(SMF_CTX(&cc_obj));
}


// Start
static void start_entry(void* o) {
    cc_obj.st_count = 0;
    cc_obj.rnd_count = 0;
    cc_obj.reset_flag = 0;

    led_initialize();
}

static enum smf_state_result start_run(void*o) {
    if (BTN_check_clear_pressed(BTN0)) {
        smf_set_state(SMF_CTX(&cc_obj), &states[ST_INC]);
    }
    else if (BTN_check_clear_pressed(BTN1)) {
        smf_set_state(SMF_CTX(&cc_obj), &states[RND_INC]);
    }

    return SMF_EVENT_HANDLED;
}


// Increase Stitch
static void st_inc_entry(void* o) {
    led_off();
    cc_obj.st_count++;
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
    kmsleep(100);
    LED_set(LED1, 1);
    kmsleep(100);
    LED_set(LED2, 1);
    kmsleep(100);
}

void transmit_result(int st_count, int rnd_count)