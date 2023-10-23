// https://www.hackerrank.com/challenges/lego-blocks


#include <iostream>

long long lego_combinations[1001][1001];
long long lego_answer[1001][1001];


class Solution {
public:
    long long modulo(long long x) {
        if (x < 0) {
            x = x % 1000000007 + 1000000007;
        }
        return x % 1000000007;
    }

    long long compute_lego_combinations(const int n, const int m) {
        if (n <= 0) return 0;
        if (m <= 0) return 0;
        if (lego_combinations[n][m] > 0) return lego_combinations[n][m];
        if (n == 1) {
            lego_combinations[1][m] = modulo(compute_lego_combinations(1, m - 1) + compute_lego_combinations(1, m - 2) + compute_lego_combinations(1, m - 3) + compute_lego_combinations(1, m - 4));
        } else {
            lego_combinations[n][m] = modulo(compute_lego_combinations(n - 1, m) * compute_lego_combinations(1, m));                // compute_lego_combinations[1][m] ** n
        }
        return lego_combinations[n][m];
    }

    long long compute_lego_breaks(const int n, const int m) {
        long long total_nbreaks = 0;
        for (int i = 1; i < m; i++) {
            total_nbreaks += modulo(lego_blocks(n, i) * compute_lego_combinations(n, m - i));
        }
        return total_nbreaks;
    }

    long long lego_blocks(int height, int width) {
        if (height == 1 && width >= 5) return 0;
        if (lego_answer[height][width] > 0) return lego_answer[height][width];
        long long total_ncombinations = compute_lego_combinations(height, width);
        long long total_nbreaks = compute_lego_breaks(height, width);
        // std::cout << "Breaks " << height << " x " << width << " : " << total_nbreaks << std::endl;
        lego_answer[height][width] = modulo(total_ncombinations - total_nbreaks);
        return lego_answer[height][width];
    }
};


int main() {
    lego_combinations[1][1] = 1;
    lego_combinations[1][2] = 2;
    lego_combinations[1][3] = 4;
    lego_combinations[1][4] = 8;

    // Solution solution;
    // std::cout << solution.lego_blocks(4, 9) << std::endl;                    // ans: 839737250
    // std::cout << solution.lego_blocks(6, 6) << std::endl;                    // ans: 524291222

    int t;
    std::cin >> t;
    while (t--) {
        int n, m;
        std::cin >> n >> m;
        Solution solution;
        long long answer = solution.lego_blocks(n, m);
        std::cout << answer << std::endl;
    }
}
