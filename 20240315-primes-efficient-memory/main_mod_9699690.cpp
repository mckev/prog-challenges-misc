// https://medium.com/@mckev/generate-prime-numbers-with-efficient-memory-using-priority-queue-200c6f704ac2

#include <algorithm>
#include <deque>
#include <iostream>
#include <queue>
#include <unordered_set>
#include <vector>


// DEBUG
// #define DEBUG

// Jobs
struct Job {
    int64_t number;
    int64_t prime;
    bool operator<(const Job& other) const {
        return number > other.number;
    }
};
std::priority_queue<Job> jobs;

// Primes
std::vector<std::int64_t> list_primes(int64_t max) {
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
#ifdef DEBUG
std::vector<std::int64_t> debug_primes;
#endif

// Skip primes modulo
const std::vector<int64_t> skipped_primes_modulo = {2, 3, 5, 7, 11, 13, 17, 19};

// Reporting
void report(int64_t number) {
    const int64_t REPORT_INTERVAL = 1000000000;
    static int64_t next_report = REPORT_INTERVAL;
    if (number > next_report) {
        int status = std::system("date");
        std::cout << "Number of primes < " << next_report << ": " << jobs.size() + skipped_primes_modulo.size() << std::endl;
        #ifdef DEBUG
        {
            std::vector<std::int64_t> primes_expected = list_primes(next_report);
            std::unordered_set<std::int64_t> debug_primes_set(debug_primes.begin(), debug_primes.end());
            for (int64_t prime : primes_expected) {
                bool is_found = debug_primes_set.find(prime) != debug_primes_set.end();
                if (! is_found) {
                    std::cout << "  Missing: " << prime << std::endl;
                }
            }
            std::unordered_set<std::int64_t> primes_expected_set(primes_expected.begin(), primes_expected.end());
            for (int64_t prime : debug_primes) {
                bool is_found = primes_expected_set.find(prime) != primes_expected_set.end();
                if (! is_found) {
                    std::cout << "  Extra: " << prime << std::endl;
                }
            }
        }
        #endif
        next_report += REPORT_INTERVAL;
    }
}


int main() {
    int status = std::system("date");
    // Calculate modulo
    int64_t modulo = 1;
    for (int64_t skipped_prime : skipped_primes_modulo) {
        modulo *= skipped_prime;
    }
    // Include into candidates: All primes < modulo
    std::unordered_set<int64_t> primes_candidates_set;
    {
        std::vector<std::int64_t> primes = list_primes(modulo);
        primes_candidates_set.insert(primes.begin(), primes.end());
    }
    // Exclude from candidates: skipped_primes_modulo
    for (int64_t skipped_prime : skipped_primes_modulo) {
        primes_candidates_set.erase(skipped_prime);
    }
    // Initialize jobs
    for (int64_t prime : primes_candidates_set) {
        jobs.push({prime, prime});
        #ifdef DEBUG
        debug_primes.push_back(prime);
        #endif
    }
    // Include into candidates: Multiplication of primes
    {
        std::vector<int64_t> additionals;
        std::vector<int64_t> primes_candidates(primes_candidates_set.begin(), primes_candidates_set.end());
        std::sort(primes_candidates.begin(), primes_candidates.end());
        struct MultiPrime {
            std::vector<int64_t> primes;
            int64_t product;
        };
        std::deque<MultiPrime> multiprimes;
        for (int64_t prime : primes_candidates) {
            multiprimes.push_back({{prime}, prime});
        }
        while (! multiprimes.empty()) {
            // BFS
            MultiPrime multiprime = multiprimes.front(); multiprimes.pop_front();
            for (int64_t prime : primes_candidates) {
                MultiPrime multiprime_new = multiprime;
                multiprime_new.primes.push_back(prime);
                multiprime_new.product *= prime;
                if (multiprime_new.product >= modulo) break;
                multiprimes.push_back(multiprime_new);
                additionals.push_back(multiprime_new.product);
            }
        }
        primes_candidates_set.insert(additionals.begin(), additionals.end());
    }
    // Include into candidates: 1
    primes_candidates_set.insert(1);
    // Sort
    std::vector<int64_t> primes_candidates(primes_candidates_set.begin(), primes_candidates_set.end());
    std::sort(primes_candidates.begin(), primes_candidates.end());
    // Use priority queue to iterate non-primes
    std::vector<bool> is_primes(modulo);
    for (int64_t base_number = 0; ; base_number += modulo) {
        for (int64_t i : primes_candidates) {
            is_primes[i] = true;
        }
        while (jobs.top().number < base_number + modulo) {
            Job job = jobs.top(); jobs.pop();
            int64_t number_mod = job.number - base_number;
            is_primes[number_mod] = false;
            int64_t next_addition = job.prime;
            while (number_mod + next_addition < modulo) {
                bool is_found = primes_candidates_set.find(number_mod + next_addition) != primes_candidates_set.end();
                if (is_found) break;
                next_addition += job.prime;
            }
            jobs.push({job.number + next_addition, job.prime});
        }
        for (int64_t i : primes_candidates) {                                                           // We need in sorted form so we can report correctly
            if (is_primes[i]) {
                int64_t prime = base_number + i;
                if (prime == 1) continue;                                                               // 1 is not a prime
                report(prime);
                jobs.push({2 * prime, prime});
                #ifdef DEBUG
                debug_primes.push_back(prime);
                #endif
            }
        }
    }
    return 0;
}
