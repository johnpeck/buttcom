/* bc_command.c */

// ----------------------- Include files ------------------------------
#include <stdio.h>
#include <string.h>

/* pgmspace.h
 * Provides macros and functions for saving and reading data out of
 * flash.
 */
#include <avr/pgmspace.h>


/* bc_command.h 
 * Provides the extern declaration of command_array --
 * an array containing all the commands understood by the system. 
 */
#include "bc_command.h"

/* bc_functions.h 
 * Provides some sample functions for the command
 * handler to call.
 */
#include "bc_functions.h"

/* bc_usart.h provides functions for transmitting characters over the
 * usart.
 */
#include "bc_usart.h"

/* bc_ascii.h
 * Provides lowstring() for converting strings to lower case.
 */
#include "bc_ascii.h"

/* bc_logger.h
 * Provides logger_msg and logger_msg_p for log messages tagged with
 * a system and severity.
 */
#include "bc_logger.h"

/* bc_numbers.h
 * Provides ascii to number conversion.
 */
#include "bc_numbers.h"





/* An array of command_structs will contain our remote commands */
command_t command_array[] ={
    // The junk function
    {"junk",   // Name
    "hex",     // Argument type
    2,         // Maximum number of characters in argument
    &junkfunc, // Address of function to execute
    "Some junk"}, // Help string
    // The crap function
    {"crap",
    "none",
    0,
    &crapfunc,
    "Some crap"},
    {"logreg",
     "hex",
     4,
     &cmd_logger_setreg,
     "Some help"},
    // End of table indicator.  Must be last.
    {"","",0,0,""}
};




/* check_argsize( pointer to received command state,
 *                pointer to list of commands )
 * Returns 0 if the argument size is less than or equal to the number
 * of characters specified in the command list.  Returns -1 otherwise. 
 */
uint8_t check_argsize(recv_cmd_state_t *recv_cmd_state_ptr ,
                      struct command_struct *command_array) {
    uint8_t isok = 0;
    uint8_t argsize = strlen(recv_cmd_state_ptr -> pbuffer_arg_ptr);
    logger_msg_p("command",log_level_INFO,
        PSTR("Argument size is %d.\r\n"), argsize);
    if (argsize > (command_array -> arg_max_chars)) {
        isok = -1;
    }
    return isok;
}

void rbuffer_erase( recv_cmd_state_t *recv_cmd_state_ptr ) {
    memset((recv_cmd_state_ptr -> rbuffer),0,RECEIVE_BUFFER_SIZE);
    recv_cmd_state_ptr -> rbuffer_write_ptr =
        recv_cmd_state_ptr -> rbuffer; // Initialize write pointer
    recv_cmd_state_ptr -> rbuffer_count = 0;
    return;
}

void process_pbuffer( recv_cmd_state_t *recv_cmd_state_ptr ,
                    struct command_struct *command_array) {
    if ((recv_cmd_state_ptr -> pbuffer_lock) == 1) {
        // Parse buffer is locked -- there's a command to process
        logger_msg_p("command",log_level_INFO,
            PSTR("The parse buffer is locked.\r\n"));
        recv_cmd_state_ptr -> pbuffer_arg_ptr = strchr(recv_cmd_state_ptr -> pbuffer,' ');
        if (recv_cmd_state_ptr -> pbuffer_arg_ptr != NULL) {
            // Parse buffer contains a space -- there's an argument
            logger_msg_p("command",log_level_INFO,
                PSTR("The command contains a space.\r\n"));
            *(recv_cmd_state_ptr -> pbuffer_arg_ptr) = '\0'; // Terminate the command string
            (recv_cmd_state_ptr -> pbuffer_arg_ptr)++;
            while (*(recv_cmd_state_ptr -> pbuffer_arg_ptr) == ' ') {
                (recv_cmd_state_ptr -> pbuffer_arg_ptr)++; // Move to first non-space character
            }
            // arg_ptr now points to the beginning of the parameter
            logger_msg_p("command",log_level_INFO,
                PSTR("The command's argument is '%s'.\r\n"),
                (recv_cmd_state_ptr -> pbuffer_arg_ptr));
            // usart_printf_p(PSTR("The parameter is %s\r\n"),(recv_cmd_state_ptr -> pbuffer_arg_ptr));
        }
        lowstring(recv_cmd_state_ptr -> pbuffer); // Convert command to lower case
        // Look through the command list for a match
        uint8_t pbuffer_match = 0;
        while ((command_array -> execute) != 0) {
            if (strcmp( recv_cmd_state_ptr -> pbuffer,
                command_array -> name ) == 0) {
                // We've found a matching command
                logger_msg_p("command",log_level_INFO,
                    PSTR("Command '%s' recognized.\r\n"),command_array -> name);
                pbuffer_match = 1;
                if (strcmp( command_array -> arg_type, "none") != 0) {
                    // The command is specified to have an argument
                    uint8_t arg_ok = check_argsize(recv_cmd_state_ptr,command_array);
                    if (arg_ok != 0) {
                        logger_msg_p("command",log_level_ERROR,
                            PSTR("Argument to '%s' is out of range.\r\n"),
                            command_array -> name);
                        }
                    else {
                        // The argument is the right size
                        logger_msg_p("command",log_level_INFO,
                            PSTR("Argument to '%s' is within limits.\r\n"),
                            command_array -> name);
                        command_exec(command_array,recv_cmd_state_ptr -> pbuffer_arg_ptr);
                    }
                }
                else  {
                    // There's no argument specified
                    if (recv_cmd_state_ptr -> pbuffer_arg_ptr != NULL) {
                        // There's an argument, but we didn't expect one
                        logger_msg_p("command",log_level_WARNING,
                            PSTR("Ignoring argument for command '%s'.\r\n"),
                            command_array -> name);
                    }
                    command_exec(command_array,NULL);
                }
                recv_cmd_state_ptr -> pbuffer_lock = 0;
                break;
            }
            command_array++;
        }
        // If we didn't find a match, send an error message
        if (pbuffer_match == 0) {
            logger_msg_p("command",log_level_ERROR,
                PSTR("Unrecognized command: '%s'.\r\n"),recv_cmd_state_ptr -> pbuffer);
            recv_cmd_state_ptr -> pbuffer_lock = 0;
        }
    }
    return;
}

/* Making this function explicitly take a pointer to the received command
 * state structure makes it clear that it modifies this structure.  This
 * function will ultimately also have to set up the USART hardware. */
void command_init( recv_cmd_state_t *recv_cmd_state_ptr ) {
    memset((recv_cmd_state_ptr -> rbuffer),0,RECEIVE_BUFFER_SIZE);
    recv_cmd_state_ptr -> rbuffer_write_ptr =
        recv_cmd_state_ptr -> rbuffer; // Initialize write pointer
    memset((recv_cmd_state_ptr -> pbuffer),0,PARSE_BUFFER_SIZE);
    recv_cmd_state_ptr -> pbuffer_arg_ptr =
        recv_cmd_state_ptr -> pbuffer; // Initialize argument pointer
    recv_cmd_state_ptr -> rbuffer_count = 0;
    recv_cmd_state_ptr -> pbuffer_lock = 0; // Parse buffer unlocked
    return;
}

/* Execute a valid command received over the remote interface.
 */
void command_exec( command_t *command, char *argument ) {
    uint16_t argval = 0; // Decimal value of the argument
    if (strcmp( command -> arg_type,"none" ) == 0) {
        // There's no argument
        logger_msg_p("command",log_level_INFO,
            PSTR("Executing command with no argument\r\n"));
        command -> execute(0);
    }
    else if (strcmp( command -> arg_type,"hex" ) == 0) {
        // There's a hex argument
        logger_msg_p("command",log_level_INFO,
            PSTR("Executing command with hex argument\r\n"));
        argval = hex2num(argument);
        logger_msg_p("command",log_level_INFO,
            PSTR("The argument value is %d.\r\n"),argval);
        command -> execute(argval);
    }
}

