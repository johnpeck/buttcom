/* bc_adc.h
 * 
 * Used to set up the ADC for the buttcom project. 
 */


/* ADC measurement calibration structure. 
 */
typedef struct adc_cal_struct { 
    int16_t cal_slop; // Slope calibration factor
    int16_t cal_offs; // Offset calibration factor
} adc_cal_t;

/* adc_init(void)
 * Initialize the Butterfly's 10-bit SAR ADC module 
 */
void adc_init(void);

/* adc_mux(uint8_t channel)
 * Set the ADCs input channel.
 */
void adc_mux(uint8_t channel);


/* adc_read(void)
 * Get a single 16-bit measurement from the currently selected ADC
 * channel.  The ADC has 10 bits of resolution.
 */
uint16_t adc_read(void);

/* Called by the remote command "volt?" Returns a calibrated voltage
 * measurement in fixed-point positive millivolts.
 */
// cmd_volt_q( uint16_t setval );
