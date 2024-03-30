#include <array>
#include <cassert>
#include <iostream>
#include <queue>
#include <vector>


// Primes
std::vector<std::int64_t> compute_primes(int64_t max) {
    // Sieve of Eratosthenes
    std::vector<bool> is_primes(max, true);
    for (int64_t i = 2; i < max; i++) {
        if (is_primes[i]) {
            for (int64_t j = 2 * i; j < max; j += i) {
                is_primes[j] = false;
            }
        }
    }
    std::vector<std::int64_t> primes;
    for (int64_t i = 2; i < max; i++) {
        if (is_primes[i]) {
            primes.push_back(i);
        }
    }
    return primes;
}


// Jobs
struct Job {
    int64_t number;
    int64_t prime;
    bool operator<(const Job& other) const {
        return number > other.number;
    }
};
std::priority_queue<Job> jobs;


// Excluded Primes (for speed up)
const int EXCLUDED_PRIMES_NUM = 9;
const std::array<int, EXCLUDED_PRIMES_NUM> excluded_primes = {2, 3, 5, 7, 11, 13, 17, 19, 23};
constexpr int64_t compute_modulo(const std::array<int, EXCLUDED_PRIMES_NUM>& excluded_primes) {
    int64_t mod = 1;
    for (int prime : excluded_primes) {
        mod *= prime;
    }
    return mod;
}
constexpr int64_t MOD = compute_modulo(excluded_primes);
std::vector<bool> compute_mod_excluded_primes(const std::array<int, EXCLUDED_PRIMES_NUM>& excluded_primes) {
    std::vector<bool> mod_excluded_primes(MOD, true);
    mod_excluded_primes[0] = false;
    for (int prime : excluded_primes) {
        for (int64_t i = prime; i < MOD; i += prime) {
            mod_excluded_primes[i] = false;
        }
    }
    return mod_excluded_primes;
}
const std::vector<bool> mod_excluded_primes = compute_mod_excluded_primes(excluded_primes);


// Report
void report(int64_t number) {
    const int64_t REPORT_INTERVAL = 1000000000;
    static int64_t next_report = REPORT_INTERVAL;
    if (number > next_report) {
        int status = std::system("date");
        std::cout << "Number of primes < " << next_report << ": " << jobs.size() + excluded_primes.size() << std::endl;
        next_report += REPORT_INTERVAL;
    }
}


int main() {
    std::cout << "Modulo: " << MOD << std::endl;
    assert(mod_excluded_primes.size() == MOD);
    // Compute primes until MOD
    {
        std::vector<std::int64_t> primes = compute_primes(MOD);
        for (int64_t prime : primes) {
            if (prime <= excluded_primes.back()) continue;                                              // Skip excluded primes
            jobs.push({prime, prime});
        }
    }
    // Generate primes
    int status = std::system("date");
    std::cout << "Generating primes" << std::endl;
    for (int64_t base_number = 0; ; base_number += MOD) {
        std::vector<bool> is_primes = mod_excluded_primes;
        while (jobs.top().number < base_number + MOD) {
            Job job = jobs.top(); jobs.pop();
            int64_t number_mod = job.number - base_number;
            is_primes[number_mod] = false;
            // Skip mod excluded primes, as we know they are not prime
            while (number_mod < MOD && !is_primes[number_mod]) {
                number_mod += job.prime;
            }
            jobs.push({number_mod + base_number, job.prime});
        }
        for (int64_t i = 0; i < MOD; i++) {
            if (is_primes[i]) {
                int64_t prime = base_number + i;
                if (prime == 1) continue;                                                               // 1 is not a prime
                report(prime);
                jobs.push({2 * prime, prime});
            }
        }
    }
    return 0;
}
