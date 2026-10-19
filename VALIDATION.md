# Sprint 6 validation — October 16, 2026

## Completed locally

- Clean `make clean` and `make`: Apple Clang, C++17, `-g -Wall -Wextra`,
  explicit `-pthread`; no compiler warnings or errors.
- `python3 tools/verify.py`: 78 complete histograms match an independent
  Python implementation. N values: 1, 2, 10, 27, 1000, 100000. For each N,
  locked T=1,2,4,8 were run three times and unlocked T=1 once. Every run
  checks all 1001 bucket indices, frequencies, and timing record format.
- All 13 invalid invocations exited nonzero, printed an error to stderr,
  and produced no histogram. Includes missing arguments, negative/zero
  values, malformed decimals, unknown flags, and conversion/range overflow.
- Ten unlocked runs at N=100000, T=4 or 8: all ten histograms differed
  from the reference; totals ranged from 114691 to 153351. This is an
  observation, not a guarantee that a race will reproduce on every run.
- Final full-range check: N=3000000, T=4 matched all 1001 archived reference
  buckets. Diagnostic timing was 1.827369000 seconds; this run is excluded
  from the October 12 experimental averages.
- AddressSanitizer and UndefinedBehaviorSanitizer build:
  `clang++ -std=c++17 -g -Wall -Wextra -pthread -fsanitize=address,undefined -fno-omit-frame-pointer mt-collatz.cpp -o /tmp/mt-collatz-sprint6-asan`.
  Running the same verification script against this build passed without
  sanitizer diagnostics. The unlocked mode's deliberate races are not
  detected by these sanitizers. No Valgrind leak result is implied.
- LLDB: set a breakpoint in `worker`, ran `100000 4`, inspected `thread list`,
  disabled the breakpoint, and continued. Observed main thread and four
  worker threads, source lines and parameter values, then normal exit (0).
  Debugger execution required sandbox permission to launch the process.
- Audited all 160 archived measurements: each configuration has ten runs;
  every mean agrees with `average_times.txt` within 1e-9 seconds. All 80
  locked and ten unlocked single-thread histograms match; all 70 unlocked
  multithreaded histograms differ. The four-thread outlier remains included.
- Workbook verified cell by cell against 8 average rows, 1001 histogram rows,
  and 160 raw measurement rows. It contains native Excel runtime and
  histogram charts. Full histogram data is retained.
- Six-page PDF generated from `report.md`; text extraction confirms required
  sections and all five figures. Representative pages visually checked for
  readable plots, captions, tables, and margins.

The grader's range, thread use, timing effect, race prevention, and lock
performance questions are addressed by these correctness checks, LLDB thread
inspection, and the archived ten-run timing comparisons. Timings are specific
to the recorded machine and build.

## Pending checks and submission

The current host is macOS. GDB and Valgrind are not installed, and Docker's
daemon is not running. Department Linux execution has not been verified.
LLDB and sanitizers provide additional evidence but do not complete the
explicit GDB/Valgrind sprint items. On the department Linux server:

```sh
make clean
make
python3 tools/verify.py
valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 ./mt-collatz 100000 4 > /tmp/collatz-vg.csv
# In GDB: break worker, run 100000 4, info threads, disable 1, continue, quit.
gdb ./mt-collatz
```

Optional race analysis can use Valgrind Helgrind in locked mode. Ordinary
Memcheck does not establish the absence of data races.

The report draft follows Project 2 Midpoint Report Template.docx. Canvas upload must
use the instructor's submission rules; no upload has been performed. Five
individual submission files are staged in `deliverables/`, with supplemental
figures and staging notes alongside them.
