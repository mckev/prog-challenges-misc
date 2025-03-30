// https://leetcode.com/problems/next-permutation/

#include <cassert>
#include <queue>
#include <vector>


class Solution {
public:
    void nextPermutation(std::vector<int>& nums) {
        std::priority_queue<int, std::vector<int>, std::greater<int>> q;
        // Start from end. Find in which the sequence is decreasing.
        int pos;
        for (pos = nums.size() - 1; pos >= 0; pos--) {
            q.push(nums[pos]);
            if (pos + 1 <= nums.size() - 1 && nums[pos] < nums[pos + 1]) break;
        }
        if (pos == -1) {
            // Special case if all are increasing
            int i = 0;
            while (! q.empty()) { nums[i++] = q.top(); q.pop(); }
        } else {
            // Replace nums[pos] with the next sequence
            int i = pos + 1;
            while (! q.empty() && q.top() <= nums[pos]) { nums[i++] = q.top(); q.pop(); }
            nums[pos] = q.top(); q.pop();
            while (! q.empty()) { nums[i++] = q.top(); q.pop(); }
        }
    }
};


int main() {
    {
        Solution solution = Solution();
        std::vector<int> nums = {3, 2, 1};
        solution.nextPermutation(nums);
        std::vector<int> expected = {1, 2, 3};
        assert(nums == expected);
    }
    {
        Solution solution = Solution();
        std::vector<int> nums = {2, 2, 7, 5, 4, 3, 2, 2, 1};
        solution.nextPermutation(nums);
        std::vector<int> expected = {2, 3, 1, 2, 2, 2, 4, 5, 7};
        assert(nums == expected);
    }
    return 0;
}
