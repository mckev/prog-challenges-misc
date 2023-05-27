// https://www.hackerrank.com/challenges/lego-blocks/problem
// Bruteforce approach to understand the pattern


#include <iostream>
#include <vector>


class Solution {
private:
    int height, width;
    std::vector<std::vector<bool>> layout;
    long long count;

public:
    long long lego_blocks(int n, int m) {
        height = n;
        width = m;
        layout.resize(height, std::vector<bool>(width - 1, false));
        count = 0;
        solve(0, 0);
        solve(0, 1);
        solve(0, 2);
        solve(0, 3);
        return count;
    }

    bool is_solid_structure() {
        for (int x = 0; x < width; x++) {
            bool is_solid = false;
            for (int y = 0; y < height; y++) {
                if (! layout[y][x]) {
                    is_solid = true;
                    break;
                }
            }
            if (! is_solid) {
                return false;
            }
        }
        return true;
    }

    void solve(int h, int w) {
        if (h == height - 1 && w == width - 1) {
            if (is_solid_structure()) {
                count++;
            }
            return;
        }
        if (h >= height || w >= width) {
            return;
        }
        if (w == width - 1) {
            solve(h + 1, 0);
            solve(h + 1, 1);
            solve(h + 1, 2);
            solve(h + 1, 3);
            return;
        }

        layout[h][w] = true;
        solve(h, w + 1);
        solve(h, w + 2);
        solve(h, w + 3);
        solve(h, w + 4);
        layout[h][w] = false;
    }

};


int main() {
    {
        Solution solution;
        std::cout << solution.lego_blocks(2, 2) << std::endl;       // Output: 3
    }
    {
        Solution solution;
        std::cout << solution.lego_blocks(3, 2) << std::endl;       // Output: 7
    }
    {
        Solution solution;
        std::cout << solution.lego_blocks(2, 3) << std::endl;       // Output: 9
    }
    {
        Solution solution;
        std::cout << solution.lego_blocks(4, 4) << std::endl;       // Output: 3375
    }
    {
        Solution solution;
        std::cout << solution.lego_blocks(3, 9) << std::endl;       // Output: 1878696
    }
    {
        Solution solution;
        std::cout << solution.lego_blocks(6, 5) << std::endl;       // Output: 10366370
    }
    return 0;
}
