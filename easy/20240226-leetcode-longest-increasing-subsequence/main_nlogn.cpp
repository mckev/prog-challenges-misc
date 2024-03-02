// https://leetcode.com/problems/longest-increasing-subsequence/

#include <algorithm>
#include <cassert>
#include <vector>


class Solution {
public:
    int lengthOfLIS(const std::vector<int>& nums) {
        // Idea is not from myself, but from a colleague from office.
        // Algorithm: Construct a sorted vector "v". Replace the first element of "v" that is greater or equal than "num".
        // Time Complexity: O(n log n)
        std::vector<int> v;
        for (int num : nums) {
            auto greater_equal_iterator = std::lower_bound(v.begin(), v.end(), num);
            if (greater_equal_iterator == v.end()) {
                // Not found, all elements are lesser than "num"
                v.push_back(num);
            } else {
                // Replace the first element that is greater or equal with "num"
                (*greater_equal_iterator) = num;
            }
        }
        return v.size();
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
