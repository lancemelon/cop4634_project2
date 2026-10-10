#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>

unsigned long long COUNTER = 1;
std::mutex shared_mutex;

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

    // Launch T workers.
    for (int i = 0; i < T; i++) {
        threads.emplace_back(worker, N, tally, use_locks);
    }

    // Wait for every worker before printing the histogram.
    for (std::thread& thread : threads) {
        thread.join();
    }

    for (int i = 0; i <= max_stopping_time; i++) {
        std::cout << i << "," << tally[i] << "\n";
    }

    return 0;
}
