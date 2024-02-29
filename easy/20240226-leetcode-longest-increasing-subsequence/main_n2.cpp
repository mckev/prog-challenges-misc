// https://leetcode.com/problems/longest-increasing-subsequence/

#include <algorithm>
#include <cassert>
#include <vector>


class Solution {
public:
    int lengthOfLIS(const std::vector<int>& nums) {
        // Time Complexity: O(n^2)
        std::vector<int> lengths(nums.size(), 1);
        for (int i = 0; i < nums.size(); i++) {
            // Find an element which is on the left and lesser, that has the greatest length
            for (int j = 0; j < i; j++) {
                if (nums[j] < nums[i]) {
                    lengths[i] = std::max(lengths[i], lengths[j] + 1);
                }
            }
        }
        int max_length = *std::max_element(lengths.begin(), lengths.end());
        return max_length;
    }
};


int main() {
    {
        std::vector<int> nums = {10, 9, 2, 5, 3, 7, 101, 18};
        Solution solution = Solution();
        int answer = solution.lengthOfLIS(nums);
        assert(answer == 4);
    }
    {
        std::vector<int> nums = {0, 1, 0, 3, 2, 3};
        Solution solution = Solution();
        int answer = solution.lengthOfLIS(nums);
        assert(answer == 4);
    }
    {
        std::vector<int> nums = {7, 7, 7, 7, 7, 7, 7};
        Solution solution = Solution();
        int answer = solution.lengthOfLIS(nums);
        assert(answer == 1);
    }
    return 0;
}
