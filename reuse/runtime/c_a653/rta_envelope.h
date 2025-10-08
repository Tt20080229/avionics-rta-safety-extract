// FILE: reuse/runtime/c_a653/rta_envelope.h
#ifndef RTA_ENVELOPE_H
#define RTA_ENVELOPE_H
#include "rta_types.h"
typedef struct { float w_max; float nz_max; float b_margin; } envelope_param_t;
float envelope_barrier(const state_t* x, const envelope_param_t* ep);
void  envelope_bc(const state_t* x, float* thrust_b, vec3_t* rates_b);
#endif
