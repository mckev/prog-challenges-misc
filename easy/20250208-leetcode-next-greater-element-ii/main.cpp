// https://leetcode.com/problems/next-greater-element-ii/

#include <cassert>
#include <stack>
#include <vector>


class Solution {
    public:
    struct Element {
        int num;
        int pos;
    };
    std::vector<int> nextGreaterElements(const std::vector<int>& nums) {
        std::vector<int> results(nums.size(), -1);
        std::stack<Element> elements;
        for (int i = 0; i < nums.size() * 2; i++) {
            int num = nums[i % nums.size()];
            while (!elements.empty() && elements.top().num < num) {
                results[elements.top().pos % nums.size()] = num;
                elements.pop();
            }
            elements.push({num, i});
        }
        return results;
    }
};


int main() {
    {
        Solution solution = Solution();
        std::vector<int> results = solution.nextGreaterElements({1, 2, 1});
        std::vector<int> expected = {2, -1, 2};
        assert(results == expected);
    }
    {
        Solution solution = Solution();
        std::vector<int> results = solution.nextGreaterElements({1, 2, 3, 4, 3});
        std::vector<int> expected = {2, 3, 4, -1, 4};
        assert(results == expected);
    }
    {
        Solution solution = Solution();
        std::vector<int> results = solution.nextGreaterElements({1, 2, 3, 2, 1});
        std::vector<int> expected = {2, 3, -1, 3, 2};
        assert(results == expected);
    }
    return 0;
}
