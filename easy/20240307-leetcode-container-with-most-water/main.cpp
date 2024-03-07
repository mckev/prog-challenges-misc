// https://leetcode.com/problems/container-with-most-water/description/

#include <algorithm>
#include <cassert>
#include <random>
#include <vector>


struct Line {
    int height;
    int x;
};


class Solution {
public:
    int maxArea(const std::vector<int>& heights) const {
        // Create a new heights which has much lesser members
        // Firstly create an ascending heights
        std::vector<Line> heights_ascending;
        for (int x = 0; x < heights.size(); x++) {
            if (heights_ascending.size() == 0 || heights[x] > heights_ascending.back().height) {
                heights_ascending.push_back({heights[x], x});
            }
        }
        // Secondly create a descending heights
        std::vector<Line> heights_descending;
        for (int x = heights.size() - 1; x >= heights_ascending.back().x + 1; x--) {
            if (heights_descending.size() == 0 || heights[x] > heights_descending.back().height) {
                heights_descending.push_back({heights[x], x});
            }
        }
        std::reverse(heights_descending.begin(), heights_descending.end());
        // Concatenate
        std::vector<Line> lines = heights_ascending;
        lines.insert(lines.end(), heights_descending.begin(), heights_descending.end());
        // Calculate
        int max_area = 0;
        for (int i = 0; i < lines.size(); i++) {
            for (int j = lines.size() - 1; j >= i + 1; j--) {
                int area = std::min(lines[i].height, lines[j].height) * (lines[j].x - lines[i].x);
                if (area > max_area) max_area = area;
                // Speed up: Skip as the height is limited to lines[i]
                if (lines[j].height > lines[i].height) break;
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
