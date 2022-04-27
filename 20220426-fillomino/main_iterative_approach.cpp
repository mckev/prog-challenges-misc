// Puzzle's Link : https://twitter.com/PuzzleKlasik/status/1518500080197775360

#include <algorithm>
#include <cassert>
#include <deque>
#include <iostream>
#include <limits>
#include <set>
#include <tuple>
#include <vector>


static const int E = 0;

struct COORD_T {
    int y, x;
    bool operator<(const COORD_T& b) const {
        return std::tie(y, x) < std::tie(b.y, b.x);
    }
};

struct HINT_T {
    COORD_T coord;
    int target;
};


void print_board(const std::vector<std::vector<int>>& board) {
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            switch (board[i][j]) {
                case E:
                    std::cout << ".";
                    break;
                default:
                    std::cout << board[i][j];
                    break;
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


void count_shape(const std::vector<std::vector<int>>& board, int target, COORD_T coord, std::set<COORD_T>& shape) {
    // Count the area of the polyomino/shape at coordinate (y, x)
    // Output: shape.size()
    if (coord.y < 0 || coord.x < 0 || coord.y >= board.size() || coord.x >= board[0].size()) {
        return;
    }
    if (board[coord.y][coord.x] != target) {
        return;
    }
    bool is_in = shape.find({coord.y, coord.x}) != shape.end();
    if (is_in) {
        return;
    }
    shape.insert(coord);
    count_shape(board, target, {coord.y - 1, coord.x}, shape);
    count_shape(board, target, {coord.y + 1, coord.x}, shape);
    count_shape(board, target, {coord.y, coord.x - 1}, shape);
    count_shape(board, target, {coord.y, coord.x + 1}, shape);
}


bool verify_final_board(const std::vector<std::vector<int>>& _board) {
    // Rules: https://www.gmpuzzles.com/blog/fillomino-rules-and-info/
    std::vector<std::vector<int>> board = _board;
    // Fill in the E
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            if (board[i][j] != E) continue;
            std::set<COORD_T> shape;
            count_shape(board, E, {i, j}, shape);
            for (const COORD_T& coord : shape) {
                board[coord.y][coord.x] = shape.size();
            }
        }
    }
    // Verify the number of each square is correct
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            int target = board[i][j];
            std::set<COORD_T> shape;
            count_shape(board, target, {i, j}, shape);
            if (shape.size() != target) {
                return false;
            }
        }
    }
    return true;
}


bool verify_intermediate_board(const std::vector<std::vector<int>>& board) {
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            if (board[i][j] == E) continue;
            int target = board[i][j];
            std::set<COORD_T> shape;
            count_shape(board, target, {i, j}, shape);
            if (shape.size() > target) {
                return false;
            }
        }
    }
    return true;
}


bool solve(std::vector<std::vector<int>>& board, std::deque<HINT_T>& hints);


void explore_shapes(std::vector<std::vector<int>>& board, std::deque<HINT_T>& hints, COORD_T start_coord, int target) {
    // This function generates all possible polyominoes/shapes with size "target". Will call solve() for each polyomino/shape to proceed with the next hint.

    std::set<std::set<COORD_T>> prev_shapes;
    std::deque<std::set<COORD_T>> shapes; shapes.push_back({{start_coord.y, start_coord.x}});
    while (! shapes.empty()) {
        std::set<COORD_T> shape = shapes.front(); shapes.pop_front();

        // Efficiency: Ensure this is not just permutation of the previous shapes
        bool is_in = prev_shapes.find(shape) != prev_shapes.end();
        if (is_in) {
            continue;
        }
        prev_shapes.insert(shape);

        if (shape.size() == target) {
            // We have achieved the target polyomino/shape size!
            // Proceed to the next hint
            std::vector<std::vector<int>> board_copy = board;
            for (const COORD_T& coord : shape) {
                board[coord.y][coord.x] = target;
            }
            // Efficiency: Do not further process if the board is invalid, for example: two shapes with the same area share an edge
            if (verify_intermediate_board(board)) {
                solve(board, hints);
            }
            // Notice that we may overwrite the hint, or may overwrite squares that have the same target, simplest way is to restore the board from a copy
            board = board_copy;
            continue;
        }
        assert (shape.size() <= target);

        for (const COORD_T& coord : shape) {
            // Efficiency: Ensure we grow the size of the intermediate shape
            bool is_in;
            is_in = shape.find({coord.y - 1, coord.x}) != shape.end();
            if (coord.y - 1 >= 0 && (board[coord.y - 1][coord.x] == E || board[coord.y - 1][coord.x] == target) && ! is_in) {
                std::set<COORD_T> new_shape = shape; new_shape.insert({coord.y - 1, coord.x}); shapes.push_back(new_shape);
            }
            is_in = shape.find({coord.y + 1, coord.x}) != shape.end();
            if (coord.y + 1 < board.size() && (board[coord.y + 1][coord.x] == E || board[coord.y + 1][coord.x] == target) && ! is_in) {
                std::set<COORD_T> new_shape = shape; new_shape.insert({coord.y + 1, coord.x}); shapes.push_back(new_shape);
            }
            is_in = shape.find({coord.y, coord.x - 1}) != shape.end();
            if (coord.x - 1 >= 0 && (board[coord.y][coord.x - 1] == E || board[coord.y][coord.x - 1] == target) && ! is_in) {
                std::set<COORD_T> new_shape = shape; new_shape.insert({coord.y, coord.x - 1}); shapes.push_back(new_shape);
            }
            is_in = shape.find({coord.y, coord.x + 1}) != shape.end();
            if (coord.x + 1 < board[coord.y].size() && (board[coord.y][coord.x + 1] == E || board[coord.y][coord.x + 1] == target) && ! is_in) {
                std::set<COORD_T> new_shape = shape; new_shape.insert({coord.y, coord.x + 1}); shapes.push_back(new_shape);
            }
        }
    }
}


bool solve(std::vector<std::vector<int>>& board, std::deque<HINT_T>& hints) {
    // Print progress
    // static int best = std::numeric_limits<int>::max();
    // if (hints.size() < best) {
    //     best = hints.size();
    //     std::cout << "Progress: " << hints.size() << " hints remaining" << std::endl;
    //     print_board(board);
    // }

    HINT_T hint;
    if (hints.empty()) {
        if (verify_final_board(board)) {
            std::cout << "SOLVED:" << std::endl;
            print_board(board);
        }
        return true;
    }
    hint = hints.front(); hints.pop_front();

    explore_shapes(board, hints, hint.coord, hint.target);

    hints.push_front(hint);
    return false;
}


int main() {
    // Board: https://twitter.com/PuzzleKlasik/status/1518500080197775360
    // std::vector<std::vector<int>> board = {
    //     {1, 2, 3, 4, 5, 6, 7},
    //     {E, E, E, E, E, E, E},
    //     {E, E, E, E, E, E, E},
    //     {E, 3, E, E, E, E, E},
    //     {E, E, 4, E, E, E, E},
    //     {2, E, E, 5, E, E, E},
    //     {E, E, E, E, E, E, E}
    // };
    // Board: https://www.gmpuzzles.com/blog/fillomino-rules-and-info/
    std::vector<std::vector<int>> board = {
        {E, E, E, E, E, E, E, 2, 3},
        {E, 3, 2, 6, E, E, E, E, E},
        {E, E, E, 2, E, E, E, E, E},
        {E, 5, 2, 1, E, 4, 2, 2, E},
        {E, 3, E, E, E, E, E, 3, E},
        {E, 2, 1, 5, E, 5, 7, 9, E},
        {E, E, E, E, E, E, E, 2, E},
        {E, E, E, E, E, 5, 2, 4, E},
        {2, 3, E, E, E, E, E, E, E}
    };

    std::deque<HINT_T> hints;
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            if (board[i][j] != E) {
                HINT_T hint;
                hint.coord = {i, j};
                hint.target = board[i][j];
                hints.push_back(hint);
            }
        }
    }
    std::reverse(hints.begin(), hints.end());

    solve(board, hints);

    return 0;
}
