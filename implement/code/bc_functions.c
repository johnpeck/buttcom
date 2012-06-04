#include <stdio.h>
#include <string.h>

/* pgmspace.h
 * Contains macros and functions for saving and reading data out of
 * flash.
 */
#include <avr/pgmspace.h>
#include "bc_usart.h"


#include "bc_functions.h"
/* pgmspace.h
 * Contains macros and functions for saving and reading data out of
 * flash.
 */
#include <avr/pgmspace.h>

/* bc_logger.h sets up logging */
#include "bc_logger.h"


void cmd_hello( uint16_t setval ) {
    usart_printf_p(PSTR("Hello yourself!\r\n"));
    return;
}

void cmd_help( uint16_t setval ) {
    print_help( command_array );
    return;
}

void print_help( command_t *command_array ) {
    while ((command_array -> execute) != 0) {
        usart_printf_p(command_array -> help);
        command_array++;
    }
    return;
}
