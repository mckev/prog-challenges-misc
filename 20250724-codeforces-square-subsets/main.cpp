// Square Subsets: https://codeforces.com/problemset/problem/895/C

#include <bitset>
#include <cassert>
#include <iostream>
#include <map>
#include <random>
#include <set>
#include <vector>
#pragma GCC optimize "trapv"                                        // Detect overflow
// #define _TEST


#ifdef _TEST
class BruteforceSolution {
private:
    long long answer;

    bool hasIntegerSquareRoot(__int128_t n) {
        if (n < 0) return false;
        long long root = static_cast<long long>(std::sqrt(static_cast<long double>(n)));
        return static_cast<__int128>(root) * root == n;
    }

    void process(const std::vector<int>& inputs, int pos, __int128_t multiplication, std::vector<int> poses) {
        // End
        if (pos >= inputs.size()) {
            if (hasIntegerSquareRoot(multiplication)) {
                // std::cout << "DEBUG: "; for (int i : poses) { std::cout << inputs.at(i) << " "; }; std::cout << std::endl;
                answer++;
            }
            return;
        }

        // Either we pick ...
        __int128_t pick_multiplication = multiplication * inputs.at(pos);
        std::vector<int> pick_poses = poses; pick_poses.push_back(pos);
        process(inputs, pos + 1, pick_multiplication, pick_poses);
        // ... or not pick
        process(inputs, pos + 1, multiplication, poses);
    }

public:
    long long solve(const std::vector<int>& inputs) {
        answer = 0;
        process(inputs, 0, 1, {});
        return answer - 1;
    }
};
#endif


class Solution {
private:
    long long modpow(long long base, long long exp, long long mod) {
        // Computes (base^exp) % mod using binary exponentiation
        base %= mod;                                                // Ensure base is within mod
        long long result = 1;
        while (exp > 0) {
            if (exp & 1)                                            // If exp is odd
                result = (result * base) % mod;
            base = (base * base) % mod;
            exp >>= 1;                                              // exp = exp / 2
        }
        return result;
    }

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

    static const int MAX_NUM_PRIMES = 19;
    uint32_t compute_odd_prime_powers(const std::vector<int>& primes, int number) {
        // Observations:
        //   - Number 2 behaves similarly to 2*2*2 = 8, 2*3*3 = 18, 2*5*5 = 50, 2*2*2*2*2 = 32, 2*3*3*3*3 = 162, 2*2*2*2*2*2*2 = 128
        //     Means we are only interested on the odd powers.
        //   - Max output is 19 bits which fits 32 bits integer.
        //     uint32_t is interchangable with std::bitset<MAX_NUM_PRIMES>.

        std::bitset<MAX_NUM_PRIMES> odd_prime_powers;
        if (number == 0) {
            return odd_prime_powers.to_ulong();
        }
        int remainder = number;
        for (int i = 0; i < (int) primes.size(); i++) {
            int power = 0;
            while (remainder % primes.at(i) == 0) {
                power++;
                remainder /= primes.at(i);
            }
            if (power % 2 == 1) {
                if (i < MAX_NUM_PRIMES) {
                    odd_prime_powers[i] = 1;
                } else {
                    assert(false);
                }
            }
        }
        return odd_prime_powers.to_ulong();
    }

    struct History {
        int pos;
        uint32_t resultant;
        bool operator<(const History& other) const {
            return std::tie(pos, resultant) < std::tie(other.pos, other.resultant);
        }
    };
    std::set<History> history;

    struct Element {
        const uint32_t odd_prime_powers;
        const int occurrences;
        bool is_full_mark;
    };
    std::vector<Element> elements;

    void process(int pos, uint32_t resultant, std::vector<int> poses) {
        if (pos >= (int) elements.size()) {
            return;
        }

        // Have we encountered this before?
        bool is_exist = history.find({pos, resultant}) != history.end();
        if (is_exist) {
            return;
        }
        history.insert({pos, resultant});

        // Case we pick the element
        std::vector<int> pick_poses = poses; pick_poses.push_back(pos);
        uint32_t pick_resultant = (resultant xor elements.at(pos).odd_prime_powers);
        if (pick_resultant == 0) {
            // If the XOR operation is 0, means it's a square.
            // std::cout << "Got XOR: "; for (int _pos : pick_poses) { std::cout << elements.at(_pos).odd_prime_powers << " "; }; std::cout << std::endl;
            elements.at(pos).is_full_mark = true;
        }
        process(pos + 1, pick_resultant, pick_poses);

        // Case we do not pick the element
        process(pos + 1, resultant, poses);
    }

public:
    static const int MAX_NUMBER = 70;

    long long solve(std::vector<int>& inputs) {
        // std::cout << "--- Case: "; for (int input : inputs) { std::cout << input << " "; }; std::cout << std::endl;
        history.clear();
        elements.clear();

        // Establish numbering system based on odd prime powers
        std::vector<int> primes = compute_primes(MAX_NUMBER);
        std::map<int, uint32_t> odd_prime_powers_mapping;
        for (int number = 0; number <= MAX_NUMBER; number++) {
            uint32_t odd_prime_powers = compute_odd_prime_powers(primes, number);
            odd_prime_powers_mapping[number] = odd_prime_powers;
            // std::cout << "Map " << number << " to " << std::bitset<MAX_NUM_PRIMES>(odd_prime_powers) << std::endl;
        }

        // Count their occurrences
        std::map<uint32_t, int> odd_prime_powers_counters;
        for (int input : inputs) {
            odd_prime_powers_counters[odd_prime_powers_mapping.at(input)]++;
        }

        // Convert into vector
        for (const auto& [k, v] : odd_prime_powers_counters) {
            // std::cout << std::bitset<MAX_NUM_PRIMES>(k) << " (" << k << "): " << v << " occurrences" << std::endl;
            elements.push_back({ k, v, false });
        }

        // Observations:
        //   - Even occurrences always make a square, so 2^(n-1) of each element is already a square.
        //   - Here we need to find combinations of elements which their XOR is 0 (meaning they are a square), in this case the last element got a full mark, i.e. 2^n.
        process(0, 0, {});

        long long answer = 1;
        for (const Element& element : elements) {
            if (element.is_full_mark) {
                answer *= modpow(2, element.occurrences, 1000000007);
            } else {
                answer *= modpow(2, element.occurrences - 1, 1000000007);
            }
            answer %= 1000000007;
        }
        return answer - 1;
    }
};


void test() {
    {
        std::vector<int> inputs = {1, 1, 1, 1};
        Solution solution = Solution();
        long long answer1 = solution.solve(inputs);
        assert(answer1 == 15);
#ifdef _TEST
        BruteforceSolution bruteforce_solution = BruteforceSolution();
        long long answer2 = bruteforce_solution.solve(inputs);
        assert(answer2 == answer1);
#endif
    }
    {
        std::vector<int> inputs = {2, 2, 2, 2};
        Solution solution = Solution();
        long long answer1 = solution.solve(inputs);
        assert(answer1 == 7);
#ifdef _TEST
        BruteforceSolution bruteforce_solution = BruteforceSolution();
        long long answer2 = bruteforce_solution.solve(inputs);
        assert(answer2 == answer1);
#endif
    }
    {
        std::vector<int> inputs = {1, 2, 4, 5, 8};
        Solution solution = Solution();
        long long answer1 = solution.solve(inputs);
        assert(answer1 == 7);
#ifdef _TEST
        BruteforceSolution bruteforce_solution = BruteforceSolution();
        long long answer2 = bruteforce_solution.solve(inputs);
        assert(answer2 == answer1);
#endif
    }
    {
        std::vector<int> inputs = {1, 2, 3, 4, 5, 6};
        Solution solution = Solution();
        long long answer1 = solution.solve(inputs);
        assert(answer1 == 7);
#ifdef _TEST
        BruteforceSolution bruteforce_solution = BruteforceSolution();
        long long answer2 = bruteforce_solution.solve(inputs);
        assert(answer2 == answer1);
#endif
    }
    {
        std::vector<int> inputs = {70, 70};
        Solution solution = Solution();
        long long answer1 = solution.solve(inputs);
        assert(answer1 == 1);
#ifdef _TEST
        BruteforceSolution bruteforce_solution = BruteforceSolution();
        long long answer2 = bruteforce_solution.solve(inputs);
        assert(answer2 == answer1);
#endif
    }
    {
        std::vector<int> inputs = {64, 65, 40, 26, 36, 46, 53, 31, 63, 11, 2, 46, 59};
        Solution solution = Solution();
        long long answer1 = solution.solve(inputs);
        assert(answer1 == 15);
#ifdef _TEST
        BruteforceSolution bruteforce_solution = BruteforceSolution();
        long long answer2 = bruteforce_solution.solve(inputs);
        assert(answer2 == answer1);
#endif
    }
    {
        std::vector<int> inputs = {44, 57, 54, 57, 54, 65, 40, 57, 59, 16, 39, 51, 32, 51, 20, 9, 8};
        Solution solution = Solution();
        long long answer1 = solution.solve(inputs);
        assert(answer1 == 511);
#ifdef _TEST
        BruteforceSolution bruteforce_solution = BruteforceSolution();
        long long answer2 = bruteforce_solution.solve(inputs);
        assert(answer2 == answer1);
#endif
    }
    {
        std::vector<int> inputs = {50, 40, 62, 60, 29, 51, 12, 40, 57, 36, 35, 45, 51, 4, 25, 31};
        Solution solution = Solution();
        long long answer1 = solution.solve(inputs);
        assert(answer1 == 255);
#ifdef _TEST
        BruteforceSolution bruteforce_solution = BruteforceSolution();
        long long answer2 = bruteforce_solution.solve(inputs);
        assert(answer2 == answer1);
#endif
    }
    {
        std::vector<int> inputs = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70};
        Solution solution = Solution();
        long long answer1 = solution.solve(inputs);
        assert(answer1 == 797922654);
    }
    {
        Solution solution = Solution();
#ifdef _TEST
        BruteforceSolution bruteforce_solution = BruteforceSolution();
#endif
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<> rnd(0, 1);                     // [0, 1)
        for (int T = 0; T < 100; T++) {
            int N = int(rnd(mt) * 20) + 1;                              // Max combinations: 2^20
            std::vector<int> inputs;
            for (int n = 0; n < N; n++) {
                int input = int(rnd(mt) * Solution::MAX_NUMBER) + 1;    // [1 - 70]
                inputs.push_back(input);
            }
            long long answer1 = solution.solve(inputs);
#ifdef _TEST
            long long answer2 = bruteforce_solution.solve(inputs);
            assert(answer2 == answer1);
#endif
        }
    }
    {
        Solution solution = Solution();
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<> rnd(0, 1);                     // [0, 1)
        for (int T = 0; T < 3; T++) {
            int N = int(rnd(mt) * 10000) + 1;                           // Max combinations: 2^10000
            std::vector<int> inputs;
            for (int n = 0; n < N; n++) {
                int input = int(rnd(mt) * Solution::MAX_NUMBER) + 1;    // [1 - 70]
                inputs.push_back(input);
            }
            long long answer1 = solution.solve(inputs);
        }
    }
    std::cout << "Tests passed." << std::endl;
}


int main() {
#ifdef _TEST
    test();
#endif

    int N; std::cin >> N;
    std::vector<int> inputs;
    for (int n = 0; n < N; n++) {
        int input; std::cin >> input;
        inputs.push_back(input);
    }
    Solution solution = Solution();
    long long answer = solution.solve(inputs);
    std::cout << answer << std::endl;

    return 0;
}
