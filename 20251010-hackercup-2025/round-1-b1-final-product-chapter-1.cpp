// https://www.facebook.com/codingcompetitions/hacker-cup/2025/round-1/problems/B1

#include <cassert>
#include <deque>
#include <iostream>
#include <vector>


std::deque<int> prime_factors(int n) {
    std::deque<int> factors;
    for (int i = 2; i * i <= n; i++) {
        while (n % i == 0) {
            factors.push_back(i);
            n /= i;
        }
    }
    if (n > 1) {
        factors.push_back(n);
    }
    return factors;
}


std::vector<int> solve(int N, int A, int B) {
    std::vector<int> answers;
    std::deque<int> factors = prime_factors(B);              // Here factors are sorted
    // At N, max is A
    int coolness = 1;
    while (! factors.empty() && coolness * factors.front() <= A) {
        coolness *= factors.front(); factors.pop_front();
    }
    answers.push_back(coolness);
    while (answers.size() < N) {
        answers.push_back(1);
    }
    assert(coolness <= A);
    assert(answers.size() == N);
    // At 2 * N, it must equal B
    answers.push_back(B / coolness);                        // Finish the job by reaching B immediately
    coolness *= B / coolness;
    while (answers.size() < 2 * N) {
        answers.push_back(1);
    }
    assert(coolness == B);
    assert(answers.size() == 2 * N);
    return answers;
}


void test() {
    {
        std::vector<int> answers = solve(1, 2, 64);
        std::vector<int> expected = { 2, 32 };
        assert(answers == expected);
    }
    {
        std::vector<int> answers = solve(1, 32, 64);
        std::vector<int> expected = { 32, 2 };
        assert(answers == expected);
    }
    {
        std::vector<int> answers = solve(1, 64, 64);
        std::vector<int> expected = { 64, 1 };
        assert(answers == expected);
    }
    {
        std::vector<int> answers = solve(2, 32, 64);
        std::vector<int> expected = { 32, 1, 2, 1 };
        assert(answers == expected);
    }
    {
        std::vector<int> answers = solve(2, 64, 64);
        std::vector<int> expected = { 64, 1, 1, 1 };
        assert(answers == expected);
    }
    {
        std::vector<int> answers = solve(5, 63, 64);
        std::vector<int> expected = { 32, 1, 1, 1, 1, 2, 1, 1, 1, 1 };
        assert(answers == expected);
    }
}


int main() {
    test();
    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        int N; std::cin >> N;
        int A; std::cin >> A;
        int B; std::cin >> B;
        std::vector<int> answers = solve(N, A, B);
        std::cout << "Case #" << t + 1 << ":";
        for (int answer : answers) {
            std::cout << " " << answer;
        }
        std::cout << std::endl;
    }
    return 0;
}
