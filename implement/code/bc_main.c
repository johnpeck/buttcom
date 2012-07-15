/* bc_main.c */


// ----------------------- Include files ------------------------------
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>

#include "bc_functions.h"
#include "bc_main.h"

/* bc_clock.h
 * Provides fosc_cal() to set up a calibrated 1MHz system clock.
 */
#include "bc_clock.h"

/* bc_command.h 
 * Contains the extern declaration of command_array --
 * an array containing all the commands understood by the system. 
 * 
 * Defines the received command state structure recv_cmd_state_t.  Use
 * this to keep track of the remote interface state. */
#include "bc_command.h"
#include "bc_usart.h"

/* pgmspace.h
 * Contains macros and functions for saving and reading data out of
 * flash.
 */
#include <avr/pgmspace.h>

/* bc_logger.h sets up logging 
 */
#include "bc_logger.h"

/* bc_adc.h
 * Provides functions for configuring and using the Butterfly's ADC
 * module.
 */
#include "bc_adc.h"







// Define a pointer to the received command state
recv_cmd_state_t  recv_cmd_state;
recv_cmd_state_t *recv_cmd_state_ptr = &recv_cmd_state;



int main() {
    int retval = 0;
    sei(); // Enable interrupts
    fosc_cal(); // Set up calibrated 1MHz system clock
    usart_init(); // Set up the USART
    logger_init();
    logger_disable();
    logger_setsystem( "logger" ); // Enable logger system logging
    logger_setsystem( "rxchar" ); // Enable received character logging
    logger_setsystem( "command" ); // Enable command system logging
    logger_setsystem( "ranger" ); // Enable ranger system logging
    adc_init(); // Set the ADCs reference and SAR prescaler
    command_init( recv_cmd_state_ptr );
    for(;;) {
        process_pbuffer( recv_cmd_state_ptr, command_array );
    }// end main for loop
    return retval;
} // end main






/* -------------------------- Interrupts -------------------------------
 * Find the name of interrupt signala in iom169p.h and not pa.  Why
 * not?  We define the mcu name to be atmega169p in the makefile, not
 * atmega169pa. 
 * 
 * See the naming convention outlined at
 * http://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
 * to make sure you don't use depricated names. */
 

/* Interrupt on character received via the USART */
ISR(USART0_RX_vect) {
    // Write the received character to the buffer
    *(recv_cmd_state_ptr -> rbuffer_write_ptr) = UDR0;
    if (*(recv_cmd_state_ptr -> rbuffer_write_ptr) == '\r') {
        logger_msg_p("rxchar",log_level_INFO,
            PSTR("Received a command terminator.\r\n"));
        if ((recv_cmd_state_ptr -> rbuffer_count) == 0) {
            /* We got a terminator, but the received character buffer is
             * empty.  The user is trying to clear the transmit and
             * receive queues. */
            return;
        }
        else {
            if ((recv_cmd_state_ptr -> pbuffer_lock) == 1) {
                /* We got a terminator, and there are characters in the received
                 * character buffer, but the parse buffer is locked.  This is
                 * bad -- we're receiving commands faster than we can process
                 * them. */
                logger_msg_p("rxchar",log_level_ERROR,
                    PSTR("Command process speed error!\r\n"));
                rbuffer_erase(recv_cmd_state_ptr);
                return;
            }
            else {
                /* We got a terminator, and there are characters in the received
                 * character buffer.  The parse buffer is unlocked so terminate
                 * the received string and copy it to the parse buffer. */
                *(recv_cmd_state_ptr -> rbuffer_write_ptr) = '\0';
                strcpy((recv_cmd_state_ptr -> pbuffer),
                    (recv_cmd_state_ptr -> rbuffer));
                recv_cmd_state_ptr -> pbuffer_lock = 1;
                logger_msg_p("rxchar",log_level_INFO,
                    PSTR("Parse buffer contains '%s'.\r\n"),
                    (recv_cmd_state_ptr -> pbuffer));
                rbuffer_erase(recv_cmd_state_ptr);
                return;
            }
        }
    }
    else {
        // The character is not a command terminator.
        (recv_cmd_state_ptr -> rbuffer_count)++;
        logger_msg_p("rxchar",log_level_INFO,
            PSTR("%c  <-- copied to receive buffer.  Received count is %d.\r\n"),
            *(recv_cmd_state_ptr -> rbuffer_write_ptr),
            recv_cmd_state_ptr -> rbuffer_count);
        if ((recv_cmd_state_ptr -> rbuffer_count) >= (RECEIVE_BUFFER_SIZE-1)) {
            logger_msg_p("rxchar",log_level_ERROR,
                PSTR("Received character number above limit.\r\n"));
            rbuffer_erase(recv_cmd_state_ptr);
            return;
        }
        else {
            // Increment the write pointer
            (recv_cmd_state_ptr -> rbuffer_write_ptr)++;
        }
    }
    return;
}
