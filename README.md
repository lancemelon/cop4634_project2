# COP4634 Project 2

Build with `make`, then run `./mt-collatz N T [-nolock]`.

The histogram goes to stdout as `stopping_time,frequency`; timing goes to
stderr as `N,T,seconds.nanoseconds`, with nine fractional digits. Timing
includes thread creation, histogram computation, and joining.

```sh
./mt-collatz 100000 4 > histogram.csv 2> timing.csv
```

Use `>>` and `2>>` to append. Default mode uses mutex protection; `-nolock`
intentionally demonstrates data races.

The complete usage notes and significant problems are in [README](README).
See [report.pdf](report.pdf) for the report draft, [report.md](report.md) for
its editable source, and [VALIDATION.md](VALIDATION.md) for checks and remaining
Linux tools. The five submission files are staged in `deliverables/`.
The draft is formatted to the supplied report template. Canvas submission is pending.
