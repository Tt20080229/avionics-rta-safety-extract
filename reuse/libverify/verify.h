// FILE: reuse/libverify/verify.h
#ifndef VERIFY2_H
#define VERIFY2_H
#include <stddef.h>
#include <stdint.h>
typedef struct { const uint8_t* begin; size_t len; } byte_span_t;
typedef struct { uint32_t lines, ok_head, ok_parent, ok_body, bad_parent, bad_head, bad_body; } verify_stats_t;
int verify_chain_ex(byte_span_t ndjson, int strict_body, verify_stats_t* st);
#endif
