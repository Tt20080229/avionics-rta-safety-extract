// FILE: reuse/libverify/verify.c
#include "verify.h"
#include "sha256.h"
#include <string.h>
static int ct_memeq(const void* a, const void* b, size_t n){ const uint8_t* x=a; const uint8_t* y=b; unsigned v=0; for(size_t i=0;i<n;i++) v|=(unsigned)(x[i]^y[i]); return v==0; }
static const uint8_t* find_sub(const uint8_t* s,size_t n,const char* pat){ size_t m=strlen(pat); if(m==0||n<m) return NULL; for(size_t i=0;i<=n-m;i++){ if(s[i]==(uint8_t)pat[0] && memcmp(&s[i],pat,m)==0) return &s[i]; } return NULL; }
static int extract_string_value_exact(const uint8_t* l,size_t n,const char* key,const uint8_t** v,size_t* vn){
  const uint8_t* kp=find_sub(l,n,key); if(!kp) return -1; const uint8_t* p=kp+strlen(key);
  const uint8_t* q=p; int esc=0; for(; (size_t)(q-l)<n; q++){ uint8_t c=*q; if(esc){ esc=0; continue; } if(c=='\\'){ esc=1; continue; } if(c=='\"') break; }
  if((size_t)(q-l)>=n) return -2; *v=p; *vn=(size_t)(q-p); return 0;
}
static int extract_partition_scope(const uint8_t* l,size_t n,const uint8_t** s,size_t* sn){
  const uint8_t* kp=find_sub(l,n,"\"partition\":{"); if(!kp) return -1; const uint8_t* p=kp+12; int depth=1; const uint8_t* q=p;
  for(; (size_t)(q-l)<n; q++){ if(*q=='{' ) depth++; else if(*q=='}'){ depth--; if(depth==0){ *s=p; *sn=(size_t)(q-p); return 0; } } else if(*q=='\"'){ q++; int esc=0; for(; (size_t)(q-l)<n; q++){ if(esc){ esc=0; continue; } if(*q=='\\'){ esc=1; continue; } if(*q=='\"') break; } } }
  return -2;
}
static int extract_partition_id_scoped(const uint8_t* l,size_t n,const uint8_t** v,size_t* vn){
  const uint8_t* s; size_t sn; if(extract_partition_scope(l,n,&s,&sn)!=0) return -1;
  return extract_string_value_exact(s,sn,"\"id\":\"",v,vn);
}
static int hex32_nibbles(const uint8_t* v,size_t n,uint8_t out[32]){
  if(n!=64) return -1;
  for(size_t i=0;i<32;i++){
    uint8_t h=v[2*i], l=v[2*i+1];
    int hv=(h>='0'&&h<='9')?h-'0':(h>='a'&&h<='f')?h-'a'+10:(h>='A'&&h<='F')?h-'A'+10:-1;
    int lv=(l>='0'&&l<='9')?l-'0':(l>='a'&&l<='f')?l-'a'+10:(l>='A'&&l<='F')?l-'A'+10:-1;
    if(hv<0||lv<0) return -2; out[i]=(uint8_t)((hv<<4)|lv);
  }
  return 0;
}
static int recompute_head_digest(const uint8_t* line,size_t ln,uint8_t out32[32]){
  const uint8_t *vd=NULL,*vp=NULL; size_t nd=0,np=0;
  if(extract_string_value_exact(line,ln,"\"decision_id\":\"",&vd,&nd)!=0) return -1;
  if(extract_partition_id_scoped(line,ln,&vp,&np)!=0) return -2;
  static const char *p1="{\"decision_id\":\"", *p2="\",\"partition_id\":\"", *p3="\"}";
  sha256_ctx c; sha256_init(&c);
  sha256_update(&c,(const uint8_t*)p1,strlen(p1));
  sha256_update(&c,vd,nd);
  sha256_update(&c,(const uint8_t*)p2,strlen(p2));
  sha256_update(&c,vp,np);
  sha256_update(&c,(const uint8_t*)p3,strlen(p3));
  sha256_final(&c,out32);
  return 0;
}
int verify_chain_ex(byte_span_t in, int strict_body, verify_stats_t* st){
  memset(st,0,sizeof(*st));
  const uint8_t* p=in.begin; size_t n=in.len; size_t i=0; uint8_t prev_head[32]; int have_prev=0;
  while(i<n){
    size_t j=i; while(j<n && p[j]!='\n') j++;
    if(j>i){
      st->lines++; const uint8_t* L=&p[i]; size_t ln=j-i;
      const uint8_t *vh,*vb,*vs,*vp; size_t nh,nb,ns,nph;
      uint8_t head_hex[32], head_re[32];
      int head_ok=0;
      if(extract_string_value_exact(L,ln,"\"head\":\"0x",&vh,&nh)==0 && nh==64 &&
         hex32_nibbles(vh,nh,head_hex)==0 && recompute_head_digest(L,ln,head_re)==0 && ct_memeq(head_hex,head_re,32)){ head_ok=1; st->ok_head++; }
      else st->bad_head++;
      if(extract_string_value_exact(L,ln,"\"parent_head\":\"0x",&vp,&nph)==0){
        if(nph==64){
          if(have_prev){ uint8_t ph[32]; if(hex32_nibbles(vp,nph,ph)==0 && ct_memeq(ph,prev_head,32)) st->ok_parent++; else st->bad_parent++; }
          else { uint8_t z[32]={0}, ph[32]; if(hex32_nibbles(vp,nph,ph)==0 && ct_memeq(ph,z,32)) st->ok_parent++; else st->bad_parent++; }
        } else st->bad_parent++;
      } else { if(!have_prev) st->ok_parent++; else st->bad_parent++; }
      if(extract_string_value_exact(L,ln,"\"body\":\"0x",&vb,&nb)==0){
        if(nb==64){
          if(extract_string_value_exact(L,ln,"\"body_src\":\"",&vs,&ns)==0) st->ok_body++;
          else { if(strict_body) st->bad_body++; else st->ok_body++; }
        } else { if(strict_body) st->bad_body++; }
      } else { if(strict_body) st->bad_body++; }
      if(head_ok){ memcpy(prev_head,head_re,32); have_prev=1; }
    }
    i=j+1;
  }
  return 0;
}
