// FILE: reuse/runtime/c_a653/rta_monitor.c
#include "rta_monitor.h"
static int dwell_ok(const rta_ctx_t* c, uint32_t need_ms){
  uint64_t delta = rta_monotonic_us() - c->last_switch_us; return delta >= (uint64_t)need_ms*1000ull;
}
void rta_init(rta_ctx_t* c, const envelope_param_t* ep, uint32_t dn, uint32_t dd, uint32_t ds,
              float bed, float bes, float ber){
  c->st.last_rates.x=c->st.last_rates.y=c->st.last_rates.z=0.0f;
  c->st.last_mode=MODE_NOMINAL; c->st.e_value=1.0f; c->st.alpha_budget=0.0f;
  c->mode=MODE_NOMINAL; c->last_switch_us=0; c->env=*ep;
  c->fdir.dwell_nom_ms=dn; c->fdir.dwell_deg_ms=dd; c->fdir.dwell_safe_ms=ds;
  c->fdir.b_enter_deg=bed; c->fdir.b_enter_safe=bes; c->fdir.b_exit_recov=ber;
}
void rta_step_simplex(rta_ctx_t* c, const lec_cmd_t* ac, const state_t* x, safe_cmd_t* out){
  float b = envelope_barrier(x,&c->env);
  rta_mode_t target=c->mode;
  if(b > c->fdir.b_enter_safe && dwell_ok(c,c->fdir.dwell_safe_ms)) target=MODE_SAFE;
  else if(b > c->fdir.b_enter_deg && dwell_ok(c,c->fdir.dwell_deg_ms)) target=MODE_DEGRADED;
  else if(b < c->fdir.b_exit_recov && dwell_ok(c,c->fdir.dwell_nom_ms)) target=MODE_NOMINAL;
  if(target!=c->mode){ c->mode=target; c->last_switch_us=rta_monotonic_us(); }
  float t; vec3_t r;
  if(c->mode==MODE_SAFE){ envelope_bc(x,&t,&r); out->switch_flag=1; out->reason_code=1; }
  else if(c->mode==MODE_DEGRADED){
    float tb; vec3_t rb; envelope_bc(x,&tb,&rb); const float beta=0.6f;
    t = beta*tb + (1.0f-beta)*ac->thrust;
    r.x = beta*rb.x + (1.0f-beta)*ac->rates_cmd.x;
    r.y = beta*rb.y + (1.0f-beta)*ac->rates_cmd.y;
    r.z = beta*rb.z + (1.0f-beta)*ac->rates_cmd.z;
    out->switch_flag=0; out->reason_code=2;
  } else { t=ac->thrust; r=ac->rates_cmd; out->switch_flag=0; out->reason_code=0; }
  out->ts_us=x->ts_us; out->thrust=t; out->rates_cmd=r; out->mode=(uint8_t)c->mode;
  c->st.last_rates=r; c->st.last_mode=(uint8_t)c->mode;
}
__attribute__((weak)) uint64_t rta_monotonic_us(void){ return 0; }
