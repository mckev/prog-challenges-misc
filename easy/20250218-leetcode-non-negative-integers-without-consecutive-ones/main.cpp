// https://leetcode.com/problems/non-negative-integers-without-consecutive-ones/

#include <cassert>


class Solution {
    public:
    static const int MAX_BITS = 32;
    int dp[MAX_BITS];

    int findIntegers(int N) {
        dp[0] = 1;
        dp[1] = 2;
        for (int i = 2; i < MAX_BITS; i++) {
            dp[i] = dp[i - 1] + dp[i - 2];
        }
        int answer = 0;
        for (int bit = MAX_BITS - 1; bit >= 0; bit--) {
            if (N & (1 << bit)) {
                // If current bit is set
                answer += dp[bit];
                if (N & (1 << bit + 1)) {
                    // If previous bit is also set
                    return answer;
                }
            }
        }
        answer++;
        return answer;
    }
};


class SolutionBruteForce {
    public:
    static const int MAX_BITS = 32;
    int findIntegers(int N) {
        int answer = 0;
        for (int n = 0; n <= N; n++) {
            bool is_previous_one = false;
            for (int bit = 0; bit < MAX_BITS; bit++) {
                if (n & (1 << bit)) {
                    if (is_previous_one) {
                        goto skip;
                    }
                    is_previous_one = true;
                } else {
                    is_previous_one = false;
                }
            }
            answer++;
            skip:
            ;
        }
        return answer;
    }
};


int main() {
    {
        SolutionBruteForce solutionBruteForce = SolutionBruteForce();
        Solution solution = Solution();
        for (int i = 0; i < 10000; i++) {
            int answer1 = solutionBruteForce.findIntegers(i);
            int answer2 = solution.findIntegers(i);
            assert(answer1 == answer2);
        }
    }
    return 0;
}
