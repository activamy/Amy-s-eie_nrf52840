/*
 * states.c
 */


#include <zephyr/smf.h>

#include "LED.h"
#include "BTN.h"
#include "states.h"

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
    int binary[8], decimal;
    char acsii_string[];

} state_objects_t;


/*---------------------------------------------------------
 * Local Variables
 *-------------------------------------------------------*/
static const struct smf_state states[] = {
    [CLEAR_LIST_BLINK_1HZ] = SMF_CREATE_STATE(clean_state_entry, clean_state_run, clear_state_exit, NULL, NULL),
    [INPUT_LIST_BLINK_1HZ] = SMF_CREATE_STATE(input_state_entry, input_state_run, input_state_exit, NULL, NULL),
    [ASCII_SAVE_BLINK_4HZ] = SMF_CREATE_STATE(ascii_save_entry, ascii_save_run, ascii_save_exit, NULL, NULL),
    [STRING_SAVE_BLINK_16HZ] = SMF_CREATE_STATE(string_save_entry, string_save_run, string_save_exit, NULL, NULL),
    [DISPLAY_STRING_BLINK_OFF] = SMF_CREATE_STATE(display_state_entry, display_state_run, display_state_exit, NULL, NULL),
};

static state_objects_t state_objects;


/*---------------------------------------------------------
 * State Definitions
 *-------------------------------------------------------*/
void state_machine_init() {
    state_objects.count = 0;
    state_objects.current_duty_cycle = 0;
    smf_set_initial(SMF_CTX(&state_objects), &states[CLEAR_LIST_BLINK_1HZ]);
}

int state_machine_run() {
    return smf_run_states(SMF_CTX(&state_objects));
}

// Clean State
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

/*---------------------------------------------------------
 * FUnction Definitions
 *-------------------------------------------------------*/