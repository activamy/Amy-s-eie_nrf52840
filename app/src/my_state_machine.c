/**
 * @file my_state_machine.c
 */

#include <zephyr/smf.h>

#include "LED.h"
#include "my_state_machine.h"

/*---------------------------------------------------------
 * Function Prototypes  - defines the entry, running and exits
 *-------------------------------------------------------*/

static void s0_state_entry(void* o);
static enum smf_state_result s0_state_run(void* o);
static void s0_state_exit(void* o);

static void s1_state_entry(void* o);
static enum smf_state_result s1_state_run(void* o);
static void s1_state_exit(void* o);

static void s2_state_entry(void* o);
static enum smf_state_result s2_state_run(void* o);
static void s2_state_exit(void* o);

static void s3_state_entry(void* o);
static enum smf_state_result s3_state_run(void* o);
static void s3_state_exit(void* o);

static void s4_state_entry(void* o);
static enum smf_state_result s4_state_run(void* o);
static void s4_state_exit(void* o);


/*---------------------------------------------------------
 * Typedefs
 *-------------------------------------------------------*/
// enum assigns numbers
enum state_diagram_machine_states {     // 'defines' all the states
    ALL_LEDS_OFF_STATE,
    LED_1_BLINK_4HZ_STATE,
    LED_ODD_ON_STATE,
    LED_EVEN_ON_STATE,
    ALL_LED_BLINK_16HZ_STATE
};

typedef struct {
    struct smf_ctx ctx;
    
    uint16_t count;

}s_state_objects_t;

/*---------------------------------------------------------
 * Local Variables
 *-------------------------------------------------------*/
static const struct smf_state s_states[] = {
    [ALL_LEDS_OFF_STATE] = SMF_CREATE_STATE(s0_state_entry, s0_state_run, s0_state_exit, NULL, NULL),
    [LED_1_BLINK_4HZ_STATE] = SMF_CREATE_STATE(s1_state_entry, s1_state_run, s1_state_exit, NULL, NULL),   
    [LED_ODD_ON_STATE] = SMF_CREATE_STATE(s2_state_entry, s2_state_run, s2_state_exit, NULL, NULL),
    [LED_EVEN_ON_STATE] = SMF_CREATE_STATE(s3_state_entry, s3_state_run, s3_state_exit, NULL, NULL),
    [ALL_LED_BLINK_16HZ_STATE] = SMF_CREATE_STATE(s4_state_entry, s4_state_run, s4_state_exit, NULL, NULL)
};


static s_state_objects_t s_state_objects;

void state_machine_init() {
    s_state_objects.count = 0;
    smf_set_initial(SMF_CTX(&s_state_objects), &s_states[ALL_LEDS_OFF_STATE]);
}

int state_machine_run() {
    return smf_run_state(SMF_CTX(&s_state_objects));
}


static void s0_state_entry(void* o){
    
}

static enum smf_state_result s0_state_run(void* o);
static void s0_state_exit(void* o);

static void s1_state_entry(void* o);
static enum smf_state_result s1_state_run(void* o);
static void s1_state_exit(void* o);

static void s2_state_entry(void* o);
static enum smf_state_result s2_state_run(void* o);
static void s2_state_exit(void* o);

static void s3_state_entry(void* o);
static enum smf_state_result s3_state_run(void* o);
static void s3_state_exit(void* o);

static void s4_state_entry(void* o);
static enum smf_state_result s4_state_run(void* o);
static void s4_state_exit(void* o);


// typedef struct { // this is like a toolbox that is shared throughout the program, like a save file
//     // Context variable used by zephyr to track state machine state. Must be first
//     struct smf_ctx ctx;

//     uint16_t count; // count is put in here bc every state needs a counter, uint16_t is an unsigned 16 bit int
// } led_state_object_t; // a structure that'll look at state and count


// static led_state_object_t led_state_object;

// void state_machine_init() {
//     led_state_object.count = 0;
//     smf_set_initial(SMF_CTX(&led_state_object), &led_states[LED_ON_STATE]);
// }

// int state_machine_run() {
//     return smf_run_state(SMF_CTX(&led_state_object));
// }

// static void led_off_state_exit(void* o) {
//     LED_set(LED0, LED_ON);
// }

// static enum smf_state_result led_on_state_run(void* o){
//     if (led_state_object.count > 500) {
//         led_state_object.count = 0;
//         smf_set_state(SMF_CTX(&led_state_object), &led_states[LED_OFF_STATE]);
//     } else {
//         led_state_object.count++;
//     }

//     return SMF_EVENT_HANDLED;
// }

// static void led_on_state_exit(void* o) {
//     LED_set(LED0, LED_OFF);
// }

// static enum smf_state_result led_off_state_run(void* o) {
//     if(led_state_object.count > 500) {
//         led_state_object.count = 0;
//         smf_set_state(SMF_CTX(&led_state_object), &led_states[LED_ON_STATE]);
//     } else {
//         led_state_object.count++;
//     }

//     return SMF_EVENT_HANDLED;
// }