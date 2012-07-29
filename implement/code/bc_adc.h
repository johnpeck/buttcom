/* bc_adc.h
 * 
 * Used to set up the ADC for the buttcom project. 
 */


/* ADC measurement calibration structure. 
 */
typedef struct adc_cal_struct { 
    uint16_t cal_slope; // Slope calibration factor
    uint16_t cal_offset; // Offset calibration factor
} adc_cal_t;

/* adc_init(void)
 * Initialize the Butterfly's 10-bit SAR ADC module.
 *     Set the default ADC mux position to 1: the voltage reader
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

/* cmd_vcounts_q()
 * Query the raw ADC reading from the voltage measurement -- before
 * slope and offset are applied.
 */
void cmd_vcounts_q(uint16_t nonval);

/* cmd_volt_q(void)
 * Query the voltage measurement.  Returns a calibrated value in
 * millivolts. 
 */
void cmd_volt_q(uint16_t nonval);

/* cmd_vslope
 * Set the voltage measurement slope factor.  ADC data will be multiplied
 * by this factor before being downshifted by 4 bits and given an offset.
 * The ultimate output will be in 1 bit = 1 mV.
 */
void cmd_vslope(uint16_t vslope);

/* cmd_voffset
 * Set the offset adjustment in mV.  This will be subtracted from the
 * slope-corrected voltage output.
 */
void cmd_voffset(uint16_t voffset);
