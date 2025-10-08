// FILE: libs/wcet_guard_lib/include/wcet_guard.h
#ifndef WCET_GUARD_H
#define WCET_GUARD_H
#include <stdint.h>
typedef struct { uint64_t begin_cycles; uint64_t budget_cycles; uint32_t hits; uint32_t over; } wcet_guard_t;
uint64_t wcet_read_cycles(void);
uint64_t wcet_cycles_per_ms(void);
static inline void wcet_begin(wcet_guard_t* g){ g->begin_cycles = wcet_read_cycles(); }
static inline int wcet_end(wcet_guard_t* g){
  uint64_t end = wcet_read_cycles();
  uint64_t delta = end - g->begin_cycles;
  g->hits++; if(delta > g->budget_cycles){ g->over++; return -1; } return 0;
}
#endif
