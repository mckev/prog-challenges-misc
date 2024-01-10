#include <iostream>


class Solution {
public:
    int answers[50];

    int climbStairs(int n) {
        if (n == 1) return 1;
        if (n == 2) return 2;
        if (answers[n] > 0) return answers[n];
        answers[n] = climbStairs(n - 1) + climbStairs(n - 2);
        return answers[n];
    }
};


int main() {
    Solution solution = Solution();
    std::cout << solution.climbStairs(1) << std::endl;
    std::cout << solution.climbStairs(2) << std::endl;
    std::cout << solution.climbStairs(3) << std::endl;
    std::cout << solution.climbStairs(4) << std::endl;
    std::cout << solution.climbStairs(5) << std::endl;
    std::cout << solution.climbStairs(45) << std::endl;
}
