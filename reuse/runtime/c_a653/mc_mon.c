// FILE: reuse/runtime/c_a653/mc_mon.c
#include "mc_mon.h"
#include <time.h>
void mc_read(mc_counters_t* o){
  struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts);
  o->cycles = (uint64_t)ts.tv_sec*1000000000ull + (uint64_t)ts.tv_nsec;
}
