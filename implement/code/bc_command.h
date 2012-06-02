/* bc_command.h
 * 
 * Used to set up the parkdar command interface. 
 */



// Define the size of the received character buffer
#define RECEIVE_BUFFER_SIZE 20
// Define the size of the parse buffer
#define PARSE_BUFFER_SIZE 20

/* Received command state structure.  This is a collection of global 
 * variables needed to set up and keep track of the remote command 
 * interface.  A structure of this type must be instantiated in bc_main.c
 * so that it can work with the received character ISR. */
typedef struct recv_cmd_struct { 
    char rbuffer[RECEIVE_BUFFER_SIZE]; // Received character buffer
    /* rbuffer_write_ptr will always point to the next write location
     * in the received character buffer. */
    char *rbuffer_write_ptr;
    /* Properly terminated strings will wait in the parse buffer to be
     * processed.  These strings may or may not have one argument, separated
     * from the command by 1 or a few spaces (the entire string isn't
     * allowed to exceed the buffer size).  */
    char pbuffer[PARSE_BUFFER_SIZE];
    char *pbuffer_arg_ptr; // Points to the beginning of the argument
    uint8_t rbuffer_count; // Counts up as characters go into receive buffer.
    uint8_t pbuffer_lock; // Parse buffer lock.  1 = locked
} recv_cmd_state_t;


/* Define fpointer_t to have the type "pointer to function," a return
 * value of void, and a parameter type of void. */
 
/* In order to make all functions called by remote commands take the same
 * arguments, we settle on them all taking a 16-bit argument.
 */
typedef void (*fpointer_t)(uint16_t argval);
/* Each command_struct will describe one command */
typedef struct command_struct {
    char *name; // The name of the command
    char *arg_type; // A string representing the type of argument expected
    uint8_t arg_max_chars; // The maximum number of characters in the argument
    fpointer_t execute; // The function to execute
    char *help;
} command_t;

/* The array of command structures will have global scope.  The variable
 * command_array should be initialized in bc_command.c 
 */
extern command_t command_array[];


/* Initialize the received command state: Erase the buffers, reset
 * the write and argument pointers, zero the received character
 * counter, and unlock the parse buffer. */
void command_init( recv_cmd_state_t *recv_cmd_state_ptr );

/* Execute a valid command received over the remote interface.
 */
void command_exec( command_t *command, char *argument );





/* check_argsize( pointer to received command state,
 *                pointer to list of commands )
 * Returns 0 if the argument size is less than or equal to the number
 * of characters specified in the command list.  Returns -1 otherwise. */
uint8_t check_argsize(recv_cmd_state_t *recv_cmd_state_ptr ,
                  struct command_struct *command_array);


/* process_pbuffer( recv_cmd_state_t *recv_cmd_state_ptr,
 *                  command_struct *commands )
 * Process the command (if there is one) in the parse buffer. */
void process_pbuffer( recv_cmd_state_t *recv_cmd_state_ptr ,
                    struct command_struct *command_array);
                    
/* Erases the received character buffer, resets the received character
 * number, and resets the write pointer. */
void rbuffer_erase( recv_cmd_state_t *recv_cmd_state_ptr );