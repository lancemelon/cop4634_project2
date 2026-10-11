# cop4634_project2

Build with `make`, then run `./mt-collatz N T [-nolock]`.

The histogram is written to stdout as `stopping_time,frequency`. Timing is
written to stderr as `N,T,seconds.nanoseconds`, with nine fractional digits.
Timing includes thread creation, computation of the complete histogram, and
thread joining; it ends before printing the histogram.

Save the two outputs independently:

```sh
./mt-collatz 100000 4 > histogram.csv 2> timing.csv
```

Use `>>` and `2>>` to append instead of overwriting:

```sh
./mt-collatz 100000 4 >> histogram.csv 2>> timing.csv
```
