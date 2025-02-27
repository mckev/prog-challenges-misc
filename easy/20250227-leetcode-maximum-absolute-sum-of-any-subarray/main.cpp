// https://leetcode.com/problems/maximum-absolute-sum-of-any-subarray/

#include <cassert>
#include <vector>


class Solution {
public:
    int maxAbsoluteSum(const std::vector<int>& nums) {
        int answer = 0;
        int current_positive = 0;
        int current_negative = 0;
        for (int num : nums) {
            current_positive += num;
            if (current_positive < 0) current_positive = 0;
            current_negative += num;
            if (current_negative > 0) current_negative = 0;
            answer = std::max(answer, current_positive);
            answer = std::max(answer, -current_negative);
        }
        return answer;
    }
};


int main() {
    {
        Solution solution;
        std::vector<int> nums = {1, -3, 2, 3, -4};
        int answer = solution.maxAbsoluteSum(nums);
        assert(answer == 5);
    }
    {
        Solution solution;
        std::vector<int> nums = {2, -5, 1, -4, 3, -2};
        int answer = solution.maxAbsoluteSum(nums);
        assert(answer == 8);
    }
    return 0;
}
