// https://www.facebook.com/codingcompetitions/hacker-cup/2025/round-1/problems/C
//
// To run:
//    $ g++ -Ofast -o main.out main.cpp
//    $ cat narrowing_down_input.txt | time ./main.out > narrowing_down_output.txt

#include <algorithm>
#include <cassert>
#include <execution>
#include <iostream>
#include <random>
#include <thread>
#include <vector>


long long solve_bruteforce(const std::vector<long long>& A) {
    long long answer = 0;
    for (int l = 0; l < A.size(); l++) {
        for (int r = l; r < A.size(); r++) {
            // Process subarray [l, r]
            std::vector<long long> subarray(A.begin() + l, A.begin() + r + 1);
            long long counter = 0;
            for (int i = 0; i < subarray.size() - 1; i++) {     // Notice here we skip the last element. However it can be changed by previous operation.
                long long element = subarray.at(i);
                if (element == 0) {
                    continue;
                } else {
                    subarray.at(i + 1) ^= element;
                    subarray.at(i) = 0;                         // Same as: subarray.at(i) ^= element;
                    counter++;
                }
            }
            bool is_all_zero = std::all_of(subarray.begin(), subarray.end(), [](long long x) { return x == 0; });
            if (! is_all_zero) {
                counter = subarray.size();
            }
            answer += counter;
        }
    }
    return answer;
}


long long solve_efficient(const std::vector<long long>& A) {
    long long answer = 0;
    for (int l = 0; l < A.size(); l++) {
        long long x = 0;
        long long counter = 0;
        for (int r = l; r < A.size(); r++) {
            // Process subarray [l, r]
            x ^= A.at(r);
            if (x != 0) {
                answer += r - l + 1;
                counter++;
            } else {
                answer += counter;
            }
        }
    }
    return answer;
}


template <typename T, typename UnaryFunc>
void parallel_transform_vector(const std::vector<T>& input, std::vector<T>& output, UnaryFunc func) {
    const size_t n = input.size();
    if (n == 0) return;

    const unsigned num_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    auto worker = [&](size_t start, size_t end) {
        for (size_t i = start; i < end; i++) {
            output.at(i) = func(input.at(i));
        }
    };

    // size_t chunk_size = (n + num_threads - 1) / num_threads;
    size_t chunk_size = num_threads;
    size_t start = 0;
    for (unsigned t = 0; /* t < num_threads && */ start < n; t++) {
        size_t end = std::min(start + chunk_size, n);
        threads.push_back(std::thread(worker, start, end));
        start = end;
    }

    assert(threads.size() < 200000);                            // See: ulimit -u
    for (auto& th : threads)
        th.join();
}


long long solve_efficient_parallel(const std::vector<long long>& A) {
    // Populate answers[]
    std::vector<long long> answers(A.size());

    // Generate indexes
    std::vector<long long> indexes(A.size());
    std::iota(indexes.begin(), indexes.end(), 0 /* initial value */);
    // Optimization (optional): Randomize it for more efficiency
    // std::random_device rd;
    // std::mt19937 mt(rd());
    // std::shuffle(indexes.begin(), indexes.end(), mt);

    // Process A[0, n), A[1, n), A[2, n), ..., in parallel
    parallel_transform_vector(
        indexes,
        answers,
        [&A](int index) {
        long long answer = 0;
        long long x = 0;
        long long counter = 0;
        for (int r = index; r < A.size(); r++) {
            // Process subarray [l, r]
            x ^= A.at(r);
            if (x != 0) {
                answer += r - index + 1;
                counter++;
            } else {
                answer += counter;
            }
        }
        return answer;                                          // this populates answers[]
    });

    long long total_answers = 0;
    for (long long answer : answers) {
        assert(answer >= 0);
        total_answers += answer;
    }
    return total_answers;
}


void test() {
    {
        std::vector<long long> A = { 0, 0 };
        long long answer_bruteforce = solve_bruteforce(A);
        assert(answer_bruteforce == 0);
        long long answer_efficient = solve_efficient(A);
        assert(answer_efficient == answer_bruteforce);
    }
    {
        std::vector<long long> A = { 1, 1, 1 };
        long long answer_bruteforce = solve_bruteforce(A);
        assert(answer_bruteforce == 8);
        long long answer_efficient = solve_efficient(A);
        assert(answer_efficient == answer_bruteforce);
    }
    {
        std::vector<long long> A = { 1, 2, 3 };
        long long answer_bruteforce = solve_bruteforce(A);
        assert(answer_bruteforce == 9);
        long long answer_efficient = solve_efficient(A);
        assert(answer_efficient == answer_bruteforce);
    }
    {
        std::vector<long long> A = { 0, 1, 0, 2, 0, 3, 0 };
        long long answer_bruteforce = solve_bruteforce(A);
        assert(answer_bruteforce == 72);
        long long answer_efficient = solve_efficient(A);
        assert(answer_efficient == answer_bruteforce);
    }
    {
        // Test case 91
        std::vector<long long> A = { 0, 8, 9, 2, 6, 9, 6, 3, 12, 2, 9, 1, 14, 1, 11, 4, 12, 8, 5, 7, 11, 12, 6, 9, 4, 8, 1, 15, 4, 8, 11, 0, 11, 15, 1, 8, 13, 1, 0, 6, 4, 9, 11, 13, 11, 4, 9, 7, 4, 5, 5, 13, 4, 15, 12, 0, 2, 14, 2, 4, 11, 14, 13, 15 };
        long long answer_efficient = solve_efficient(A);
        assert(answer_efficient == 45489);
    }
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<> rnd(0, 1);             // [0, 1)
        for (int t = 0; t < 1000; t++) {
            int N = int(rnd(mt) * 100) + 1;
            std::vector<long long> A;
            for (int n = 0; n < N; n++) {
                int a = int(rnd(mt) * 16);                      // [0, 15]
                A.push_back(a);
            }
            long long answer_bruteforce = solve_bruteforce(A);
            long long answer_efficient = solve_efficient_parallel(A);
            assert(answer_efficient == answer_bruteforce);
        }
    }
}


int main() {
    test();
    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        std::vector<long long> A;
        int N; std::cin >> N;
        for (int n = 0; n < N; n++) {
            long long a; std::cin >> a;
            A.push_back(a);
        }
        long long answer_efficient = solve_efficient_parallel(A);
        std::cout << "Case #" << t + 1 << ": " << answer_efficient << std::endl;
    }
    return 0;
}
