// FILE: reuse/runtime/c_a653/rta_monitor.h
#ifndef RTA_MONITOR_H
#define RTA_MONITOR_H
#include <stdint.h>
#include "rta_types.h"
#include "rta_envelope.h"
typedef enum { MODE_NOMINAL=0, MODE_DEGRADED=2, MODE_SAFE=3 } rta_mode_t;
typedef struct {
  rta_state_t st; rta_mode_t mode; uint64_t last_switch_us;
  envelope_param_t env; struct { uint32_t dwell_nom_ms,dwell_deg_ms,dwell_safe_ms; float b_enter_deg,b_enter_safe,b_exit_recov; } fdir;
} rta_ctx_t;
void rta_init(rta_ctx_t* c, const envelope_param_t* ep, uint32_t dwell_nom_ms, uint32_t dwell_deg_ms, uint32_t dwell_safe_ms,
              float b_enter_deg, float b_enter_safe, float b_exit_recov);
void rta_step_simplex(rta_ctx_t* c, const lec_cmd_t* ac, const state_t* x, safe_cmd_t* out);
uint64_t rta_monotonic_us(void);
#endif
