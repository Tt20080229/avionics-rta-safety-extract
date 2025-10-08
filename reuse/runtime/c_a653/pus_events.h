// FILE: reuse/runtime/c_a653/pus_events.h
#ifndef PUS_EVENTS_H
#define PUS_EVENTS_H
#include <stdint.h>
typedef enum { EV_RTA_SWITCH=1, EV_RTA_VIOL=2, EV_HEALTH=3, EV_VERIFY=4 } event_id_t;
typedef struct { uint64_t last_us; uint32_t min_gap_us; } rate_limit_t;
typedef struct { rate_limit_t sw, viol, health, verify; } event_rl_t;
void events_init(event_rl_t* rl);
int  event_send_switch(event_rl_t* rl, uint16_t apid, uint8_t from, uint8_t to, uint16_t reason, uint8_t* out, size_t* outn);
int  event_send_verify(event_rl_t* rl, uint16_t apid, uint32_t ok_head, uint32_t ok_parent, uint32_t ok_body, uint8_t* out, size_t* outn);
uint64_t events_monotonic_us(void);
#endif
