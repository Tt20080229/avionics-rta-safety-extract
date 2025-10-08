// FILE: apps/receipt_verify_app/src/receipt_verify_app.c
#include "apps/common/cfs_stub/cfs_stub.h"
#include "apps/common/pack_et_tlm.h"
#include "apps/receipt_verify_app/receipt_verify_msgs.h"
#include "reuse/libverify/verify.h"
#include "reuse/runtime/c_a653/ccsds_pus.h"
#include "reuse/runtime/c_a653/pus_events.h"
#include <stdio.h>
#include <stdlib.h>
int main(int argc,char** argv){
  if(argc<2){ fprintf(stderr,"usage: %s receipts.ndjson\n", argv[0]); return 1; }
  FILE* f=fopen(argv[1],"rb"); if(!f){ perror("open"); return 1; }
  fseek(f,0,SEEK_END); long sz=ftell(f); fseek(f,0,SEEK_SET);
  uint8_t* buf=(uint8_t*)malloc((size_t)sz); if(!buf){ return 1; }
  fread(buf,1,(size_t)sz,f); fclose(f);
  byte_span_t sp={.begin=buf,.len=(size_t)sz}; verify_stats_t st; verify_chain_ex(sp, 1, &st);
  uint8_t pkt[128]; size_t n=0; event_rl_t rl; events_init(&rl);
  event_send_verify(&rl, 0x322, st.ok_head, st.ok_parent, st.ok_body, pkt, &n);
  fwrite(pkt,1,n,stdout); fputc('\n',stdout);
  free(buf);
  return 0;
}
