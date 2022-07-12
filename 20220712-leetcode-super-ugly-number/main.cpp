#include <iostream>
#include <set>
#include <vector>


class Solution {
public:
    int nthSuperUglyNumber(int n, std::vector<int>& primes) {
        std::set<long long> pool = {1};
        for (int prime : primes) {
            pool.insert(prime);
        }
        long long min;
        for (int i = 0; i < n; i++) {
            min = *pool.begin(); pool.erase(min);
            // std::cout << min << std::endl;
            for (int prime : primes) {
                pool.insert(min * prime);
            }
        }
        return min;
    }
};


int main() {
    {
        Solution solution;
        std::vector<int> primes = {2, 7, 13, 19};
        std::cout << solution.nthSuperUglyNumber(12, primes) << std::endl;          // output: 32
    }
    {
        Solution solution;
        std::vector<int> primes = {7, 19, 29, 37, 41, 47, 53, 59, 61, 79, 83, 89, 101, 103, 109, 127, 131, 137, 139, 157, 167, 179, 181, 199, 211, 229, 233, 239, 241, 251};
        std::cout << solution.nthSuperUglyNumber(100000, primes) << std::endl;      // output: 1092889481
    }
    return 0;
}
