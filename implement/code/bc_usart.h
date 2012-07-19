/* bc_usart.h
 * 
 * Used to set up the USART for the buttcom project. 
 */

/* stdint.h
 * Defines fixed-width integer types like uint8_t
 */
#include <stdint.h>

/* Define the maximum string length that will be sent to the USART.
 */
#define USART_TXBUFFERSIZE 150

/* Send a format string to the USART interface 
 */
uint8_t usart_printf (const char *fmt, ...);

/* Send a format string stored in flash memory to the USART interface.
 */
uint8_t usart_printf_p(const char *fmt, ...);

/* usart_receive
 * Simple USART receive function based on polling of the receive
 * complete (RXCn) flag.  The Butterfly has only one USART, so n will
 * always be zero.  The USART must be initialized before this can be used.
 *
 * The function simply waits for data to be present in the receive buffer
 * by checking the RXCn flag.  This flag is set when data is present. 
 */
unsigned char usart_receive(void);

/* usart_putc(char data)
 * Sends a character to the USART 
 */
void usart_putc(char data);

/* usart_puts(char s[])
 * Sends a string over the USART by repeatedly calling usart_putc() 
 */
void usart_puts(const char s[]);

/* usart_puts_p(const char *data)
 * Sends strings stored in flash memory to the USART.
 */
void usart_puts_p(const char *data);

/* usart_init()
 * Initialize the USART for 9600 baud, 8 data bits, 1 stop bit, no parity
 * checking. 
 */
void usart_init(void);
