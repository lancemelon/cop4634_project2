#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <ctime>
#include <iomanip>
#include <cstdio>

unsigned long long COUNTER = 1;
std::mutex shared_mutex;

timespec elapsed_time(const timespec& start, const timespec& end) {
    timespec elapsed{end.tv_sec - start.tv_sec, end.tv_nsec - start.tv_nsec};

    // Borrow one second when the ending nanosecond field is smaller.
    if (elapsed.tv_nsec < 0) {
        elapsed.tv_sec--;
        elapsed.tv_nsec += 1000000000L;
    }

    return elapsed;
}

unsigned long long collatz_stopping_time(unsigned long long n) {
    unsigned long long steps = 0;

    while (n != 1) {
        if (n % 2 == 0) {
            n = n / 2;
        } 
        else {
            n = 3 * n + 1;
        }
        
        steps++;
    }

    return steps;
}

void worker(unsigned long long N, int tally[], bool use_locks) {
    while (true) {
        unsigned long long n;
        {
            // Protect the limit check, read, and increment as one operation.
            std::unique_lock<std::mutex> lock(shared_mutex, std::defer_lock);
            if (use_locks) {
                lock.lock();
            }
            if (COUNTER > N) {
                return;
            }
            n = COUNTER;
            COUNTER++;
        }

        // Each worker computes independently without holding the mutex.
        unsigned long long steps = collatz_stopping_time(n);

        {
            // Protect the shared bucket so simultaneous updates are not lost.
            std::unique_lock<std::mutex> lock(shared_mutex, std::defer_lock);
            if (use_locks) {
                lock.lock();
            }
            tally[steps]++;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3 && argc != 4) {
        std::cerr << "Usage: " << argv[0] << " N T [-nolock]\n";
        return 1;
    }

    if (argc == 4 && std::string(argv[3]) != "-nolock") {
        std::cerr << "The optional third argument must be -nolock.\n";
        return 1;
    }

    if (std::stoull(argv[1]) < 1) {
        std::cerr << "Please enter a positive integer greater than 0 for the first argument.\n";
        return 1;
    }

    if (std::stoi(argv[2]) < 1) {
        std::cerr << "Please enter a positive integer greater than 0 for the second argument.\n";
        return 1;
    }

    const int max_stopping_time = 1000;

    // Arguments
    unsigned long long N = std::stoull(argv[1]);
    int T = std::stoi(argv[2]);
    bool use_locks = (argc == 3);

    // Stopping times tally
    int tally[max_stopping_time + 1] = {0};

    std::vector<std::thread> threads;

    // Measure thread creation, computation, and joining using the real-time clock.
    timespec start{}, end{};
    if (clock_gettime(CLOCK_REALTIME, &start) != 0) {
        std::perror("clock_gettime (start)");
        return 1;
    }

    // Launch T workers.
    for (int i = 0; i < T; i++) {
        threads.emplace_back(worker, N, tally, use_locks);
    }

    // Wait for every worker before printing the histogram.
    for (std::thread& thread : threads) {
        thread.join();
    }

    if (clock_gettime(CLOCK_REALTIME, &end) != 0) {
        std::perror("clock_gettime (end)");
        return 1;
    }

    timespec elapsed = elapsed_time(start, end);
    // Nine fractional digits preserve nanosecond precision in the CSV record.
    std::cerr << N << "," << T << "," << elapsed.tv_sec << "."
              << std::setfill('0') << std::setw(9) << elapsed.tv_nsec << "\n";

    for (int i = 0; i <= max_stopping_time; i++) {
        std::cout << i << "," << tally[i] << "\n";
    }

    return 0;
}
