# Sprint 5 experiment data

The experiment uses N = 3,000,000 with thread counts 1 through 8, both with
locks and with `-nolock`. Each configuration has 10 timed runs (160 total).
Programs run sequentially. Each round shuffles all 16 configurations using
seed 4634 to distribute changes in background load across configurations.
Pilot runs and a separate reference histogram run are excluded from averages.

Data files use tabs as separators:

- `average_times.txt`: average runtimes with and without locks by thread count.
- `histogram.txt`: stopping times 0 through 1000 and their frequencies from a
  locked single-thread run.
- `raw_times.txt`: all 160 measurements, execution order, histogram totals,
  and number of buckets differing from the reference.
- `timing_statistics.txt`: mean, sample standard deviation, minimum, maximum,
  and histogram mismatch counts for each configuration.
- `pilot_times.txt`: candidate N measurements supporting the selected range.
- `machine_and_method.txt`: machine specifications, compiler, build flags,
  timestamps, and measurement method for the Sprint 6 report.

The figures are `runtime_chart.png` and `histogram_chart.png`, with matching
PDF copies. Plot descriptions and analysis belong in the Sprint 6 report.

All locked histograms are checked against the reference, including every bucket
and a total of N. Unlocked T=1 runs must also match. Unlocked multithreaded runs
intentionally contain data races: their runtimes describe the observed program,
and do not measure speedups for a guaranteed correct computation.

In this experiment, all 80 locked runs and all 10 unlocked single-thread runs
matched the reference. All 70 unlocked multithreaded runs differed. The largest
nonzero stopping-time bucket is 559.

The locked four-thread measurements include one 14.682816-second observation;
the other nine range from 1.788375 to 1.832101 seconds. This observation is retained
in the required ten-run average (3.099322200 seconds). Its cause was not measured;
discuss the variability in the report rather than silently removing the point.