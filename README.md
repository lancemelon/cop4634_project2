Lance Penaflor and Jay McIntosh
COP 4634: Systems & Networks
Project 2: Multi-Threaded Collatz Stopping Time Generator

COP 4634 Multi-Threaded Collatz Project
Setup:
1. Download and unzip the file from submission, or clone the repository to the department's public Linux server.
2. The directory should contain these project files:
- mt-collatz.cpp - The main source code handling the thread pool, Collatz sequence generation, and synchronization.
- Makefile - Script for compiling and cleaning the project.
- README - This file.
- report.pdf - The project report analyzing thread performance and lock contention.
- collatz_data.xlsx - The spreadsheet containing the visualized histogram and runtime charts.

Compiling:
3. The project includes a Makefile for easy compilation. Open a terminal in the project directory and run:
make

This should create an executable file named 'mt-collatz'.

Running the program:
4. After compiling, run the program by executing:
./mt-collatz

   To run the program without mutex locks (to observe race conditions), run:
     ./mt-collatz <N> <T> -nolock
Parallel Execution
The first aspect of the project involves parallelizing a computational workload. The program evaluates the Collatz stopping time for every integer from 1 to N. It distributes this work dynamically across T worker threads using the standard C++  library. Each thread claims the next available number from a shared global COUNTER, computes its stopping time independently, and records the result in a global histogram array.

Synchronization & Timing
The second aspect extends the program to handle memory race conditions and measure performance.
- Mutex Locking: Shared resources (the global COUNTER and the histogram array) are protected using std::mutex to prevent data races and guarantee 100% accurate counts.
- Unsynchronized Mode: An optional -nolock command-line flag disables locking to demonstrate data corruption when multiple threads read/write to shared memory concurrently.
- Timing Instrumentation: The program measures the total wall-clock execution time of the threads using clock_gettime(CLOCK_REALTIME) and formats the output down to the nanosecond.

Output & Interaction:
5. Once running, the program will compute the stopping times and immediately output the results without further user input.
- Histogram data is printed to standard output (stdout) in the format: k,frequency
- Timing data is printed to standard error (stderr) in the format: N,T,seconds.nanoseconds

   You can test it and redirect streams with standard Linux commands:
     - ./mt-collatz 1000 4 (Basic execution to console)
     - ./mt-collatz 5000000 4 -nolock (Unsynchronized execution)
     - ./mt-collatz 5000000 4 > histogram.csv 2> timing.csv (Redirects stdout and stderr to separate files)
***Notes:
- Environment: Tested on the department's public Linux servers (cs-ssh.argo.uwf.edu) and compiled using g++ with -std=c++17 and -pthread.
- Data Types: Intermediate Collatz calculations utilize unsigned long long (64-bit integers) to prevent 32-bit integer overflows during the 3n + 1 mathematical steps.
- Nanosecond Timing: The elapsed time calculation implements borrow-logic to correctly handle nanosecond wrap-around across second boundaries, and zero-pads the output to ensure accurate decimal representation.