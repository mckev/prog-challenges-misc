// https://www.facebook.com/codingcompetitions/hacker-cup/2025/round-1/problems/B2

#include <cassert>
#include <iostream>
#include <vector>


std::vector<int> compute_primes(int max) {
    // Sieve of Eratosthenes
    std::vector<bool> is_primes(max + 1, true);
    is_primes.at(0) = false;
    is_primes.at(1) = false;
    for (int i = 2; i <= max; i++) {
        if (is_primes.at(i)) {
            for (int j = 2 * i; j <= max; j += i) {
                is_primes.at(j) = false;
            }
        }
    }
    std::vector<int> primes;
    for (int i = 0; i <= max; i++) {
        if (is_primes.at(i)) {
            primes.push_back(i);
        }
    }
    return primes;
}


std::vector<std::pair<long long, int>> find_factors(long long N, const std::vector<int>& primes) {
    std::vector<std::pair<long long, int>> answers;
    assert(primes.size() > 0);
    assert(N <= (long long) primes.at(primes.size() - 1) * primes.at(primes.size() - 1));
    for (int prime : primes) {
        int exp = 0;
        while (N % prime == 0) {
            exp++;
            N /= prime;
        }
        if (exp > 0) {
            answers.push_back({ prime, exp });
        }
    }
    if (N > 1) {
        // It must be a prime greater than our primes list
        answers.push_back({ N, 1 });
    }
    return answers;
}


class Combinatorics {
// To calculate C(n, r) mod p =  ( n! / (r!  (n - r)!) ) mod p
// Special case: n is very large, r is small.
private:
    static long long modpow(long long base, long long exp, long long mod) {
        // Compute (base^exp) % mod using binary exponentiation
        base %= mod;                                                // Ensure base is within mod
        long long result = 1;
        while (exp > 0) {
            if (exp & 1) {                                          // If exp is odd
                result = (result * base) % mod;
            }
            base = (base * base) % mod;
            exp = exp / 2;
        }
        return result;
    }

    int max_r;
    int mod;
    std::vector<long long> factorial, inverse_factorial;

public:
    Combinatorics(int max_r, int mod) : max_r(max_r), mod(mod), factorial(max_r + 1), inverse_factorial(max_r + 1) {
        factorial[0] = 1;
        for (int i = 1; i <= max_r; i++) factorial.at(i) = (factorial.at(i - 1) * i) % mod;
        // Using Fermat's Little Theorem if p is prime:  a^(-1) mod p = a^(p - 2) mod p
        inverse_factorial.at(max_r) = modpow(factorial.at(max_r), mod - 2, mod);
        for (int i = max_r - 1; i >= 0; i--) inverse_factorial.at(i) = (inverse_factorial.at(i + 1) * (i + 1)) % mod;
    }

    long long C(long long n, int r) const {
        assert (r <= max_r);
        // 0 <= r <= n, mod is a prime number
        assert (0 <= r); assert (r <= n);
        // n! / (r!  (n - r)!)  =  n * (n - 1) * (n - 2) * ... * (n - r + 1) / r!
        long long numerator = 1;
        for (int i = 0; i < r; i++) {
            numerator = (numerator * ((n - i) % mod)) % mod;
        }
        return numerator * inverse_factorial.at(r) % mod;
    }
};


class Solution {
private:
    std::vector<int> primes;
    Combinatorics combinatorics;
    std::vector<std::pair<long long, int>> factors;
    const long long mod = 1000000007;

public:
    Solution(const std::vector<int>& primes, const Combinatorics& combinatorics): primes(primes), combinatorics(combinatorics) {}

    long long dfs(long long N, long long A, int index, long long cur_products) {
        if (index == factors.size()) return 1;
        long long prime = factors.at(index).first;
        int exp = factors.at(index).second;
        long long products = cur_products;
        long long total_ways = 0;
        for (int k = 0; k <= exp; k++) {
            if (products > A) break;
            long long ways_first = combinatorics.C(N - 1 + k, k);
            long long ways_second = combinatorics.C(N - 1 + (exp - k), exp - k);
            total_ways = (total_ways + (ways_first * ways_second) % mod * dfs(N, A, index + 1, products)) % mod;
            products *= prime;
        }
        return total_ways;
    }

    long long solve(long long N, long long A, long long B) {
        factors = find_factors(B, primes);
        long long total_ways = dfs(N, A, 0, 1);
        return total_ways;
    }
};


void unit_test(const std::vector<int>& primes) {
    {
        // https://www.mathematical.com/primes0to1000k.html
        std::vector<std::pair<long long, int>> factors = find_factors((long long) 998831 * 999979, primes);
        std::vector<std::pair<long long, int>> expected = { {998831, 1}, {999979, 1} };
        assert(factors == expected);
    }
    {
        std::vector<std::pair<long long, int>> factors = find_factors((long long) 1, primes);
        std::vector<std::pair<long long, int>> expected = { };
        assert(factors == expected);
    }
    {
        std::vector<std::pair<long long, int>> factors = find_factors((long long) 56, primes);
        std::vector<std::pair<long long, int>> expected = { {2, 3}, {7, 1} };     // 2^3 * 7^1
        assert(factors == expected);
    }
    {
        std::vector<std::pair<long long, int>> factors = find_factors((long long) 999979 * 100000007, primes);
        std::vector<std::pair<long long, int>> expected = { {999979, 1}, {100000007, 1} };
        assert(factors == expected);
    }
    {
        const long long mod = 1000000007;
        Combinatorics combinatorics = Combinatorics(10000, mod);
        long long answer = combinatorics.C(20000, 10000);
        assert(answer == 703593270);
    }
}


void test(const std::vector<int>& primes, const Combinatorics& combinatorics) {
    {
        Solution solution(primes, combinatorics);
        long long answer = solution.solve(2, 10, 15);
        assert(answer == 12);
    }
    {
        Solution solution(primes, combinatorics);
        long long answer = solution.solve(2, 1000, 21);
        assert(answer == 16);
    }
    {
        Solution solution(primes, combinatorics);
        long long answer = solution.solve(50, 50000, 3628800);
        assert(answer == 229471373);
    }
}


int main() {
    const int MAX_PRIMES = 10000019;
    const long long mod = 1000000007;
    std::vector<int> primes = compute_primes(MAX_PRIMES);
    Combinatorics combinatorics = Combinatorics(50, mod);

    unit_test(primes);
    test(primes, combinatorics);

    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        long long N; std::cin >> N;
        long long A; std::cin >> A;
        long long B; std::cin >> B;
        Solution solution(primes, combinatorics);
        long long answer = solution.solve(N, A, B);
        std::cout << "Case #" << t + 1 << ": " << answer << std::endl;
    }
    return 0;
}
