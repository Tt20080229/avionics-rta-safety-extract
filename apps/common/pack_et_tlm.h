// FILE: apps/common/pack_et_tlm.h
#ifndef PACK_ET_TLM_H
#define PACK_ET_TLM_H
#include <stddef.h>
#include <stdint.h>
size_t tm_build_primary(uint8_t* out, uint16_t apid, uint16_t seq, uint16_t user_len);
uint16_t crc16_ccsds_be(const uint8_t* p, size_t n);
static inline void be_put16(uint8_t* p, uint16_t v){ p[0]=(uint8_t)(v>>8); p[1]=(uint8_t)(v&0xFFu); }
#endif
