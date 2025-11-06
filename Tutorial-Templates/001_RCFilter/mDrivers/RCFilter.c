#include "RCFilter.h"

void RCFilter_Init(RCFilter_t *filt, float cutoffFreqHz, float sampleTimes)
{
    float RC = 1.0f/(6.28318530718f * cutoffFreqHz);

    filt->coeff[0] = sampleTimes/(sampleTimes + RC);
    filt->coeff[1] = RC/ (RC + sampleTimes);
    
    filt->out[0] = 0.0f;
    filt->out[1] = 0.0f;
}
float RCFilter_Update(RCFilter_t *filt, float inp)
{
    filt->out[1] = filt->out[0];
    filt->out[0] = filt->coeff[0]*inp + filt->coeff[1]*filt->out[1];
    return filt->out[0];
}
