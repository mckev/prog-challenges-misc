// Puzzle's Link : https://twitter.com/PuzzleKlasik/status/1517775292751912967

#include <cassert>
#include <iostream>
#include <set>
#include <deque>
#include <vector>


static const int E = 0;


void print_board(const std::vector<std::vector<int>>& board) {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            std::cout << board[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


bool is_rules_followed(const std::vector<int>& rows, int target_visible) {
    assert(rows.size() == 6);
    int nempty = 0, nvisible = 0, cur_height = 0;
    for (int i = 0; i < 6; i++) {
        if (rows[i] == E) {
            nempty++;
            continue;
        }
        if (rows[i] > cur_height) {
            cur_height = rows[i];
            nvisible++;
        }
    }
    if (nempty == 0 && nvisible != target_visible) return false;
    return true;
}


bool is_all_rules_followed(const std::vector<std::vector<int>>& b) {
    if (! is_rules_followed({b[0][0], b[1][0], b[2][0], b[3][0], b[4][0], b[5][0]}, 1)) return false;
    if (! is_rules_followed({b[0][2], b[1][2], b[2][2], b[3][2], b[4][2], b[5][2]}, 2)) return false;
    if (! is_rules_followed({b[0][3], b[1][3], b[2][3], b[3][3], b[4][3], b[5][3]}, 3)) return false;
    if (! is_rules_followed({b[0][4], b[1][4], b[2][4], b[3][4], b[4][4], b[5][4]}, 4)) return false;

    if (! is_rules_followed({b[5][0], b[4][0], b[3][0], b[2][0], b[1][0], b[0][0]}, 2)) return false;
    if (! is_rules_followed({b[5][2], b[4][2], b[3][2], b[2][2], b[1][2], b[0][2]}, 3)) return false;
    if (! is_rules_followed({b[5][3], b[4][3], b[3][3], b[2][3], b[1][3], b[0][3]}, 4)) return false;
    if (! is_rules_followed({b[5][5], b[4][5], b[3][5], b[2][5], b[1][5], b[0][5]}, 5)) return false;

    if (! is_rules_followed({b[1][0], b[1][1], b[1][2], b[1][3], b[1][4], b[1][5]}, 4)) return false;
    if (! is_rules_followed({b[2][0], b[2][1], b[2][2], b[2][3], b[2][4], b[2][5]}, 3)) return false;
    if (! is_rules_followed({b[3][0], b[3][1], b[3][2], b[3][3], b[3][4], b[3][5]}, 2)) return false;

    if (! is_rules_followed({b[2][5], b[2][4], b[2][3], b[2][2], b[2][1], b[2][0]}, 2)) return false;
    if (! is_rules_followed({b[3][5], b[3][4], b[3][3], b[3][2], b[3][1], b[3][0]}, 3)) return false;
    if (! is_rules_followed({b[4][5], b[4][4], b[4][3], b[4][2], b[4][1], b[4][0]}, 4)) return false;

    return true;
}


std::set<int> get_possible_heights(const std::vector<std::vector<int>>& board, int y, int x) {
    std::set<int> output = {1, 2, 3, 4, 5, 6};
    for (int i = 0; i < 6; i++) {
        if (board[i][x] == E) continue;
        output.erase(board[i][x]);
    }
    for (int j = 0; j < 6; j++) {
        if (board[y][j] == E) continue;
        output.erase(board[y][j]);
    }
    return output;
}


bool solve(std::vector<std::vector<int>>& board, std::deque<int>& empties_avail) {
    if (empties_avail.empty()) {
        print_board(board);
        return true;
    }

    int empty_avail = empties_avail.front(); empties_avail.pop_front();
    int y = empty_avail / 6;
    int x = empty_avail % 6;

    std::set<int> possible_heights = get_possible_heights(board, y, x);
    for (int height : possible_heights) {
        board[y][x] = height;
        if (! is_all_rules_followed(board)) {
            continue;
        }
        solve(board, empties_avail);
    }

    board[y][x] = E;
    empties_avail.push_front(empty_avail);
    return false;
}


int main() {
    std::vector<std::vector<int>> board = {
        {E, E, E, E, E, E},
        {E, E, E, E, E, E},
        {E, E, E, E, E, E},
        {E, E, E, E, E, E},
        {E, E, E, E, E, E},
        {E, E, E, E, E, E},
    };

    std::deque<int> empties_avail;
    for (int i = 0; i < 36; i++) {
        empties_avail.push_back(i);
    }

    solve(board, empties_avail);

    return 0;
}
