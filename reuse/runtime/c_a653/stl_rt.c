// FILE: reuse/runtime/c_a653/stl_rt.c
#include "stl_rt.h"
#include <math.h>
void stl_wmax_init(stl_wmax_t* m, uint32_t win_len){ if(win_len==0) win_len=1; if(win_len>256) win_len=256; m->win_len=win_len; m->idx=0; }
void stl_wmax_push(stl_wmax_t* m, vec3_t w){
  uint32_t L=m->win_len; uint32_t i = m->idx % L;
  m->buf_x[i]=fabsf(w.x); m->buf_y[i]=fabsf(w.y); m->buf_z[i]=fabsf(w.z); m->idx++;
}
float stl_robust_wmax(const stl_wmax_t* m, float W_MAX){
  uint32_t L=m->win_len; if(L==0) L=1; uint32_t n = (m->idx<L)?m->idx:L;
  float mx=0, my=0, mz=0;
  for(uint32_t i=0;i<n;i++){ if(m->buf_x[i]>mx) mx=m->buf_x[i]; if(m->buf_y[i]>my) my=m->buf_y[i]; if(m->buf_z[i]>mz) mz=m->buf_z[i]; }
  float worst = mx>my? (mx>mz?mx:mz) : (my>mz?my:mz);
  return worst - W_MAX;
}
void stl_monotone_init(stl_monotone_t* m, float init){ m->last=init; }
int  stl_monotone_check(stl_monotone_t* m, float v, float slack){ int ok = (v >= m->last - slack); m->last=v; return ok; }
