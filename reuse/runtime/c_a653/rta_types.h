// FILE: reuse/runtime/c_a653/rta_types.h
#ifndef RTA_TYPES_H
#define RTA_TYPES_H
#include <stdint.h>
typedef struct { float x,y,z; } vec3_t;
typedef struct { uint64_t ts_us; vec3_t w_rad; float nz; } state_t;
typedef struct { uint64_t ts_us; float thrust; vec3_t rates_cmd; float conf; uint8_t mode; uint32_t nic; } lec_cmd_t;
typedef struct { uint64_t ts_us; float thrust; vec3_t rates_cmd; uint8_t mode; uint8_t switch_flag; uint16_t reason_code; } safe_cmd_t;
typedef struct { vec3_t last_rates; uint8_t last_mode; float e_value; float alpha_budget; } rta_state_t;
#endif
