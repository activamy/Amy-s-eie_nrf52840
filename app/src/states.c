/*
 * states.c
 */


#include <zephyr/smf.h>

#include "LED.h"
#include "BTN.h"
#include "states.h"

#define BINARY_LENGTH 8

/*---------------------------------------------------------
 * Function Prototypes
 *-------------------------------------------------------*/

 // States
static void clean_state_entry(void* o);
static enum smf_state_result clean_state_run(void* o);
static void clean_state_exit(void* o);

static void input_state_entry(void* o);
static enum smf_state_result input_state_run(void* o);
static void input_state_exit(void* o);

static void ascii_save_entry(void* o);
static enum smf_state_result ascii_save_run(void* o);
static void ascii_save_exit(void* o);

static void string_save_entry(void* o);
static enum smf_state_result string_save_run(void* o);
static void string_save_exit(void* o);

static void display_state_entry(void* o);
static enum smf_state_result display_state_run(void* o);
static void display_state_exit(void* o);

 // Functions

/*---------------------------------------------------------
 * Typedefs
 *-------------------------------------------------------*/
enum state_diagram_machine_states {
    CLEAR_LIST_BLINK_1HZ,
    INPUT_LIST_BLINK_1HZ,
    ASCII_SAVE_BLINK_4HZ,
    STRING_SAVE_BLINK_16HZ,
    DISPLAY_STRING_BLINK_OFF
};

typedef struct {
    struct smf_ctx ctx;

    uint8_t count, current_duty_cycle;
    int binary[BINARY_LENGTH], decimal, button_count;
    char ascii_string[256];
    bool list_clear;

} state_objects_t;


/*---------------------------------------------------------
 * Local Variables
 *-------------------------------------------------------*/
static const struct smf_state states[] = {
    [CLEAR_LIST_1HZ] = SMF_CREATE_STATE(clean_state_entry, clean_state_run, clear_state_exit, NULL, NULL),
    [INPUT_LIST_1HZ] = SMF_CREATE_STATE(input_state_entry, input_state_run, input_state_exit, NULL, NULL),
    [ASCII_SAVE_4HZ] = SMF_CREATE_STATE(ascii_save_entry, ascii_save_run, ascii_save_exit, NULL, NULL),
    [STRING_SAVE_16HZ] = SMF_CREATE_STATE(string_save_entry, string_save_run, string_save_exit, NULL, NULL),
    [DISPLAY_STRING_OFF] = SMF_CREATE_STATE(display_state_entry, display_state_run, display_state_exit, NULL, NULL),
};

static state_objects_t state_objects;


/*---------------------------------------------------------
 * State Definitions
 *-------------------------------------------------------*/
void state_machine_init() {
    state_objects.count = 0;
    state_objects.current_duty_cycle = 0;
    state_objects.decimal = 0;
    state_objetcs.list_clear = FALSE;

    smf_set_initial(SMF_CTX(&state_objects), &states[CLEAR_LIST_1HZ]);
}

int state_machine_run() {
    return smf_run_states(SMF_CTX(&state_objects));
}

// Clean State
static void clean_state_entry(void* o) {
    state_objects.count = 0;
    LED_blink(LED3, LED_1HZ);
    printk("Entering Clean State \n");
}

static enum smf_state_result clean_state_run(void* o) {
    if (state_objects.clear_list == FALSE) {
        ascii_string[0] = '\0';
        for (int i = 0; i < BINARY_LENGTH; i++) {
            a[i] = 0;
        }
    }
    smf_set_state(SMF_CTX(&states_objects), &states[INPUT_LIST_1HZ]);
}

static void clean_state_exit(void* o) {
    printk("Exiting Clean State \n");
}


// Input State
static void input_state_entry(void* o) {
    LED_blink(LED3, LED_1HZ);
    printk("Entering Input State \n");
    state_objects.button_count = 0;
}

static enum smf_state_result input_state_run(void* o) {
    if (BTN_check_clear_pressed(BTN0)) {
        state_objects.binary[state_objects.button_count] = 0;
        state_objects.button_count ++;
    }

    if (BTN_check_clear_pressed(BTN1)) {
        state_objects.binary[state_objects.button_count] = 1;
        state_objects.button_count ++;
    }

    if (BTN_check_clear_pressed(BTN2)) {
        smf_set_state(SMX_CTX(&states_objects), &states[CLEAR_LIST_1HZ]);
    }

    if (BTN_check_clear_pressed(BTN3)) {
        smf_set_state(SMX_CTX(&states_objects), &states[ASCII_SAVE_4HZ]);
    }

    if (count > BINARY_MAX_LENGTH) {
        smf_set_state(SMX_CTX(&states_objects), &states[CLEAR_LIST_1HZ]);
    }
}

static void input_state_exit(void* o) {
    printk("Exiting Input State \n");
}


// ASCII Save State
static void ascii_save_entry(void* o) {
    printk("Entering ASCII Save Mode");
}

static enum smf_state_result ascii_save_run(void* o);
static void ascii_save_exit(void* o) {
    printk("Exiting ASCII Save Mode");
}

// String Save State
static void string_save_entry(void* o);
static enum smf_state_result string_save_run(void* o);
static void string_save_exit(void* o);

static void display_state_entry(void* o);
static enum smf_state_result display_state_run(void* o);
static void display_state_exit(void* o);

/*---------------------------------------------------------
 * Function Definitions
 *-------------------------------------------------------*/