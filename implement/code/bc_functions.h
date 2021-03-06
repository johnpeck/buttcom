/* bc_functions.h
 * 
 * Sample functions for the buttcom project */

/* bc_command.h 
 * Contains the definition of command_t -- the variable type containing
 * the attributes of each remote command.
 */
#include "bc_command.h"

/* Calibration structure */
struct cal_struct {
    uint8_t slope; // Measurements are multiplied by this factor before offset
    uint8_t offset; // Measurements are offset by this factor after slope is applied
};

/* cmd_hello()
 * Print a greeting.
 */
void cmd_hello( uint16_t nonval );

/* cmd_help()
 * Call print_help() to print the help strings for all recognized
 * commands.
 */
void cmd_help( uint16_t nonval );

/* print_help()
 * Called by cmd_help().  Prints the help strings for all recognized
 * commands.
 */
void print_help(command_t *command_array);
