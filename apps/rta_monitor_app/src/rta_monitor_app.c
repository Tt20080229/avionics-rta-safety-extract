// FILE: apps/rta_monitor_app/src/rta_monitor_app.c
#include "apps/common/cfs_stub/cfs_stub.h"
#include "apps/common/pack_et_tlm.h"
#include "apps/rta_monitor_app/rta_monitor_msgs.h"
#include "libs/wcet_guard_lib/include/wcet_guard.h"
#include "reuse/runtime/c_a653/rta_monitor.h"
#include "reuse/runtime/c_a653/pus_events.h"
#include "reuse/runtime/c_a653/ccsds_pus.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
static uint64_t host_now_us(void){ struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts); return (uint64_t)ts.tv_sec*1000000ull + (uint64_t)(ts.tv_nsec/1000ull); }
uint64_t rta_monotonic_us(void){ return host_now_us(); }
uint64_t events_monotonic_us(void){ return host_now_us(); }
int main(int argc, char** argv){
  if(argc<2){ fprintf(stderr,"usage: %s state_cmd.csv\n", argv[0]); return 1; }
  FILE* f=fopen(argv[1],"r"); if(!f){ perror("open csv"); return 1; }
  rta_ctx_t ctx; envelope_param_t ep={2.5f,3.0f,0.05f}; rta_init(&ctx,&ep,200,200,500,0.0f,0.5f,-0.05f);
  event_rl_t rl; events_init(&rl);
  wcet_guard_t g; g.budget_cycles = wcet_cycles_per_ms()*1;
  char line[256];
  while(fgets(line,sizeof line,f)){
    state_sp_t s; lec_cmd_sp_t c;
    uint64_t ts; float wx,wy,wz,nz,th,rx,ry,rz,conf; unsigned mode; unsigned nic;
    if(sscanf(line,"%llu,%f,%f,%f,%f,%f,%f,%f,%f,%f,%u,%u",
              (unsigned long long*)&ts,&wx,&wy,&wz,&nz,&th,&rx,&ry,&rz,&conf,&mode,&nic)!=12) continue;
    s.ts_us=ts; s.w_x=wx; s.w_y=wy; s.w_z=wz; s.nz=nz;
    c.ts_us=ts; c.thrust=th; c.rx=rx; c.ry=ry; c.rz=rz; c.conf=conf; c.mode=(uint8_t)mode; c.nic=nic;
    state_t x={.ts_us=s.ts_us, .w_rad={s.w_x,s.w_y,s.w_z}, .nz=s.nz};
    lec_cmd_t ac={.ts_us=c.ts_us, .thrust=c.thrust, .rates_cmd={c.rx,c.ry,c.rz}, .conf=c.conf, .mode=c.mode, .nic=c.nic};
    safe_cmd_t out;
    wcet_begin(&g);
    rta_step_simplex(&ctx,&ac,&x,&out);
    int over = wcet_end(&g);
    uint8_t pkt[128]; size_t n=0;
    if(out.switch_flag){ event_send_switch(&rl, 0x321, (uint8_t)ctx.st.last_mode, out.mode, out.reason_code, pkt, &n); fwrite(pkt,1,n,stdout); fputc('\n',stdout); }
    if(over){ size_t m=0; uint8_t u[4]={0,0,0,1}; uint16_t crc; m = pus_tm_pack(pkt,0x321,0,5,EV_HEALTH,u,4,&crc); fwrite(pkt,1,m,stdout); fputc('\n',stdout); }
  }
  fclose(f);
  return 0;
}
