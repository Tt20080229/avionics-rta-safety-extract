// FILE: reuse/libverify/sha256.c
#include "sha256.h"
#define ROR(x,n) (((x)>>(n))|((x)<<(32-(n))))
static uint32_t K[64]={
0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2};
static void T(sha256_ctx* c){
  uint32_t w[64]; for(int i=0;i<16;i++){ size_t j=i*4;
    w[i]=((uint32_t)c->b[j]<<24)|((uint32_t)c->b[j+1]<<16)|((uint32_t)c->b[j+2]<<8)|((uint32_t)c->b[j+3]); }
  for(int i=16;i<64;i++){ uint32_t s0=ROR(w[i-15],7)^ROR(w[i-15],18)^(w[i-15]>>3); uint32_t s1=ROR(w[i-2],17)^ROR(w[i-2],19)^(w[i-2]>>10); w[i]=w[i-16]+s0+w[i-7]+s1; }
  uint32_t a=c->s[0],b=c->s[1],d=c->s[3],c2=c->s[2],e=c->s[4],f=c->s[5],g=c->s[6],h=c->s[7];
  for(int i=0;i<64;i++){
    uint32_t S1=ROR(e,6)^ROR(e,11)^ROR(e,25); uint32_t ch=(e&f)^(~e&g); uint32_t temp1=h+S1+ch+K[i]+w[i];
    uint32_t S0=ROR(a,2)^ROR(a,13)^ROR(a,22); uint32_t maj=(a&b)^(a&c2)^(b&c2); uint32_t temp2=S0+maj;
    h=g; g=f; f=e; e=d+temp1; d=c2; c2=b; b=a; a=temp1+temp2;
  }
  c->s[0]+=a; c->s[1]+=b; c->s[2]+=c2; c->s[3]+=d; c->s[4]+=e; c->s[5]+=f; c->s[6]+=g; c->s[7]+=h;
}
void sha256_init(sha256_ctx* c){
  c->s[0]=0x6a09e667;c->s[1]=0xbb67ae85;c->s[2]=0x3c6ef372;c->s[3]=0xa54ff53a;
  c->s[4]=0x510e527f;c->s[5]=0x9b05688c;c->s[6]=0x1f83d9ab;c->s[7]=0x5be0cd19;c->n=0;c->m=0;
}
void sha256_update(sha256_ctx* c, const uint8_t* p, size_t n){
  for(size_t i=0;i<n;i++){ c->b[c->m++]=p[i]; if(c->m==64){ T(c); c->n+=512; c->m=0; } }
}
void sha256_final(sha256_ctx* c, uint8_t out[32]){
  c->n += (uint64_t)c->m*8ull; c->b[c->m++]=0x80; if(c->m>56){ while(c->m<64) c->b[c->m++]=0; T(c); c->m=0; }
  while(c->m<56) c->b[c->m++]=0; for(int i=7;i>=0;i--) c->b[c->m++]=(uint8_t)((c->n>>(i*8))&0xFFu); T(c);
  for(int i=0;i<8;i++){ out[i*4]=(uint8_t)(c->s[i]>>24); out[i*4+1]=(uint8_t)(c->s[i]>>16); out[i*4+2]=(uint8_t)(c->s[i]>>8); out[i*4+3]=(uint8_t)c->s[i]; }
}
