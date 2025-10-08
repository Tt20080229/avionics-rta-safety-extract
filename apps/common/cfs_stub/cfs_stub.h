// FILE: apps/common/cfs_stub/cfs_stub.h
#ifndef CFS_STUB_H
#define CFS_STUB_H
#include <stdint.h>
int32_t CFE_EVS_SendEvent(uint16_t eid, uint16_t type, const char* fmt, ...);
void CFE_ES_ExitApp(int32_t code);
void CFE_ES_RunLoop(uint32_t* state);
#define CFE_SUCCESS 0
#define CFE_SB_OK    0
#define CFE_EVS_EventType_INFORMATION 1
#define CFE_EVS_EventType_WARNING     2
#define CFE_EVS_EventType_ERROR       3
#endif
