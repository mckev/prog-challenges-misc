// https://medium.com/@mckev/generate-prime-numbers-with-efficient-memory-using-priority-queue-200c6f704ac2

#include <array>
#include <iostream>
#include <queue>


struct Job {
    int64_t number;
    int64_t prime;
    bool operator<(const Job& other) const {
        return number > other.number;
    }
};

std::priority_queue<Job> jobs;

void report(int64_t number) {
    const int64_t REPORT_INTERVAL = 1000000000;
    static int64_t next_report = REPORT_INTERVAL;
    if (number > next_report) {
        std::cout << "Number of primes < " << next_report << ": " << jobs.size() + 3 << std::endl;      // We add 3 because the first three primes 2, 3 and 5 were not included
        next_report += REPORT_INTERVAL;
    }
}

int main() {
    jobs.push({7, 7});
    jobs.push({11, 11});
    jobs.push({13, 13});
    jobs.push({17, 17});
    jobs.push({19, 19});
    jobs.push({23, 23});
    jobs.push({29, 29});
    for (int64_t base_number = 0; ; base_number += 30) {
        std::vector<bool> is_primes(30, true);
        while (jobs.top().number < base_number + 30) {
            Job job = jobs.top(); jobs.pop();
            is_primes[job.number - base_number] = false;
            jobs.push({job.number + job.prime, job.prime});
        }
        for (int i : std::array<int, 8>{1, 7, 11, 13, 17, 19, 23, 29}) {
            if (is_primes[i]) {
                int64_t number = base_number + i;
                if (number == 1) continue;                                                              // 1 is not a prime
                report(number);
                // std::cout << "Prime: " << number << std::endl;
                jobs.push({2 * number, number});
            }
        }
    }
    return 0;
}
