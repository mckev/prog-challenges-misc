// https://leetcode.com/problems/container-with-most-water/description/

#include <cassert>
#include <random>
#include <vector>


class Solution {
public:
    int maxArea(const std::vector<int>& heights) const {
        int max_area = 0;
        int i = 0;
        int j = heights.size() - 1;
        while (i < j) {
            int area = std::min(heights[i], heights[j]) * (j - i);
            if (area > max_area) max_area = area;
            if (heights[j] > heights[i]) {
                int prev_i = i;
                while (i < j && heights[i] <= heights[prev_i]) i++;
            } else {
                int prev_j = j;
                while (i < j && heights[j] <= heights[prev_j]) j--;
            }
        }
        return max_area;
    }
};


class Solution2 {
public:
    int maxArea(const std::vector<int>& heights) const {
        int max_area = 0;
        for (int x1 = 0; x1 < heights.size(); x1++) {
            for (int x2 = x1 + 1; x2 < heights.size(); x2++) {
                int area = std::min(heights[x1], heights[x2]) * (x2 - x1);
                if (area > max_area) max_area = area;
            }
        }
        return max_area;
    }
};


int main() {
    {
        std::vector<int> heights = {1, 8, 6, 2, 5, 4, 8, 3, 7};
        Solution solution;
        int max_area = solution.maxArea(heights);
        assert(max_area == 49);
    }
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<> rnd(0, 1);
        for (int t = 0; t < 1000; t++) {
            std::vector<int> heights;
            int len = int(rnd(mt) * 1000) + 2;
            for (int x = 0; x < len; x++) {
                int height = int(rnd(mt) * 1000);
                heights.push_back(height);
            }
            Solution solution;
            Solution2 solution2;
            int max_area = solution.maxArea(heights);
            int max_area2 = solution2.maxArea(heights);
            assert(max_area == max_area2);
        }
    }
    return 0;
}
