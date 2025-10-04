// https://codeforces.com/contest/57/problem/C

#include <cassert>
#include <iostream>
#include <vector>


long long solve_bruteforce(int n) {
    std::vector<long long> arr(n);
    // Calculate no less than the preciding one
    //             1
    //           2   1
    //         6   3   1
    //       20  10  4   1
    //     70  35  15  5   1
    // We reverse each row to make it easier to calculate arr[]
    long long answer = 0;
    int mod = 1000000007;
    for (int i = 1; i <= n; i++) {
        arr.at(0) = 1;
        for (int j = 1; j < i - 1; j++) {
            arr.at(j) = (arr.at(j - 1) + arr.at(j)) % mod;
        }
        if (i - 2 >= 0) {
            arr.at(i - 1) = (arr.at(i - 2) * 2) % mod;
        }
    }
    for (int i = 0; i < n; i++) {
        answer = (answer + arr.at(i)) % mod;
    }
    // Calculate no more and no less than the preciding one
    answer = (answer * 2 - n) % mod;
    return answer;
}


class Combinatorics {
// To calculate C(n, r) mod p =  (n! / (r!  (n - r)!) ) mod p
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

    int n;
    int mod;
    std::vector<long long> factorial, inverse_factorial;

public:
    Combinatorics(int n, int mod) : n(n), mod(mod), factorial(n + 1), inverse_factorial(n + 1) {
        factorial[0] = 1;
        for (int i = 1; i <= n; i++) factorial.at(i) = factorial.at(i - 1) * i % mod;
        // Using Fermat's Little Theorem if p is prime:  a^(-1) mod p = a^(p - 2) mod p
        inverse_factorial.at(n) = modpow(factorial.at(n), mod - 2, mod);
        for (int i = n - 1; i >= 0; i--) inverse_factorial.at(i) = inverse_factorial.at(i + 1) * (i + 1) % mod;
    }

    long long C(int n, int r) const {
        // 0 <= r <= n, mod is a prime number
        assert (0 <= r);
        assert (r <= n);
        return factorial.at(n) * inverse_factorial.at(r) % mod * inverse_factorial.at(n - r) % mod;
    }
};


long long solve_efficient(int n) {
    // From https://oeis.org/, formula is C(2n, n) - n
    int mod = 1000000007;
    Combinatorics combinatorics = Combinatorics(2 * n, mod);
    long long answer = combinatorics.C(2 * n, n) - n;
    return answer;
}


void test() {
    for (int n = 1; n <= 1000; n++) {
        long long answer_bruteforce = solve_bruteforce(n);
        long long answer_efficient = solve_efficient(n);
        assert(answer_efficient == answer_bruteforce);
    }
    {
        long long answer_bruteforce = solve_bruteforce(10000);
        long long answer_efficient = solve_efficient(10000);
        assert(answer_efficient == answer_bruteforce);
    }
}


int main() {
    // test();
    int n; std::cin >> n;
    long long answer_efficient = solve_efficient(n);
    std::cout << answer_efficient << std::endl;
    return 0;
}
