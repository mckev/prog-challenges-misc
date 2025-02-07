// https://leetcode.com/problems/trapping-rain-water/

#include <cassert>
#include <vector>


class Solution {
    public:
    int trap(const std::vector<int>& heights) {
        std::vector<int> left(heights.size());
        std::vector<int> right(heights.size());
        int max_height = 0;
        for (int i = 0; i < heights.size(); i++) {
            max_height = std::max(max_height, heights[i]);
            left[i] = max_height;
        }
        max_height = 0;
        for (int i = heights.size() - 1; i >= 0; i--) {
            max_height = std::max(max_height, heights[i]);
            right[i] = max_height;
        }
        int volume = 0;
        for (int i = 0; i < heights.size(); i++) {
            volume += std::min(left[i], right[i]) - heights[i];
        }
        return volume;
    }
};


int main() {
    {
        std::vector<int> heights = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};
        Solution solution = Solution();
        assert(solution.trap(heights) == 6);
    }
    {
        std::vector<int> heights = {4, 2, 0, 3, 2, 5};
        Solution solution = Solution();
        assert(solution.trap(heights) == 9);
    }
    return 0;
}
