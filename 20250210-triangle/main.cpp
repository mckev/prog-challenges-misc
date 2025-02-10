// https://leetcode.com/problems/triangle/
// https://dmoj.ca/problem/ioi94p1

#include <cassert>
#include <vector>


class Solution {
    public:
    int minimumTotal(std::vector<std::vector<int>>& triangle) {
        for (int i = triangle.size() - 1; i >= 1; i--) {
            // At level i, there is i + 1 elements
            for (int j = 0; j < i; j++) {
                // To detect overflow:  triangle.at(i - 1).at(j) += std::min(triangle.at(i).at(j), triangle.at(i).at(j + 1));
                triangle[i - 1][j] += std::min(triangle[i][j], triangle[i][j + 1]);
            }
        }
        return triangle[0][0];
    }
};


int main() {
    {
        std::vector<std::vector<int>> triangle = {{2}, {3, 4}, {6, 5, 7}, {4, 1, 8, 3}};
        Solution solution = Solution();
        int answer = solution.minimumTotal(triangle);
        assert (answer == 11);
    }
    {
        std::vector<std::vector<int>> triangle = {{-10}};
        Solution solution = Solution();
        int answer = solution.minimumTotal(triangle);
        assert (answer == -10);
    }
    return 0;
}
