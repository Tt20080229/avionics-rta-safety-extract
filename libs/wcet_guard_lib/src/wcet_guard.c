// FILE: libs/wcet_guard_lib/src/wcet_guard.c
#include "wcet_guard.h"
#if defined(__x86_64__) && !defined(HOST_SIM)
static inline uint64_t rdtsc(void){ unsigned lo,hi; __asm__ __volatile__("rdtsc":"=a"(lo),"=d"(hi)); return ((uint64_t)hi<<32)|lo; }
uint64_t wcet_read_cycles(void){ return rdtsc(); }
uint64_t wcet_cycles_per_ms(void){ return 3000000ull; }
#else
#include <time.h>
uint64_t wcet_read_cycles(void){
  struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts);
  return (uint64_t)ts.tv_sec*1000000000ull + (uint64_t)ts.tv_nsec;
}
uint64_t wcet_cycles_per_ms(void){ return 1000000ull; }
#endif
