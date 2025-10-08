```markdown
<!-- FILE: GUIDE.md -->
# GUIDE

## Build targets
- `build/rta_monitor_app_host`: host-mode cFS-like app harness.
- `build/receipt_verify_app_host`: NDJSON verifier app.
- `build/libwcet_guard.a`: reusable WCET guard.

## Replace stubs for flight
- `apps/common/cfs_stub`: swap to cFE includes and link against cFE Core.
- `reuse/runtime/c_a653/a653_port_if_apex.c`: bind to ARINC-653 APEX on target RTOS.
- Provide monotonic time sources by overriding weak symbols if needed.

## Data paths
- **RTA input**: CSV (`state_cmd.csv`) mock of SB messages → `rta_step_simplex`.
- **Telemetry**: CCSDS TM from `pus_tm_pack` with CRC-16-CCSDS (BE).

## Verification
- `reuse/libverify/verify.c` performs zero-allocation NDJSON line checks:
  - `head == SHA256({"decision_id","partition_id"})`
  - First line `parent_head` must be zero; subsequent lines must match prior `head`.
  - Optional strict body policy.

## WCET monitoring
- `libs/wcet_guard_lib`: begin/end cycle sampling with budget and overrun count.
