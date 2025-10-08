// FILE: reuse/runtime/c_a653/rta_envelope.c
#include "rta_envelope.h"
#include <math.h>
static inline float satf(float v,float lo,float hi){ if(!isfinite(v)) return 0.0f; return v<lo?lo:(v>hi?hi:v); }
float envelope_barrier(const state_t* x, const envelope_param_t* ep){
  float wx=fabsf(x->w_rad.x), wy=fabsf(x->w_rad.y), wz=fabsf(x->w_rad.z);
  float w_norm = wx>wy? (wx>wz?wx:wz):(wy>wz?wy:wz);
  float b1 = w_norm - ep->w_max + ep->b_margin;
  float b2 = fabsf(x->nz) - ep->nz_max + ep->b_margin;
  return b1>b2?b1:b2;
}
void envelope_bc(const state_t* x, float* thrust_b, vec3_t* rates_b){
  const float k=0.3f; (void)x;
  *thrust_b = 0.5f;
  rates_b->x = satf(-k*x->w_rad.x,-5.0f,5.0f);
  rates_b->y = satf(-k*x->w_rad.y,-5.0f,5.0f);
  rates_b->z = satf(-k*x->w_rad.z,-5.0f,5.0f);
}
