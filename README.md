# avionics-rta-safety-extract

Pluggable aerospace safety artifacts extracted from the larger RTA stack:

- **cFS apps (C)**:
  - `rta_monitor_app`: wraps `rta_step_simplex` as a cFE task; SB input, CCSDS TM output.
  - `receipt_verify_app`: online/offline verifier publishing TM stats.
- **Library**:
  - `wcet_guard_lib`: zero-allocation WCET guard for task cycles.
- **Reusable runtime**: O(1) Simplex monitor, CCSDS/PUS packer, time quality, monotonic clocks, NDJSON verifier.
- **F´ components**:
  - `RTA_NoAlloc`: fixed-memory RTA step.
  - `VerifyChain`: NDJSON chain verification.
- **Open MCT plugin**: minimal provider skeleton.

## Quick start (host)
```bash
make -j
./build/rta_monitor_app_host apps/input/state_cmd.csv | hexdump -C | head
./build/receipt_verify_app_host apps/input/receipts.ndjson | hexdump -C
