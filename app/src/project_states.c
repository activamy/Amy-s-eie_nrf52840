/*
 * project_states.c
 *
 */

#include <zephyr/smf.h>

#include "project_states.h"
#include "BTN.h"
#include "LED.h"


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

static void end_entry(void* o);
static enum smf_state_result end_run(void* o);
static void end_exit(void* o);

static void delete_entry(void* o);
static enum smf_state_result delete_run(void* o);
static void delete_exit(void* o);

static void final_entry(void* o);
static enum smf_state_result final_run(void* o);
static void final_exit(void* o);


 // Functions


/*----------------------------------------------------------------
 * Typedefs
 *--------------------------------------------------------------*/

enum states_def {
    RND_1,
    STITCH_SELECTOR,
    AMOUNT,
    RND_REVIEW,
    END_RND,
    DELETE_OPTION,
    FINISH_PROJECT
};

typedef struct {
  struct smf_ctx ctx;

  int round_number;
  
  char project_rnd[20]; // set limit for how many rounds now, edit later
  char round_stitches[20];

} project_states_objects_t;


/*----------------------------------------------------------------
 * Local Variables
 *--------------------------------------------------------------*/

static const struct smf_state states[] = {
    [RND_1] = SMF_CREATE_STATE(start_entry, start_run, start_exit, NULL, NULL),
    [STITCH_SELECTOR] = SMF_CREATE_STATE(selection_entry, selection_run, selection_exit, NULL, NULL),
    [AMOUNT] = SMF_CREATE_STATE(amount_entry, amount_run, amount_exit, NULL, NULL),
    [RND_REVIEW] = SMF_CREATE_STATE(review_entry, review_run, review_exit, NULL, NULL),
    [END_RND] = SMF_CREATE_STATE(end_entry, end_run, end_exit, NULL, NULL),
    [DELETE_OPTION] = SMF_CREATE_STATE(delete_entry, delete_run, delete_exit, NULL, NULL),
    [FINISH_PROJECT] = SMF_CREATE_STATE(final_entry, final_run, final_exit, NULL, NULL)
};

static project_states_objects_t state_objects;


/*----------------------------------------------------------------
 * State Definitions
 *--------------------------------------------------------------*/

 void project_states_init() {
    state_objects.round_number = 1;

    smf_set_initial(SMF_CTX(&state_objects), &states[RND_1]);
 }