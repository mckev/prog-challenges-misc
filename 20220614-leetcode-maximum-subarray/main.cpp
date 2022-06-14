#include <iostream>
#include <limits>
#include <vector>


class Solution {
public:
    int maxSubArray(std::vector<int>& nums) {
        int max = std::numeric_limits<int>::min();
        int cur = std::numeric_limits<int>::min();
        for (int num : nums) {
            if (cur < 0) {
                cur = num;
            } else {
                cur += num;
            }
            if (cur > max) {
                max = cur;
            }
        }
        return max;
    }
};


int main() {
    Solution solution = Solution();
    std::vector<int> nums = {-2, 1, -3, 4, -1, 2, 1, -5, 4};        // ans: 6
    int max = solution.maxSubArray(nums);
    std::cout << max << std::endl;
}
