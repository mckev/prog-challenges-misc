// https://leetcode.com/problems/house-robber

#include <algorithm>
#include <cassert>
#include <vector>


class Solution {
public:
    int rob(const std::vector<int>& nums) {
        // We need to only process house + 2 or house + 3
        int max_sum = 0;
        std::vector<int> sums(nums.size() + 3);
        for (int i = nums.size() - 1; i >= 0; i--) {
            sums[i] = std::max(nums[i] + sums[i + 2], nums[i] + sums[i + 3]);
            if (sums[i] > max_sum) {
                max_sum = sums[i];
            }
        }
        return max_sum;
    }
};


int main() {
    {
        Solution solution;
        std::vector<int> nums = {1, 2, 3, 1};
        int max_sum = solution.rob(nums);
        assert(max_sum == 4);
    }
    {
        Solution solution;
        std::vector<int> nums = {2, 7, 9, 3, 1};
        int max_sum = solution.rob(nums);
        assert(max_sum == 12);

    }
    return 0;
}
