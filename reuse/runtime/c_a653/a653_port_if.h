// FILE: reuse/runtime/c_a653/a653_port_if.h
#ifndef A653_PORT_IF_H
#define A653_PORT_IF_H
#include <stddef.h>
#include <stdint.h>
int  a653_read_sampling_port(uint32_t id, void* msg, size_t cap, size_t* out_sz);
int  a653_write_sampling_port(uint32_t id, const void* msg, size_t len);
#endif
