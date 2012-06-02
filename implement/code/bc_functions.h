/* bc_functions.h
 * 
 * Sample functions for the buttcom project */


/* Calibration structure */
struct cal_struct {
    uint8_t slope; // Measurements are multiplied by this factor before offset
    uint8_t offset; // Measurements are offset by this factor after slope is applied
};


/* This is the junk function.
 * It's great */
void junkfunc(uint16_t);


/* This is the crap function.
 * It's also great */
void crapfunc(uint16_t);
