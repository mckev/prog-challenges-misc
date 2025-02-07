// https://leetcode.com/problems/trapping-rain-water/

#include <cassert>
#include <vector>


class Solution {
    public:
    int trap(const std::vector<int>& heights) {
        std::vector<int> water_level(heights.size());
        for (int i = 0; i < heights.size(); i++) {
            water_level[i] = std::max(i == 0 ? 0 : water_level[i - 1], heights[i]);
        }
        for (int i = heights.size() - 1; i >= 0; i--) {
            water_level[i] = std::min(water_level[i], std::max(i == heights.size() - 1 ? 0 : water_level[i + 1], heights[i]));
        }
        int volume = 0;
        for (int i = 0; i < heights.size(); i++) {
            volume += water_level[i] - heights[i];
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
