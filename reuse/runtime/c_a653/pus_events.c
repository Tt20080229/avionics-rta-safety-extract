// FILE: reuse/runtime/c_a653/pus_events.c
#include "pus_events.h"
#include "ccsds_pus.h"
static int rl_ok(rate_limit_t* r){ uint64_t now=events_monotonic_us(); if(now - r->last_us < r->min_gap_us) return 0; r->last_us=now; return 1; }
void events_init(event_rl_t* rl){ rl->sw.min_gap_us=200000; rl->viol.min_gap_us=100000; rl->health.min_gap_us=500000; rl->verify.min_gap_us=300000; }
int event_send_switch(event_rl_t* rl, uint16_t apid, uint8_t from, uint8_t to, uint16_t reason, uint8_t* out, size_t* outn){
  if(!rl_ok(&rl->sw)) return 0; uint8_t user[6]={from,to,(uint8_t)(reason>>8),(uint8_t)reason,0,0}; uint16_t crc; size_t n = pus_tm_pack(out, apid, 0, 5, EV_RTA_SWITCH, user, 6, &crc); *outn = n; return 1;
}
int event_send_verify(event_rl_t* rl, uint16_t apid, uint32_t ok_head, uint32_t ok_parent, uint32_t ok_body, uint8_t* out, size_t* outn){
  if(!rl_ok(&rl->verify)) return 0;
  uint8_t u[12]; u[0]=(uint8_t)(ok_head>>24); u[1]=(uint8_t)(ok_head>>16); u[2]=(uint8_t)(ok_head>>8); u[3]=(uint8_t)ok_head;
  u[4]=(uint8_t)(ok_parent>>24); u[5]=(uint8_t)(ok_parent>>16); u[6]=(uint8_t)(ok_parent>>8); u[7]=(uint8_t)ok_parent;
  u[8]=(uint8_t)(ok_body>>24); u[9]=(uint8_t)(ok_body>>16); u[10]=(uint8_t)(ok_body>>8); u[11]=(uint8_t)ok_body;
  uint16_t crc; size_t n = pus_tm_pack(out,  apid, 0, 5, EV_VERIFY, u, 12, &crc); *outn=n; return 1;
}
__attribute__((weak)) uint64_t events_monotonic_us(void){ return 0; }
