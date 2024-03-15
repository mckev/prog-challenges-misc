// https://medium.com/@mckev/generate-prime-numbers-with-efficient-memory-using-priority-queue-200c6f704ac2

#include <iostream>
#include <queue>


struct Job {
    int64_t number;
    int64_t prime;
    bool operator<(const Job& other) const {
        return number > other.number;
    }
};


int main() {
    std::priority_queue<Job> jobs;
    jobs.push({2, 2});
    int64_t last_number = 2;
    while (true) {
        Job job = jobs.top(); jobs.pop();
        int64_t number = job.number;
        jobs.push({number + job.prime, job.prime});
        if (number == last_number + 2) {
            int64_t prime = last_number + 1;
            jobs.push({2 * prime, prime});
        }
        if (number % 1000000000 == 0) {
            std::cout << "Number of primes < " << number << ": " << jobs.size() << std::endl;
        }
        last_number = number;
    }
    return 0;
}
