// FILE: apps/receipt_verify_app/receipt_verify_msgs.h
#ifndef RECEIPT_VERIFY_MSGS_H
#define RECEIPT_VERIFY_MSGS_H
#include <stdint.h>
typedef struct __attribute__((__packed__)){ uint32_t lines, ok_head, ok_parent, ok_body, bad_head, bad_parent, bad_body; } verify_stats_tm_t;
#endif
