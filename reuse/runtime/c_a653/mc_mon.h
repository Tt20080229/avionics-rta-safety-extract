// FILE: reuse/runtime/c_a653/mc_mon.h
#ifndef MC_MON_H
#define MC_MON_H
#include <stdint.h>
typedef struct { uint64_t cycles; } mc_counters_t;
void mc_read(mc_counters_t* o);
#endif
