# Project 2: Multi-Threaded Collatz Stopping Time Generator — Sprint Schedule

Outline only — no implementation. Each task names what needs to exist; you look up the how.

## Sprint 1 — Core single-threaded logic
- [X] Set up repo structure: `mt-collatz.cpp`, `Makefile`, `README`
- [X] Write a `Makefile` with `-g -Wall`, a default build target, and a `clean` rule
- [X] Implement argument parsing: `N` (range), `T` (thread count), optional `-nolock` flag
  - Validate argument count/order; decide what happens on bad input
- [X] Implement `collatz_stopping_time(n)`: applies `f(n) = n/2` (even) or `3n+1` (odd), counts steps until reaching 1
- [X] Implement a global histogram array/structure sized for stopping times 0–1000, zero-initialized before any computation
- [X] Single-threaded version: loop `n` from 1 to `N`, compute stopping time, increment histogram bucket
- [X] Print histogram to stdout in `k,frequency` format (k = 0..1000)
- [X] Manual sanity check: n=1 → stopping time 0 → `0,1`

## Sprint 2 — Threading
- [X] Decide on the global shared `COUNTER` variable (starts at 1, ends at N) representing "next number to claim"
- [X] Implement a worker thread function: loop { claim next COUNTER value, compute stopping time, record in histogram, repeat until COUNTER > N }
- [X] Launch `T` threads using the C++ `<thread>` library with the worker function
- [X] Join all worker threads on the main thread before proceeding to output
- [X] Verify correctness with T=1 (should match Sprint 1 single-threaded output exactly)

## Sprint 3 — Synchronization (race condition handling)
- [X] Identify the two shared-resource race conditions: (1) reading/incrementing `COUNTER`, (2) incrementing a histogram bucket
- [X] Introduce a `<mutex>` lock to protect `COUNTER` increment (and histogram update, if not using an atomic/lock-free approach)
- [X] Implement `-nolock` mode: same worker logic but skip locking entirely, so races are reproducible
- [X] Test with T=4 or T=8 and verify: with locks, total counted numbers == N, no duplicate/skipped values; without locks, observe inconsistent/incorrect totals across runs
  - Verified N=100000: five locked runs each with T=1, 4, and 8 matched an independent single-threaded reference histogram exactly, with totals of 100000.
  - Five unlocked runs each with T=4 and T=8 all produced incorrect histograms. Totals ranged from 116862 to 119400 (T=4) and 151631 to 163674 (T=8).

## Sprint 4 — Timing instrumentation
- [X] Add `clock_gettime()` (real-time clock) calls bracketing thread creation through thread join + histogram completion
- [X] Handle nanosecond "borrow" when subtracting start/end times (seconds and nanoseconds don't subtract cleanly)
- [X] Print elapsed time to stderr in `N,T,seconds.nanoseconds` format
- [X] Confirm stdout (histogram) and stderr (timing) can be redirected independently (`2>`, `>`, `2>>`, `>>`)
  - Verified subtraction with and without borrowing and with zero duration; timing uses nine fractional digits in locked and unlocked modes.
  - Locked histograms for N=100000 and T=1, 4, and 8 matched an independent reference. Separate overwrite and append redirection passed.

## Sprint 5 — Experiments & data collection
- [ ] Choose a large N that produces measurable runtime (test a few candidates)
- [ ] Script or manually run T = 1 through 8, with locks, 10 runs each, record times, compute averages
- [ ] Repeat the same T=1..8 sweep with `-nolock`, 10 runs each, average
- [ ] Record machine specs used for testing (CPU model, core/thread count, RAM) — needed for the report
- [ ] Export histogram data (for one chosen N) into Excel
- [ ] Build Excel chart: threads vs. average runtime, with and without locks (same chart or paired charts)
- [ ] Build Excel chart/visual of the Collatz stopping-time histogram

## Sprint 6 — Report & polish
- [ ] Write `report.pdf` using the provided template: experiment description + machine specs, histogram visualization, threads-vs-runtime graph (locked vs. unlocked), analysis of parallel speedup, analysis of lock overhead, conclusions
- [ ] Write `README`: describe any significant problems encountered (or state none)
- [ ] Code pass: add comments, check structure/readability against grading rubric
- [ ] Clean build check: compile fresh with no warnings/errors (`-g -Wall`)
- [ ] Run under valgrind to check for leaks/race-related memory issues
- [ ] Run under gdb at least once to confirm debuggability
- [ ] Final dry run of grader's test questions: range correctness, multiple threads used, threading affects time, locks prevent races, locks affect time
- [ ] Package deliverables: `mt-collatz.cpp`, `report.pdf`, histogram+chart `.xlsx`, `README`, `Makefile`
- [ ] Upload to Canvas dropbox ahead of deadline

## Notes / things to look up as you implement
- C++ `<thread>` and `<mutex>` usage basics
- `clock_gettime(CLOCK_REALTIME, ...)` struct `timespec` and the seconds/nanoseconds borrow logic
- Work-stealing/claim pattern for `COUNTER` (lock vs. `std::atomic<int>` — assignment wants a mutex specifically)
- Collatz numbers can exceed 32-bit range mid-sequence even for moderate `n` — mind integer overflow/type choice
