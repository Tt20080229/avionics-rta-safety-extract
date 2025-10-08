// FILE: apps/rta_monitor_app/rta_monitor_msgs.h
#ifndef RTA_MONITOR_MSGS_H
#define RTA_MONITOR_MSGS_H
#include <stdint.h>
typedef struct __attribute__((__packed__)){ uint64_t ts_us; float w_x,w_y,w_z; float nz; } state_sp_t;
typedef struct __attribute__((__packed__)){ uint64_t ts_us; float thrust; float rx,ry,rz; float conf; uint8_t mode; uint32_t nic; } lec_cmd_sp_t;
#endif
