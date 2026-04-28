// https://leetcode.com/problems/check-if-there-is-a-valid-path-in-a-grid/

#include <cassert>
#include <utility>
#include <vector>


class Solution {
private:
    struct Direction {
        int dx;
        int dy;
        bool operator==(const Direction& other) const {
            return dx == other.dx && dy == other.dy;
        }
    };
    const Direction LEFT = { -1, 0 };
    const Direction RIGHT = { 1, 0 };
    const Direction UP = { 0, -1 };
    const Direction DOWN = { 0, 1 };
    struct Flow {
        Direction in;
        Direction out;
    };
    const std::vector<std::pair<Flow, Flow>> StreetFlows = {
        // There are two possible Flow per street
        { { RIGHT, RIGHT }, { LEFT, LEFT } },
        { { DOWN, DOWN }, { UP, UP } },
        { { RIGHT, DOWN }, { UP, LEFT } },
        { { LEFT, DOWN }, { UP, RIGHT } },
        { { DOWN, LEFT }, { RIGHT, UP } },
        { { DOWN, RIGHT }, { LEFT, UP } },
    };
    bool is_finished;

public:
    bool traverse(const std::vector<std::vector<int>>& grid, int& x, int& y, Direction& direction) {
        int street_flow = grid.at(y).at(x) - 1;
        // There are two possible Flow per street: first.in and second.in
        if (direction == StreetFlows.at(street_flow).first.in) {
            direction = StreetFlows.at(street_flow).first.out;
        } else if (direction == StreetFlows.at(street_flow).second.in) {
            direction = StreetFlows.at(street_flow).second.out;
        } else {
            return false;
        }
        // Special case: We arrived at bottom-right cell
        int max_y = grid.size(), max_x = grid.at(0).size();
        if (x == max_x - 1 && y == max_y - 1) {
            is_finished = true;
            return true;
        }
        // Update the x and y
        x += direction.dx;
        y += direction.dy;
        if (x < 0 || y < 0 || x >= max_x || y >= max_y) return false;
        return true;
    }

    bool hasValidPath(const std::vector<std::vector<int>>& grid) {
        // We started at upper-left cell
        int street_flow = grid.at(0).at(0) - 1;
        // There are two possible directions: first.in and second.in.
        // As the case of { {4, 1}, {6, 1} }, we need to traverse BOTH.
        {
            Direction direction = StreetFlows.at(street_flow).first.in;
            int x = 0, y = 0;
            is_finished = false;
            int max_y = grid.size(), max_x = grid.at(0).size(); std::vector<std::vector<bool>> is_traversed(max_y, std::vector<bool>(max_x, false));
            while (true) {
                if (is_traversed.at(y).at(x)) return false; is_traversed.at(y).at(x) = true;
                bool is_valid = traverse(grid, x, y, direction);
                if (! is_valid) break;
                if (is_finished) return true;
            }
        }
        {
            Direction direction = StreetFlows.at(street_flow).second.in;
            int x = 0, y = 0;
            is_finished = false;
            int max_y = grid.size(), max_x = grid.at(0).size(); std::vector<std::vector<bool>> is_traversed(max_y, std::vector<bool>(max_x, false));
            while (true) {
                if (is_traversed.at(y).at(x)) return false; is_traversed.at(y).at(x) = true;
                bool is_valid = traverse(grid, x, y, direction);
                if (! is_valid) break;
                if (is_finished) return true;
            }
        }
        return false;
    }
};


int main() {
    {
        std::vector<std::vector<int>> grid = {
            {2, 4, 3},
            {6, 5, 2},
        };
        Solution solution = Solution();
        assert(solution.hasValidPath(grid) == true);
    }
    {
        std::vector<std::vector<int>> grid = {
            {1, 2, 1},
            {1, 2, 1},
        };
        Solution solution = Solution();
        assert(solution.hasValidPath(grid) == false);
    }
    {
        std::vector<std::vector<int>> grid = {
            {1, 1, 2},
        };
        Solution solution = Solution();
        assert(solution.hasValidPath(grid) == false);
    }
    {
        // Need to explore BOTH directions on the upper-left cell
        std::vector<std::vector<int>> grid = {
            {4, 1},
            {6, 1},
        };
        Solution solution = Solution();
        assert(solution.hasValidPath(grid) == true);
    }
    {
        std::vector<std::vector<int>> grid = {
            {2, 6},
        };
        Solution solution = Solution();
        assert(solution.hasValidPath(grid) == false);
    }
    {
        // Circular test case
        std::vector<std::vector<int>> grid = {
            {4, 3, 3},
            {6, 5, 2},
        };
        Solution solution = Solution();
        assert(solution.hasValidPath(grid) == false);
    }
    return 0;
}
