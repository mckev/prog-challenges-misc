// https://leetcode.com/problems/power-of-two/description/

#include <cassert>


class Solution {
public:
    bool isPowerOfTwo(long long n) {
        return (n != 0) && !(n & (n - 1));
    }
};


class Solution2 {
public:
    bool isPowerOfTwo(long long n) {
        if (n == 0) return false;
        if (n == 1) return true;
        if (n % 2 != 0) return false;
        return isPowerOfTwo(n / 2);
    }
};


int main() {
    for (int n = -1000000; n < 1000000; n++) {
        Solution solution = Solution();
        Solution2 solution2 = Solution2();
        bool answer = solution.isPowerOfTwo(n);
        bool answer2 = solution2.isPowerOfTwo(n);
        assert(answer == answer2);
    }
    return 0;
}
