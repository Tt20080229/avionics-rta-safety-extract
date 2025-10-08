// FILE: reuse/runtime/c_a653/stl_rt.h
#ifndef STL_RT_H
#define STL_RT_H
#include <stdint.h>
#include "rta_types.h"
typedef struct { uint32_t win_len; uint32_t idx; float buf_x[256],buf_y[256],buf_z[256]; } stl_wmax_t;
void stl_wmax_init(stl_wmax_t* m, uint32_t win_len);
void stl_wmax_push(stl_wmax_t* m, vec3_t w);
float stl_robust_wmax(const stl_wmax_t* m, float W_MAX);
typedef struct { float last; } stl_monotone_t;
void stl_monotone_init(stl_monotone_t* m, float init);
int  stl_monotone_check(stl_monotone_t* m, float v, float slack);
#endif
