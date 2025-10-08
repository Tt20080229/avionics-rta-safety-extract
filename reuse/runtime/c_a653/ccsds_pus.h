// FILE: reuse/runtime/c_a653/ccsds_pus.h
#ifndef CCSDS_PUS_H
#define CCSDS_PUS_H
#include <stddef.h>
#include <stdint.h>
size_t pus_tm_pack(uint8_t* out, uint16_t apid, uint16_t seq, uint8_t svc, uint8_t subsvc,
                   const uint8_t* user, uint16_t user_len, uint16_t* out_crc);
#endif
