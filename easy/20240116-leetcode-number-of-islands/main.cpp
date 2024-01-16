// https://leetcode.com/problems/number-of-islands/

#include <iostream>
#include <vector>


class Solution {
public:
    const char ISLAND_UNEXPLORED = '1';
    const char ISLAND_EXPLORED = '*';
    const char WATER = '0';

    void fill(std::vector<std::vector<char>>& grid, int x, int y) {
        if (y < 0 || y >= grid.size()) return;
        if (x < 0 || x >= grid[y].size()) return;
        if (grid[y][x] != ISLAND_UNEXPLORED) return;
        grid[y][x] = ISLAND_EXPLORED;
        fill(grid, x, y - 1);
        fill(grid, x, y + 1);
        fill(grid, x - 1, y);
        fill(grid, x + 1, y);
    }

    int numIslands(std::vector<std::vector<char>>& grid) {
        int num_islands = 0;
        for (int y = 0; y < grid.size(); y++) {
            for (int x = 0; x < grid[y].size(); x++) {
                if (grid[y][x] == ISLAND_UNEXPLORED) {
                    num_islands++;
                    fill(grid, x, y);
                }
            }
        }
        return num_islands;
    }
};


int main() {
    {
        Solution solution;
        std::vector<std::vector<char>> grid = {
            {'1','1','1','1','0'},
            {'1','1','0','1','0'},
            {'1','1','0','0','0'},
            {'0','0','0','0','0'}
        };
        std::cout << solution.numIslands(grid) << std::endl;        // Expected: 1
    }
    {
        Solution solution;
        std::vector<std::vector<char>> grid = {
            {'1','1','0','0','0'},
            {'1','1','0','0','0'},
            {'0','0','1','0','0'},
            {'0','0','0','1','1'}
        };
        std::cout << solution.numIslands(grid) << std::endl;        // Expected: 3
    }
    return 0;
}
