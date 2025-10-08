// FILE: reuse/runtime/c_a653/ccsds_pus.c
#include "ccsds_pus.h"
#include "../../apps/common/pack_et_tlm.h"
#include <string.h>
size_t pus_tm_pack(uint8_t* out, uint16_t apid, uint16_t seq, uint8_t svc, uint8_t subsvc,
                   const uint8_t* user, uint16_t user_len, uint16_t* out_crc){
  if(user_len> (uint16_t)1024) return 0;
  size_t off = tm_build_primary(out, apid, seq, (uint16_t)(2u + user_len));
  if(off==0) return 0;
  out[off+0]=svc; out[off+1]=subsvc; off+=2;
  if(user_len){ memcpy(&out[off], user, user_len); off += user_len; }
  uint16_t crc = crc16_ccsds_be(out, off);
  out[off++] = (uint8_t)(crc>>8); out[off++] = (uint8_t)(crc&0xFFu);
  if(out_crc) *out_crc=crc;
  return off;
}
