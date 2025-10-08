// FILE: apps/common/cfs_stub/cfs_stub.c
#include "cfs_stub.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
int32_t CFE_EVS_SendEvent(uint16_t eid, uint16_t type, const char* fmt, ...) {
  (void)eid; (void)type; (void)fmt; return CFE_SUCCESS;
}
void CFE_ES_ExitApp(int32_t code){ (void)code; }
void CFE_ES_RunLoop(uint32_t* state){ if(state) *state = 0; }
