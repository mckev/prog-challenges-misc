// https://www.facebook.com/codingcompetitions/hacker-cup/2024/round-1/problems/B

#include <iostream>
#include <vector>


int main() {
    // Generate prime numbers using Sieve of Eratosthenes
    const int MAX_NUMBER = 10000000 + 1;
    std::vector<bool> is_primes(MAX_NUMBER, true);
    is_primes[0] = false;
    is_primes[1] = false;
    for (int i = 2; i < MAX_NUMBER; i++) {
        if (is_primes[i]) {
            for (int j = 2 * i; j < MAX_NUMBER; j += i) {
                is_primes[j] = false;
            }
        }
    }
    // Generate answers
    // Given A + B = C, in which A, B, C are prime numbers
    //   - B can only be even numbers, as subtraction of two prime numbers will always be an even number.
    //     Hence B = 2
    std::vector<int> answers(MAX_NUMBER);
    // Let's start with number 5 with answer 2
    int answer = 1;
    for (int i = 5; i < MAX_NUMBER; i++) {
        if (is_primes[i] && is_primes[i - 2]) {
            answer++;
        }
        answers[i] = answer;
    }

    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        int N; std::cin >> N;
        int answer = answers[N];
        std::cout << "Case #" << t + 1 << ": " << answer << std::endl;
    }
    return 0;
}
