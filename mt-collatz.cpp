#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <ctime>
#include <iomanip>

// Constant
const int MAX_STOPPING_TIME = 1000;

// Global variables 
unsigned long long COUNTER = 1;
unsigned long long N = 0;
int histogram[MAX_STOPPING_TIME + 1] = {0};

// Synchronization primitives
std::mutex counter_mutex;
std::mutex histogram_mutex;
bool use_locks = true;

/**
 * Computes the Collatz stopping time for a given positive integer n.
 */
unsigned long long collatz_stopping_time(unsigned long long n) {
    unsigned long long steps = 0;

    while (n > 1) {
        if (n % 2 == 0) {
            n /= 2;
        } else {
            n = 3 * n + 1;
        }
        steps++;
    }

    return steps;
}

/**
 * Worker thread function.
 * Repeatedly claims a number from COUNTER, computes its stopping time, and records the result in the global histogram.
 */
void collatz_worker() {
    while (true) {
        unsigned long long current_n;

        // Step 1: Claim the next number from COUNTER
        if (use_locks) {
            std::lock_guard<std::mutex> lock(counter_mutex);
            if (COUNTER > N) {
                break;
            }
            current_n = COUNTER;
            COUNTER++;
        } else {
            // Unprotected access (-nolock mode)
            if (COUNTER > N) {
                break;
            }
            current_n = COUNTER;
            COUNTER++;
        }

        // Step 2: Compute stopping time
        unsigned long long steps = collatz_stopping_time(current_n);

        // Step 3: Record stopping time in the global histogram
        if (steps <= MAX_STOPPING_TIME) {
            if (use_locks) {
                std::lock_guard<std::mutex> lock(histogram_mutex);
                histogram[steps]++;
            } else {
                histogram[steps]++;
            }
        }
    }
}

int main(int argc, char* argv[]) {
    // 1. Validate argument count (must be 3 or 4 arguments)
    if (argc < 3 || argc > 4) {
        std::cerr << "Usage: " << argv[0] << " <N> <T> [-nolock]\n";
        return 1;
    }

    // 2. Parse and validate N and T (use signed long long first to catch negative inputs)
    long long input_N = 0;
    int T = 0;
    try {
        input_N = std::stoll(argv[1]);
        T = std::stoi(argv[2]);
    } catch (const std::exception&) {
        std::cerr << "Error: N and T must be valid integers.\n";
        return 1;
    }

    if (input_N < 1 || T < 1) {
        std::cerr << "Error: Both N and T must be positive integers greater than 0.\n";
        return 1;
    }
    N = static_cast<unsigned long long>(input_N);

    // 3. Check for optional -nolock flag
    if (argc == 4) {
        std::string flag = argv[3];
        if (flag == "-nolock") {
            use_locks = false;
        } else {
            std::cerr << "Error: Unknown option '" << flag << "'. Expected '-nolock'.\n";
            return 1;
        }
    }

    // 4. Start the real time clock before spawning threads
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_REALTIME, &start_time);

    // 5. Create and launch T worker threads
    std::vector<std::thread> threads;
    threads.reserve(T);
    for (int i = 0; i < T; i++) {
        threads.emplace_back(collatz_worker);
    }

    // 6. Wait for all worker threads to join
    for (int i = 0; i < T; i++) {
        threads[i].join();
    }

    // 7. Stop the real-time clock immediately after threads finish
    clock_gettime(CLOCK_REALTIME, &end_time);

    // 8. Calculate elapsed time with nanosecond borrow logic
    long seconds = end_time.tv_sec - start_time.tv_sec;
    long nanoseconds = end_time.tv_nsec - start_time.tv_nsec;
    if (nanoseconds < 0) {
        seconds -= 1;
        nanoseconds += 1000000000L;
    }

    // 9. Print timing results to stderr: N,T,seconds.nanoseconds
    std::cerr << N << "," << T << "," << seconds << "."
              << std::setfill('0') << std::setw(9) << nanoseconds << "\n";

    // 10. Print the histogram to stdout: k,frequency
    for (int k = 0; k <= MAX_STOPPING_TIME; k++) {
        std::cout << k << "," << histogram[k] << "\n";
    }

    return 0;
}
