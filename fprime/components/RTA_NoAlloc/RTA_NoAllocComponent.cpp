// FILE: fprime/components/RTA_NoAlloc/RTA_NoAllocComponent.cpp
#include "RTA_NoAllocComponent.hpp"
#include <time.h>
static uint64_t now_us(){ struct timespec ts; clock_gettime(CLOCK_MONOTONIC,&ts); return (uint64_t)ts.tv_sec*1000000ull + (uint64_t)(ts.tv_nsec/1000ull); }
extern "C" uint64_t rta_monotonic_us(void){ return now_us(); }
RTA_NoAlloc::RTA_NoAlloc():m_cb(nullptr){
  envelope_param_t ep={2.5f,3.0f,0.05f}; rta_init(&m_ctx,&ep,200,200,500,0.0f,0.5f,-0.05f);
}
void RTA_NoAlloc::setParams(float wmax,float nzmax,float bmargin,
                            unsigned dn,unsigned dd,unsigned ds,
                            float bed,float bes,float ber){
  envelope_param_t ep={wmax,nzmax,bmargin}; rta_init(&m_ctx,&ep,dn,dd,ds,bed,bes,ber);
}
void RTA_NoAlloc::setTelemCb(void(*cb)(uint64_t,uint8_t,float,float,float,float)){ m_cb=cb; }
void RTA_NoAlloc::CmdIn(uint64_t ts_us, float thrust,float rx,float ry,float rz,float conf,uint8_t mode,uint32_t nic,
                        float wx,float wy,float wz,float nz){
  lec_cmd_t ac={ts_us,thrust,{rx,ry,rz},conf,mode,nic};
  state_t x={ts_us,{wx,wy,wz},nz}; safe_cmd_t out; rta_step_simplex(&m_ctx,&ac,&x,&out);
  if(m_cb) m_cb(out.ts_us,out.mode,out.thrust,out.rates_cmd.x,out.rates_cmd.y,out.rates_cmd.z);
}
