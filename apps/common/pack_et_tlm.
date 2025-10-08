// FILE: apps/common/pack_et_tlm.c
#include "pack_et_tlm.h"
size_t tm_build_primary(uint8_t* out, uint16_t apid, uint16_t seq, uint16_t user_len){
  if(apid > 0x07FFu) return 0;
  const uint8_t ver_type_sec = 0x08;
  out[0] = ver_type_sec | (uint8_t)(apid >> 8);
  out[1] = (uint8_t)(apid & 0xFFu);
  out[2] = (uint8_t)((0xC0u) | ((seq >> 8) & 0x3Fu));
  out[3] = (uint8_t)(seq & 0xFFu);
  uint16_t length = (uint16_t)(user_len + 1u + 2u - 1u);
  be_put16(&out[4], length);
  out[6] = 0x01;
  return 7;
}
uint16_t crc16_ccsds_be(const uint8_t* p, size_t n){
  uint16_t poly = 0x1021, reg=0xFFFF;
  for(size_t i=0;i<n;i++){
    reg ^= ((uint16_t)p[i])<<8;
    for(int b=0;b<8;b++){
      if(reg & 0x8000u) reg = (uint16_t)((reg<<1) ^ poly);
      else reg <<= 1;
    }
  }
  return reg;
}
