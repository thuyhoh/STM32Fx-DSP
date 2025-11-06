#ifndef RCFILTER_H
#define RCFILTER_H

typedef struct{
    float coeff[2];
    float out[2];
}RCFilter_t;

void RCFilter_Init(RCFilter_t *filt, float cutoffFreqHz, float sampleTimes);
float RCFilter_Update(RCFilter_t *filt, float imp);

#endif