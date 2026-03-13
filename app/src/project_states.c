/*
 * project_states.c
 *
 */

#include <zephyr/smf.h>

#include "project_states.h"
#include "BTN.h"
#include "LED.h"

#include <math.h>
#include <string.h>

/*----------------------------------------------------------------
 * Function Prototypes
 *--------------------------------------------------------------*/

 // State
static void start_entry(void* o);
static enum smf_state_result start_run(void* o);
static void start_exit(void* o);

static void selection_entry(void* o);
static enum smf_state_result selection_run(void* o);
static void selection_exit(void* o);

static void amount_entry(void* o);
static enum smf_state_result amount_run(void* o);
static void amount_exit(void* o);

static void review_entry(void* o);
static enum smf_state_result review_run(void* o);
static void review_exit(void* o);

// static void end_entry(void* o);
// static enum smf_state_result end_run(void* o);
// static void end_exit(void* o);

static void restart_entry(void* o);
static enum smf_state_result restart_run(void* o);
static void restart_exit(void* o);

static void final_entry(void* o);
static enum smf_state_result final_run(void* o);
static void final_exit(void* o);


 // Functions
int binaryToDecimal(int binary[], int length);
/*Converts binary to decimal.
   Require
     A binary value the user has inputted and the length of the binary 
     number.
   Promises: The decimal equivalent of the binary number.
   */

void led_setting(int led_num, char *state);
/*
    Require
      blah blah blah
    Promises: blah blah blah
*/

void turn_off_all_leds();
void turn_on_all_leds();



/*----------------------------------------------------------------
 * Typedefs
 *--------------------------------------------------------------*/

enum states_def {
    RND_1,
    STITCH_SELECTOR,
    AMOUNT,
    RND_REVIEW,
    END_RND,
    RESTART_OPTIONS,
    FINISH_PROJECT
};

enum stitches { // Keep adding on as needed
    NO_STITCHES,
    MAGIC_RING,
    SLIP_KNOT,

    CHAIN, 
    SINGLE_CROCHET, 
    DOUBLE_CROCHET, 
    HALF_DOUBLE_CROCHET,
    /*SLIP_STITCH*/
};

static const char *stitch_names[] = {
    "no",
    "MR",
    "Slip knot",

    "CH",
    "SC",
    "DC",
    "HDC",
    /*"SL ST"*/
};


typedef struct {
    enum stitches stitch;
    int amount;
} stitch_t;
typedef struct {
  struct smf_ctx ctx;

  int round_number; // starts at zero, but = rnd 1
  int stitch_number;
  int places, count, first_appearance;
  int binary_amount[4]; // binary value from 0 to 9
  int decimal_conversion, decimal_amount;
  
  stitch_t project_rnd[20][20]; // set limit for how many rounds now and how many can go in each list, edit later
} project_states_objects_t;



/*----------------------------------------------------------------
 * Local Variables
 *--------------------------------------------------------------*/

static const struct smf_state states[] = {
    [RND_1] = SMF_CREATE_STATE(start_entry, start_run, start_exit, NULL, NULL),
    [STITCH_SELECTOR] = SMF_CREATE_STATE(selection_entry, selection_run, selection_exit, NULL, NULL),
    [AMOUNT] = SMF_CREATE_STATE(amount_entry, amount_run, amount_exit, NULL, NULL),
    [RND_REVIEW] = SMF_CREATE_STATE(review_entry, review_run, review_exit, NULL, NULL),
    // [END_RND] = SMF_CREATE_STATE(end_entry, end_run, end_exit, NULL, NULL),
    [RESTART_OPTIONS] = SMF_CREATE_STATE(restart_entry, restart_run, restart_exit, NULL, NULL),
    [FINISH_PROJECT] = SMF_CREATE_STATE(final_entry, final_run, final_exit, NULL, NULL)
};

static project_states_objects_t state_objects;

/*----------------------------------------------------------------
 * State Definitions
 *--------------------------------------------------------------*/

 void project_states_init() {
    state_objects.round_number = 0;
    memset(state_objects.project_rnd, 0, sizeof(state_objects.project_rnd));
    memset(state_objects.binary_amount, 0, sizeof(state_objects.binary_amount));
    
    smf_set_initial(SMF_CTX(&state_objects), &states[RND_1]);
 }

 int project_states_run() {
    return smf_run_state(SMF_CTX(&state_objects));
 }

//-------------------------------------------MAYBE ADD SOME OTHER INITIALIZER-------
 // Round 1 Initialization
 static void start_entry(void* o) {
    memset(state_objects.project_rnd, 0, sizeof(state_objects.project_rnd));
    memset(state_objects.binary_amount, 0, sizeof(state_objects.binary_amount));
    state_objects.round_number = 0;
    state_objects.stitch_number = 0;
    printk("--------------------\nProject Begin: \n\nRound 1: ");
    LED_blink(LED0, LED_4HZ);
}

 static enum smf_state_result start_run(void* o) {
    if (BTN_check_clear_pressed(BTN0)) {
        state_objects.project_rnd[state_objects.round_number][state_objects.stitch_number].stitch = MAGIC_RING;
        printk("%s, ", stitch_names[MAGIC_RING]);
        smf_set_state(SMF_CTX(&state_objects), &states[STITCH_SELECTOR]);
    }

    if (BTN_check_clear_pressed(BTN1)) {
        state_objects.project_rnd[state_objects.round_number][state_objects.stitch_number].stitch = SLIP_KNOT;
        printk("%s, ", stitch_names[SLIP_KNOT]);
        smf_set_state(SMF_CTX(&state_objects), &states[STITCH_SELECTOR]);
    }

    return SMF_EVENT_HANDLED;
 }

 static void start_exit(void* o) {
    state_objects.stitch_number++;
    turn_off_all_leds();
 }


 // Sticth Selection
 static void selection_entry(void* o) {
    LED_blink(LED1, LED_4HZ);
 } 

 static enum smf_state_result selection_run(void* o) {
    if (BTN_check_clear_pressed(BTN0)) {
        state_objects.project_rnd[state_objects.round_number][state_objects.stitch_number].stitch = CHAIN;
        printk("%s ", stitch_names[CHAIN]);
        smf_set_state(SMF_CTX(&state_objects), &states[AMOUNT]);
    }

    if (BTN_check_clear_pressed(BTN1)) {
        state_objects.project_rnd[state_objects.round_number][state_objects.stitch_number].stitch = SINGLE_CROCHET;
        printk("%s ", stitch_names[SINGLE_CROCHET]);
        smf_set_state(SMF_CTX(&state_objects), &states[AMOUNT]);
    }

    if (BTN_check_clear_pressed(BTN2)) {
        state_objects.project_rnd[state_objects.round_number][state_objects.stitch_number].stitch = DOUBLE_CROCHET;
        printk("%s ", stitch_names[DOUBLE_CROCHET]);
        smf_set_state(SMF_CTX(&state_objects), &states[AMOUNT]);
    }

    if (BTN_check_clear_pressed(BTN3)) {
        state_objects.project_rnd[state_objects.round_number][state_objects.stitch_number].stitch = HALF_DOUBLE_CROCHET;
        printk("%s ", stitch_names[HALF_DOUBLE_CROCHET]);
        smf_set_state(SMF_CTX(&state_objects), &states[AMOUNT]);
    }

    return SMF_EVENT_HANDLED;
}

 static void selection_exit(void* o) {
    turn_off_all_leds();
 }


 // Amount of Stitches -------- Only use 0s and 1s
 static void amount_entry(void* o) {
    state_objects.places = 2;
    state_objects.count = 0;
    state_objects.decimal_conversion = 0;
    state_objects.decimal_amount = 0;
    turn_off_all_leds(); // Turn off all led

    memset(state_objects.binary_amount, 0, sizeof(state_objects.binary_amount));
 }

 static enum smf_state_result amount_run(void* o) {
    led_setting(state_objects.places, "ON"); // Turn / keep on Led for specific place values

    // uses idea of bcd to determine ones, tens, and hundreds place amount
    // For ones place:
    if (state_objects.places == 0) {
        if (BTN_check_clear_pressed(BTN0)) {
            state_objects.binary_amount[state_objects.count] = 0;
            state_objects.count++;
        }
        if (BTN_check_clear_pressed(BTN1)) {
            state_objects.binary_amount[state_objects.count] = 1;
            state_objects.count++;
        }
    }
    // For tens place:
     if (state_objects.places == 1) {
        if (BTN_check_clear_pressed(BTN0)) {
            state_objects.binary_amount[state_objects.count] = 0;
            state_objects.count++;
        }
        if (BTN_check_clear_pressed(BTN1)) {
            state_objects.binary_amount[state_objects.count] = 1;
            state_objects.count++;
        }
    }
    // For hundreds place:
     if (state_objects.places == 2) {
        if (BTN_check_clear_pressed(BTN0)) {
            state_objects.binary_amount[state_objects.count] = 0;
            state_objects.count++;
        }
        if (BTN_check_clear_pressed(BTN1)) {
            state_objects.binary_amount[state_objects.count] = 1;
            state_objects.count++;
        }
    }

    // Convert binary to Decimal
    // Max stitches = 999
    if (BTN_check_clear_pressed(BTN2) || (state_objects.count == 4)) {
        state_objects.decimal_conversion = binaryToDecimal(state_objects.binary_amount, state_objects.count);
        
        if (state_objects.places == 0) {
            state_objects.decimal_amount += state_objects.decimal_conversion;
        }
        if (state_objects.places == 1) {
            state_objects.decimal_amount += state_objects.decimal_conversion * 10;
        }
        if (state_objects.places == 2) {
            state_objects.decimal_amount += state_objects.decimal_conversion * 100;
        }

        state_objects.places--;
        state_objects.count = 0;
    }

    if (state_objects.places == -1) {
        printk("%d ", state_objects.decimal_amount);
        state_objects.project_rnd[state_objects.round_number][state_objects.stitch_number].amount = state_objects.decimal_amount;

        smf_set_state(SMF_CTX(&state_objects), &states[RND_REVIEW]);
    }

    return SMF_EVENT_HANDLED;
 }

 static void amount_exit(void* o) {
    turn_off_all_leds();
 }
   

 // Rnd Review
 static void review_entry(void* o) {
    state_objects.first_appearance = 0;
    LED_blink(LED2, LED_4HZ);
 }
 
 static enum smf_state_result review_run(void* o) {
    if (state_objects.first_appearance == 0) {
        printk("\n\nThe result for Round %d: ", state_objects.round_number + 1);
        state_objects.first_appearance++;

        for (int i = 0; i < 20; i++) { 
            if (strcmp(stitch_names[state_objects.project_rnd[state_objects.round_number][i].stitch], "no") == 0) {
                break;
            }

            printk("%s %d, ", stitch_names[state_objects.project_rnd[state_objects.round_number][i].stitch], state_objects.project_rnd[state_objects.round_number][i].amount);
        }

        printk("\nSelect:\n\t1) Continue round %d\n\t2) Move to round %d\n\t3) Finish project\n\t4) Move to restart options\n\n", state_objects.round_number + 1, state_objects.round_number + 2);
            //"Select:\n
            // \t1) Continue to round %d\n
            // \t2) Move to round %d\n
            // \t3) Finish project\n 
            // \t4)Move to restart options    }
    }

    if (BTN_check_clear_pressed(BTN0)) {   // Continue round
        state_objects.stitch_number++;
        printk("Round %d: ", state_objects.round_number + 1);

        for (int i = 0; i < state_objects.stitch_number; i++) {
            printk("%s %d, ", stitch_names[state_objects.project_rnd[state_objects.round_number][i].stitch], state_objects.project_rnd[state_objects.round_number][i].amount);
        }
        smf_set_state(SMF_CTX(&state_objects), &states[STITCH_SELECTOR]);
    }

    if (BTN_check_clear_pressed(BTN1)) {   // Move to next round
        state_objects.round_number++;
        state_objects.stitch_number = 0;
        printk("Round %d: ", state_objects.round_number + 1);
        smf_set_state(SMF_CTX(&state_objects), &states[STITCH_SELECTOR]);
    }

    if (BTN_check_clear_pressed(BTN2)) {   // End the project
        smf_set_state(SMF_CTX(&state_objects), &states[FINISH_PROJECT]);
    }

    if (BTN_check_clear_pressed(BTN3)) {   // Move to Restart options
        smf_set_state(SMF_CTX(&state_objects), &states[RESTART_OPTIONS]);
    }

    return SMF_EVENT_HANDLED;
}

 static void review_exit(void* o) {
    turn_off_all_leds();
 }
  


 // Restart Option
 static void restart_entry(void* o) {
    state_objects.first_appearance = 0;
    LED_blink(LED3, LED_4HZ);
    printk("You have chosen restart options:\n");
 }

 static enum smf_state_result restart_run(void* o) {
    if (state_objects.first_appearance == 0) {
        printk("\t1) Restart round %d\n\t2) Restart project\n\t3) Cancel\n", state_objects.round_number + 1);

        state_objects.first_appearance++;
    }
    // \t1) Restart round\n
    // \t2) Restart project
    // \t3) Cancel

    if (BTN_check_clear_pressed(BTN0)) {
        memset(state_objects.project_rnd[state_objects.round_number], 0, sizeof(state_objects.project_rnd[state_objects.round_number]));
        state_objects.stitch_number = 0;
        printk("\n----------\nRound %d: ", state_objects.round_number + 1);

        smf_set_state(SMF_CTX(&state_objects), &states[STITCH_SELECTOR]);
    }

    if (BTN_check_clear_pressed(BTN1)) {
        smf_set_state(SMF_CTX(&state_objects), &states[RND_1]);   //RND_1 already clears
    }

    return SMF_EVENT_HANDLED;

 }

 static void restart_exit(void* o) {
    turn_off_all_leds();
 }
 

 // Complete the Project
 static void final_entry(void* o) {
    state_objects.first_appearance = 0;
    turn_on_all_leds();
 }

 static enum smf_state_result final_run(void* o) {
    if (state_objects.first_appearance == 0) {
        int k;
        printk("\n-----------------------\nPrinting Project... \n\n");
        
        for (int j = 0; j <= state_objects.round_number; j++) {
            printk("Round %d: ", j + 1);
            for (k = 0; k < 19; k++) {
                if (state_objects.project_rnd[j][k+1].stitch == NO_STITCHES) {
                    break;
                }
                printk("%s %d, ", stitch_names[state_objects.project_rnd[j][k].stitch], state_objects.project_rnd[j][k].amount);
            }

            if (state_objects.project_rnd[j][k].stitch != NO_STITCHES) {
                printk("%s %d\n", stitch_names[state_objects.project_rnd[j][k].stitch], state_objects.project_rnd[j][k].amount);
            }
        }

        printk("\n----------End----------\n\n");
        printk("Would you like to make another?\n\t1) Yes -> BTN0\n\n");
        state_objects.first_appearance++;
    }

    if (BTN_check_clear_pressed(BTN0)) {
        smf_set_state(SMF_CTX(&state_objects), &states[RND_1]);
    }

    return SMF_EVENT_HANDLED;
 }

 static void final_exit(void* o) {
    turn_off_all_leds();
 }









/*-------------------------------------------------
 * Functions 
 *-----------------------------------------------*/
int binaryToDecimal(int binary[], int length) {
    int decimal = 0;

    for (int i = 0; i < length; i++) {
        decimal += (binary[length - 1 - i] << i); // binary shifting, better version of 2^i
    }

    return decimal;
}


// add more led functionalities here
void led_setting(int led_num, char *state) {

    //Turn each on individually
    if (led_num == 0 && strcmp(state, "ON") == 0) {
        LED_set(LED0, LED_ON);
    }

    if (led_num == 1 && strcmp(state, "ON") == 0) {
        LED_set(LED1, LED_ON);
    }

    if (led_num == 2 && strcmp(state, "ON") == 0) {
        LED_set(LED2, LED_ON);
    }

    if (led_num == 3) {
        LED_set(LED3, LED_ON);
    }

    // Turn all off
    if (led_num == 4) {
        turn_off_all_leds();
    }
}


void turn_off_all_leds() {
    LED_set(LED0, LED_OFF);
    LED_set(LED1, LED_OFF);
    LED_set(LED2, LED_OFF);
    LED_set(LED3, LED_OFF);
}

void turn_on_all_leds() {
    LED_set(LED0, LED_ON);
    LED_set(LED1, LED_ON);
    LED_set(LED2, LED_ON);
    LED_set(LED3, LED_ON);
}