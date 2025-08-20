# CPU Core Optimization Simulation

This project demonstrates how to measure per-core CPU load on Windows and dynamically assign a running thread to the least busy core using **C** and the Windows API.

## What It Does
- Detects the number of logical processors (CPU cores + SMT threads)
- Monitors real-time per-core usage via **PDH performance counters**
- Periodically migrates the current thread to the least loaded core using `SetThreadAffinityMask`
- Performs a simple computation loop (`sum += i * 0.000001;`) to simulate workload
- Measures total execution time

## Why This Matters
Modern operating systems already schedule threads efficiently, but understanding:
- **Thread affinity** (binding threads to specific cores)
- **Core load measurement**
- **Scheduler behavior**

...is critical for low-level performance tuning, CPU engineering, and systems programming.

## Build Instructions
- Platform: **Windows (x64)**
- Compiler: Any MSVC-compatible compiler
- Link with: `pdh.lib`
- Example:
```powerhsell
cl /O2 Optimizer.c /link pdh.lib
```

## Example Output
```powershell
[+] Number of Logical Processors : 16
[+] Thread Priority Set to Highest [+]
Iteration 0: moved to core 0 (load 0.00%)
Iteration 10000000: moved to core 1 (load 0.00%)
Iteration 20000000: moved to core 8 (load 0.00%)
Iteration 30000000: moved to core 0 (load 0.00%)
Iteration 40000000: moved to core 1 (load 2.04%)
Iteration 50000000: moved to core 4 (load 0.00%)
Iteration 60000000: moved to core 2 (load 0.00%)
Iteration 70000000: moved to core 0 (load 0.00%)
Iteration 80000000: moved to core 0 (load 0.00%)
Iteration 90000000: moved to core 2 (load 2.57%)
...
Computation done, sum = 19999999900.000000
[+] Execution Time is 3.441 ...
```

## Notes

The workload (sum += i * 0.000001;) can be replaced with any computation to test different performance scenarios.

Removing affinity switching and performance counter checks will show pure loop execution speed.

This is a learning project — not a replacement for the Windows scheduler.

