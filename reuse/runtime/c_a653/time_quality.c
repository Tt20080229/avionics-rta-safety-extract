// FILE: reuse/runtime/c_a653/time_quality.c
#include "time_quality.h"
void timeq_init(time_q_t* q, double max_stdev_us, unsigned bad_limit){
  q->offset_us=0; q->stdev_us=0; q->locked=0; q->bad_streak=0; q->bad_limit=bad_limit; q->max_stdev_us=max_stdev_us;
}
void timeq_sample(time_q_t* q, double offset_us, double stdev_us, int ref_locked){
  q->offset_us=offset_us; q->stdev_us=stdev_us;
  if(!ref_locked){ q->locked=0; if(q->bad_streak<q->bad_limit) q->bad_streak++; return; }
  if(stdev_us > q->max_stdev_us){
    if(q->bad_streak<q->bad_limit) q->bad_streak++;
    if(q->bad_streak >= q->bad_limit) q->locked=0;
  } else { q->bad_streak=0; q->locked=1; }
}
