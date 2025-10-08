# FILE: Makefile
CC      ?= gcc
CXX     ?= g++
ARFLAGS := crD
CFLAGS  ?= -std=c99 -O2 -g -Wall -Wextra -Werror -Wconversion -Wshadow \
           -Wstrict-prototypes -Wmissing-prototypes -Wredundant-decls \
           -fno-common -fvisibility=hidden -ffile-prefix-map=$(PWD)=/src
CXXFLAGS?= -std=c++17 -O2 -g -Wall -Wextra -Werror -fno-exceptions -fno-rtti \
           -fvisibility=hidden -ffile-prefix-map=$(PWD)=/src
LDFLAGS ?=
DEFS    := -DUSE_CFS_STUB=1 -DHOST_SIM=1
INCS    := -Iapps/common -Iapps/common/cfs_stub -Iapps/rta_monitor_app -Iapps/receipt_verify_app \
           -Ilibs/wcet_guard_lib/include -Ireuse/runtime/c_a653 -Ireuse/libverify \
           -Ifprime/components/RTA_NoAlloc -Ifprime/components/VerifyChain
REUSE_C := \
  reuse/runtime/c_a653/endian.h \
  reuse/runtime/c_a653/stl_rt.h reuse/runtime/c_a653/stl_rt.c \
  reuse/runtime/c_a653/rta_types.h reuse/runtime/c_a653/rta_envelope.h reuse/runtime/c_a653/rta_envelope.c \
  reuse/runtime/c_a653/rta_monitor.h  reuse/runtime/c_a653/rta_monitor.c \
  reuse/runtime/c_a653/ccsds_pus.h reuse/runtime/c_a653/ccsds_pus.c \
  reuse/runtime/c_a653/pus_events.h reuse/runtime/c_a653/pus_events.c \
  reuse/runtime/c_a653/mc_mon.h reuse/runtime/c_a653/mc_mon.c \
  reuse/runtime/c_a653/time_quality.h reuse/runtime/c_a653/time_quality.c \
  reuse/runtime/c_a653/a653_port_if.h reuse/runtime/c_a653/a653_port_if_apex.c \
  reuse/libverify/sha256.h reuse/libverify/sha256.c \
  reuse/libverify/verify.h reuse/libverify/verify.c
APP_RTA := apps/rta_monitor_app/src/rta_monitor_app.c \
           apps/rta_monitor_app/rta_monitor_msgs.h \
           apps/common/pack_et_tlm.h apps/common/pack_et_tlm.c \
           apps/common/cfs_stub/cfs_stub.h apps/common/cfs_stub/cfs_stub.c
APP_VFY := apps/receipt_verify_app/src/receipt_verify_app.c \
           apps/receipt_verify_app/receipt_verify_msgs.h \
           apps/common/pack_et_tlm.h apps/common/pack_et_tlm.c \
           apps/common/cfs_stub/cfs_stub.h apps/common/cfs_stub/cfs_stub.c
WCETLIB := libs/wcet_guard_lib/include/wcet_guard.h libs/wcet_guard_lib/src/wcet_guard.c

all: build build/libwcet_guard.a build/rta_monitor_app_host build/receipt_verify_app_host fprime-compile

build:
	mkdir -p build

build/libwcet_guard.a: $(WCETLIB)
	$(CC) $(CFLAGS) $(DEFS) $(INCS) -c -o build/wcet_guard.o libs/wcet_guard_lib/src/wcet_guard.c
	ar $(ARFLAGS) $@ build/wcet_guard.o

build/rta_monitor_app_host: $(APP_RTA) $(REUSE_C) build/libwcet_guard.a
	$(CC) $(CFLAGS) $(DEFS) $(INCS) -o $@ \
	  apps/rta_monitor_app/src/rta_monitor_app.c \
	  apps/common/pack_et_tlm.c apps/common/cfs_stub/cfs_stub.c \
	  reuse/runtime/c_a653/stl_rt.c reuse/runtime/c_a653/rta_envelope.c reuse/runtime/c_a653/rta_monitor.c \
	  reuse/runtime/c_a653/ccsds_pus.c reuse/runtime/c_a653/pus_events.c \
	  reuse/runtime/c_a653/mc_mon.c reuse/runtime/c_a653/time_quality.c reuse/runtime/c_a653/a653_port_if_apex.c \
	  reuse/libverify/sha256.c \
	  build/libwcet_guard.a $(LDFLAGS)

build/receipt_verify_app_host: $(APP_VFY) $(REUSE_C)
	$(CC) $(CFLAGS) $(DEFS) $(INCS) -o $@ \
	  apps/receipt_verify_app/src/receipt_verify_app.c \
	  apps/common/pack_et_tlm.c apps/common/cfs_stub/cfs_stub.c \
	  reuse/libverify/sha256.c reuse/libverify/verify.c \
	  reuse/runtime/c_a653/ccsds_pus.c

fprime-compile:
	$(CXX) $(CXXFLAGS) $(INCS) -c fprime/components/RTA_NoAlloc/RTA_NoAllocComponent.cpp -o build/rta_noalloc.o
	$(CXX) $(CXXFLAGS) $(INCS) -c fprime/components/VerifyChain/VerifyChainComponent.cpp -o build/verifychain.o

test: all
	./build/rta_monitor_app_host apps/input/state_cmd.csv | head -n 5
	./build/receipt_verify_app_host apps/input/receipts.ndjson

clean:
	rm -rf build

.PHONY: all test clean fprime-compile
