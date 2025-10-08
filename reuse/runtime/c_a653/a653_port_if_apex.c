// FILE: reuse/runtime/c_a653/a653_port_if_apex.c
#include "a653_port_if.h"
int a653_read_sampling_port(uint32_t id, void* msg, size_t cap, size_t* out_sz){ (void)id; (void)msg; (void)cap; (void)out_sz; return 0; }
int a653_write_sampling_port(uint32_t id, const void* msg, size_t len){ (void)id; (void)msg; (void)len; return 0; }
