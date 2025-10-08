// FILE: reuse/runtime/c_a653/time_quality.h
#ifndef TIME_QUALITY_H
#define TIME_QUALITY_H
typedef struct { double offset_us, stdev_us; int locked; unsigned bad_streak, bad_limit; double max_stdev_us; } time_q_t;
void timeq_init(time_q_t* q, double max_stdev_us, unsigned bad_limit);
void timeq_sample(time_q_t* q, double offset_us, double stdev_us, int ref_locked);
#endif
