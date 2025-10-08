# FILE: tools/run_host_demo.sh
set -e
make -j
./build/rta_monitor_app_host apps/input/state_cmd.csv | hexdump -C | head
./build/receipt_verify_app_host apps/input/receipts.ndjson | hexdump -C
