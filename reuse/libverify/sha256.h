// FILE: reuse/libverify/sha256.h
#ifndef SHA256_MINI_H
#define SHA256_MINI_H
#include <stddef.h>
#include <stdint.h>
typedef struct{ uint32_t s[8]; uint64_t n; uint8_t b[64]; size_t m; } sha256_ctx;
void sha256_init(sha256_ctx* c);
void sha256_update(sha256_ctx* c, const uint8_t* p, size_t n);
void sha256_final(sha256_ctx* c, uint8_t out[32]);
#endif
