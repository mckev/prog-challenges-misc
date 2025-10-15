// https://www.facebook.com/codingcompetitions/hacker-cup/2025/practice-round/problems/B

#include <cassert>
#include <iostream>
#include <vector>


const char EMPTY = '.';
const char OBJECT = '#';
const char DANGER = 'D';
const char EXPLORED = 'E';      // to count safe area


void mark_danger(std::vector<std::vector<char>>& house_layout, int center_x, int center_y, int S) {
    int R = house_layout.size();
    int C = house_layout.at(0).size();
    for (int x = center_x - S - 1; x <= center_x + S + 1; x++) {
        for (int y = center_y - S - 1; y <= center_y + S + 1; y++) {
            if (x < 0 || x >= C) continue;
            if (y < 0 || y >= R) continue;
            if (house_layout.at(y).at(x) == OBJECT) continue;
            if (std::abs(x - center_x) + std::abs(y - center_y) > S) continue;
            house_layout.at(y).at(x) = DANGER;
        }
    }
}


void print_layout(const std::vector<std::vector<char>>& house_layout) {
    int R = house_layout.size();
    int C = house_layout.at(0).size();
    for (int y = 0; y < R; y++) {
        for (int x = 0; x < C; x++) {
            std::cout << house_layout.at(y).at(x);
        }
        std::cout << std::endl;
    }
}


class Fill {
public:
    int area = 0;
    void fill(std::vector<std::vector<char>>& house_layout, int x, int y) {
        int R = house_layout.size();
        int C = house_layout.at(0).size();
        if (x < 0 || x >= C) return;
        if (y < 0 || y >= R) return;
        if (house_layout.at(y).at(x) != EMPTY) return;
        area++;
        house_layout.at(y).at(x) = EXPLORED;
        fill(house_layout, x, y - 1);
        fill(house_layout, x, y + 1);
        fill(house_layout, x - 1, y);
        fill(house_layout, x + 1, y);
    }
};


int solve(std::vector<std::vector<char>>& house_layout, int S) {
    int R = house_layout.size();
    assert(R > 0);
    int C = house_layout.at(0).size();
    // Process top
    for (int x = 0; x < C; x++) {
        if (x < 0 || x >= C) continue;
        for (int y = 0; y < S; y++) {
            if (y < 0 || y >= R) continue;
            if (house_layout.at(y).at(x) == OBJECT) continue;
            house_layout.at(y).at(x) = DANGER;
        }
    }
    // Process bottom
    for (int x = 0; x < C; x++) {
        if (x < 0 || x >= C) continue;
        for (int y = R - S; y < R; y++) {
            if (y < 0 || y >= R) continue;
            if (house_layout.at(y).at(x) == OBJECT) continue;
            house_layout.at(y).at(x) = DANGER;
        }
    }
    // Process left
    for (int y = 0; y < R; y++) {
        if (y < 0 || y >= R) continue;
        for (int x = 0; x < S; x++) {
            if (x < 0 || x >= C) continue;
            if (house_layout.at(y).at(x) == OBJECT) continue;
            house_layout.at(y).at(x) = DANGER;
        }
    }
    // Process right
    for (int y = 0; y < R; y++) {
        if (y < 0 || y >= R) continue;
        for (int x = C - S; x < C; x++) {
            if (x < 0 || x >= C) continue;
            if (house_layout.at(y).at(x) == OBJECT) continue;
            house_layout.at(y).at(x) = DANGER;
        }
    }
    // Process objects
    for (int y = 0; y < R; y++) {
        for (int x = 0; x < C; x++) {
            if (house_layout.at(y).at(x) == OBJECT) {
                mark_danger(house_layout, x, y, S);
            }
        }
    }
    // Print
    // print_layout(house_layout);
    // Count areas
    int answer = 0;
    for (int y = 0; y < R; y++) {
        for (int x = 0; x < C; x++) {
            if (house_layout.at(y).at(x) == EMPTY) {
                Fill f = Fill();
                f.fill(house_layout, x, y);
                answer = std::max(answer, f.area);
            }
        }
    }
    return answer;
}


void test() {
    {
        int S = 3;
        std::vector<std::string> house_layout_in_string = {
            "....................",
            "....................",
            "..........####......",
            ".........#..........",
            ".........#..........",
            ".......######.......",
            ".........#..........",
            ".........#..........",
            ".........#..........",
            ".........#..........",
        };
        std::vector<std::vector<char>> house_layout;
        for (const std::string& row : house_layout_in_string) {
            std::vector<char> char_row(row.begin(), row.end());
            house_layout.push_back(char_row);
        }
        int answer = solve(house_layout, S);
        assert(answer == 8);
    }
}


int main() {
    // test();
    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        int R; std::cin >> R;
        int C; std::cin >> C;
        int S; std::cin >> S;
        std::vector<std::vector<char>> house_layout;
        for (int r = 0; r < R; r++) {
            std::string row; std::cin >> row;
            std::vector<char> char_row(row.begin(), row.end());
            house_layout.push_back(char_row);
        }
        int answer = solve(house_layout, S);
        std::cout << "Case #" << t + 1 << ": " << answer << std::endl;
    }
    return 0;
}
