#include <stdio.h>
#include <string.h>

#include "bc_functions.h"
/* pgmspace.h
 * Contains macros and functions for saving and reading data out of
 * flash.
 */
#include <avr/pgmspace.h>

/* bc_logger.h sets up logging */
#include "bc_logger.h"

void junkfunc(uint16_t junkval) {
    logger_msg_p("ranger",log_level_INFO,PSTR("This is all junk!\r\n"));
    return;
}

void crapfunc(uint16_t crapval) {
    logger_msg_p("ranger",log_level_INFO,PSTR("This is all crap!\r\n"));
    return;
}
