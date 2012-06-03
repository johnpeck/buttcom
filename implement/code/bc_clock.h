/* bc_clock.h
 * 
 * Functions for handling the system clock. 
 */
 
 /* fosc_cal(void)
 * This sets the frequency of the system clock provided by the internal
 * RC oscillator.  Since this frequency depends on voltage, time, and
 * temperature, the actual frequency is calibrated by comparing the
 * resulting system clock with the 32kHz crystal clock source.   In the
 * end, we'll have a 1MHz system clock to within about 2% */
void fosc_cal(void);
