/* bc_adc.c
 * 
 * Used to set up the ADC for the buttcom project. 
 */

/* bc_usart.h provides functions for transmitting characters over the
 * usart.
 */
#include "bc_usart.h"

/* pgmspace.h
 * Provides macros and functions for saving and reading data out of
 * flash.
 */
#include <avr/pgmspace.h>

/* stdint.h
 * Defines fixed-width integer types like uint8_t
 */
#include <stdint.h>

/* avr/io.h
 * Device-specific port definitions
 */
#include <avr/io.h>

/* bc_logger.h
 * Provides logger_msg and logger_msg_p for log messages tagged with
 * a system and severity.
 */
#include "bc_logger.h"

#include "bc_adc.h"

/* The voltage measurement calibration factors
 */
adc_cal_t volt_calfactor = {
    1, // Slope (mV/count)
    0  // Offset (mV)
};
adc_cal_t *volt_calfactor_ptr = &volt_calfactor;

/* cmd_vslope(uint16_t vslope)
 * Set the voltage measurement's slope calibration factor.
 */
void cmd_vslope(uint16_t vslope) {
    volt_calfactor_ptr -> cal_slope = vslope;
}

/* cmd_voffset(uint16_t voffset)
 * Set the voltage measurement's offset calibration factor.
 */
void cmd_voffset(uint16_t voffset) {
    volt_calfactor_ptr -> cal_offset = voffset;
}

/* Initialize the ADC.  */
void adc_init(void) {
    logger_msg_p("adc",log_level_INFO, PSTR("Initializing ADC.\r\n"));
    /* The butterfly has Vcc connected to AVcc via a low-pass filter.
     * It also has a shunt capacitor at the Aref pin.  So I can use the
     * voltage at AVcc as the reference. */
    ADMUX = (0<<REFS1) | (1<<REFS0);

    /* Right-justify the data in the high and low registers by clearing
     * ADLAR.  This is a 10-bit ADC. */
    ADMUX &= ~(_BV(ADLAR));

    /* The butterfly's analog input is connected to ADC1 on pin 60.
     * This makes a nice initialization value. */
    ADMUX |= 1;

    /* Enable the ADC and set the fosc --> fsar prescaler.
     * The SAR conversion requires between 50 and 200kHz for 10-bit
     * resolution, but can be set as high as fosc/2 for lower resolution.
     * Set the prescaler to 8 to get fsar = 125kHz.
     * Normal conversions take 13 cycles, or 13*8us = 104us for
     * fsar = 125kHz.  The maximum conversion rate is thus fsar / 13
     * or 9.6kHz for fsar = 125kHz. 
     * 
     * ADCSRA = (1<<ADEN) | (1<<ADPS1) | (1<<ADPS0); fsar = 125kHz
     */
    ADCSRA = (1<<ADEN) | (1<<ADPS1) | (1<<ADPS0);

    /* Disable auto-triggering.  We'll trigger the ADC manually. */
    ADCSRA &= ~(_BV(ADATE));

    /* The first ADC conversion will take 25 ADC clock cycles instead of
     * the normal 13.  The first one initializes the ADC. Take a single
     * conversion for this initialization step. */
    ADCSRA |= (1<<ADSC);

    /* The ADIF bit in the ADCSRA register will be set when the conversion
     * is finished. Wait for conversion to finish. */
    while(!(ADCSRA & (1<<ADIF)));
}

/* Set the mux channel for the ADC input.
 * channel = 0 -- ADC0
 * channel = 1 -- ADC1 (Butterfly's voltage reader)
 * ...
 * channel = 7 -- ADC7
 *
 * The mux selection overrides any data direction selection made with
 * DDRF.  See section 13.3 of the datasheet. 
 */
void adc_mux(uint8_t channel) {
    ADMUX &= (1<<REFS1) | (1<<REFS0) | (1<<ADLAR);
    ADMUX |= channel;
}

/* adc_read() 
 * Return a measurement made with the ADC.
 */
uint16_t adc_read(void) {
    uint16_t adc_temp = 0;

    /* Enable the ADC.  It seems like I already did this in adc_init(),
     * but the part locks up if I don't also do it here. */
    ADCSRA |= _BV(ADEN);

    ADCSRA |= (1<<ADSC);  // Do a single conversion
    while(!(ADCSRA & (1<<ADIF)));  // Wait for the conversion to finish
    adc_temp = ADCL;            // Read the lower 8 bits
    adc_temp += (ADCH << 8);    // Add the upper 2 bits
    return adc_temp;
}

/* cmd_vcounts_q(void)
 * Query the raw ADC counts from the voltage measurement.
 */
void cmd_vcounts_q(void) {
    uint16_t adc_temp = 0;
    adc_temp = adc_read();
    usart_printf_p(PSTR("0x%x\r\n"),adc_temp);
}

/* cmd_volt_q(void)
 * Query the calibrated voltage measurement.  The voltage in mV is arrived
 * at with:
 * mV = ((ADC counts) * vslope >> 16) - voffset 
 * Notice that voffset is assumed to be positive -- all these numbers are
 * unsigned 16-bit integers. */
void cmd_volt_q(void) {
    uint16_t raw_counts = 0;
    uint16_t result_mv = 0;
    raw_counts = adc_read();
    result_mv = ((raw_counts * volt_calfactor_ptr -> cal_slope) >> 4) +
                volt_calfactor_ptr -> cal_offset;
    usart_printf_p(PSTR("%u\r\n"),result_mv);
}
