// FILE: fprime/components/RTA_NoAlloc/RTA_NoAllocComponent.hpp
#ifndef RTA_NOALLOC_COMPONENT_HPP
#define RTA_NOALLOC_COMPONENT_HPP
#include "reuse/runtime/c_a653/rta_monitor.h"
class RTA_NoAlloc {
public:
  RTA_NoAlloc();
  void setParams(float wmax,float nzmax,float bmargin,
                 unsigned dwellNom,unsigned dwellDeg,unsigned dwellSafe,
                 float bEnterDeg,float bEnterSafe,float bExitRec);
  void CmdIn(uint64_t ts_us, float thrust, float rx,float ry,float rz, float conf, uint8_t mode, uint32_t nic,
             float wx,float wy,float wz,float nz);
  void setTelemCb(void(*cb)(uint64_t,uint8_t,float,float,float,float));
private:
  rta_ctx_t m_ctx; void(*m_cb)(uint64_t,uint8_t,float,float,float,float);
};
#endif
