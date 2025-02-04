// https://leetcode.com/problems/largest-rectangle-in-histogram/

#include <algorithm>
#include <cassert>
#include <deque>
#include <random>
#include <vector>


class SolutionBruteForce {
public:
    int largestRectangleArea(const std::vector<int>& heights) {
        int max_area = 0;
        for (int i = 0; i < heights.size(); i++) {
            // Rule #1: The area must be as tall as heights[i]
            int area = heights[i];
            // Scan to the right side
            for (int j = i + 1; j < heights.size(); j++) {
                if (heights[j] < heights[i]) break;
                area += heights[i];
            }
            // Scan to the left side
            for (int j = i - 1; j >= 0; j--) {
                if (heights[j] < heights[i]) break;
                area += heights[i];
            }
            max_area = std::max(max_area, area);
        }
        return max_area;
    }
};


class SolutionEfficient {
public:
    struct Histogram {
        int num;
        int index;
        static bool upper_bound_comparator(int num, const Histogram& other) {
            // For upper_bound()
            return num > other.num;
        }
    };

    int largestRectangleArea(const std::vector<int>& heights) {
        int max_area = 0;

        // Scan to the left side
        std::deque<Histogram> left;
        std::vector<int> left_area(heights.size());
        for (int i = 0; i < heights.size(); i++) {
            int area = 0;
            // Find the histogram in which height < heights[i]
            auto index_upper = std::upper_bound(left.begin(), left.end(), heights[i], Histogram::upper_bound_comparator);
            if (index_upper != left.end()) {
                int x = i - (*index_upper).index - 1;
                area = heights[i] * x;
            } else {
                int x = i;
                area = heights[i] * x;
            }
            // Delete histograms which height >= heights[i]
            left.erase(left.begin(), index_upper);
            // Add histogram
            left.push_front({heights[i], i});
            left_area[i] = area;
        }

        // Scan to the right side
        // Rule #2: To solve right side, we scan from right to left 
        std::deque<Histogram> right;
        std::vector<int> right_area(heights.size());
        for (int i = heights.size() - 1; i >= 0; i--) {
            int area = 0;
            // Find the histogram in which height < heights[i]
            auto index_upper = std::upper_bound(right.begin(), right.end(), heights[i], Histogram::upper_bound_comparator);
            if (index_upper != right.end()) {
                int x = heights.size() - 1 - i - (*index_upper).index - 1;
                area = heights[i] * x;
            } else {
                int x = heights.size() - 1 - i;
                area = heights[i] * x;
            }
            // Delete histograms which height >= heights[i]
            right.erase(right.begin(), index_upper);
            // Add histogram
            right.push_front({heights[i], (int) heights.size() - 1 - i});
            right_area[i] = area;
        }

        for (int i = 0; i < heights.size(); i++) {
            // Rule #1: The area must be as tall as heights[i]
            int area = heights[i] + left_area[i] + right_area[i];
            max_area = std::max(max_area, area);
        }
        return max_area;
    }
};


int main() {
    {
        std::vector<int> heights = {2, 1, 5, 6, 2, 3};
        SolutionBruteForce solutionBruteForce = SolutionBruteForce();
        assert(solutionBruteForce.largestRectangleArea(heights) == 10);
        SolutionEfficient solutionEfficient = SolutionEfficient();
        assert(solutionEfficient.largestRectangleArea(heights) == 10);
    }
    {
        std::vector<int> heights = {2, 4};
        SolutionBruteForce solutionBruteForce = SolutionBruteForce();
        assert(solutionBruteForce.largestRectangleArea(heights) == 4);
        SolutionEfficient solutionEfficient = SolutionEfficient();
        assert(solutionEfficient.largestRectangleArea(heights) == 4);
    }
    {
        std::vector<int> heights = {35, 19, 49, 46, 61};
        SolutionBruteForce solutionBruteForce = SolutionBruteForce();
        assert(solutionBruteForce.largestRectangleArea(heights) == 138);
        SolutionEfficient solutionEfficient = SolutionEfficient();
        assert(solutionEfficient.largestRectangleArea(heights) == 138);
    }
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<> rnd(0, 1);                     // [0, 1)
        for (int T = 0; T < 1000; T++) {
            int num_heights = int(rnd(mt) * 1000) + 1;
            std::vector<int> heights;
            for (int i = 0; i < num_heights; i++) {
                int height = int(rnd(mt) * 100);
                heights.push_back(height);
            }
            SolutionBruteForce solutionBruteForce = SolutionBruteForce();
            int answer1 = solutionBruteForce.largestRectangleArea(heights);
            SolutionEfficient solutionEfficient = SolutionEfficient();
            int answer2 = solutionEfficient.largestRectangleArea(heights);
            assert(answer1 == answer2);
        }
    }
    return 0;
}
