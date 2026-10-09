#include <iostream>
#include <string>
#include <vector>
#include <thread>

unsigned long long COUNTER = 1;

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

void worker(unsigned long long N, int tally[]) {
    while (COUNTER <= N) {
        unsigned long long n = COUNTER;

        COUNTER++;

        unsigned long long steps = collatz_stopping_time(n);

        tally[steps]++;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Only pass 2 arguments";
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

    // Stopping times tally
    int tally[max_stopping_time + 1] = {0};

    std::vector<std::thread> threads;

    // Launch T workers.
    for (int i = 0; i < T; i++) {
        threads.emplace_back(worker, N, tally);
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
