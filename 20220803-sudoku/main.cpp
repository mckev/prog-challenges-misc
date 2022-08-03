#include <cassert>
#include <iostream>
#include <set>
#include <vector>

const int E = -1;

void print_board(const std::vector<int>& board) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            std::cout << board[i * 9 + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int get_empty(const std::vector<int>& board) {
    for (int index = 0; index < 9 * 9; index++) {
        if (board[index] == E) return index;
    }
    return E;
}

std::set<int> get_possible_numbers(const std::vector<int>& board, int index) {
    assert(board[index] == E);
    int y = index / 9;
    int x = index % 9;
    std::set<int> possible_numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (int i = 0; i < 9; i++) {
        possible_numbers.erase(board[i * 9 + x]);
        possible_numbers.erase(board[y * 9 + i]);
    }
    for (int i = y - y % 3; i < y - y % 3 + 3; i++) {
        for (int j = x - x % 3; j < x - x % 3 + 3; j++) {
            possible_numbers.erase(board[i * 9 + j]);
        }
    }
    return possible_numbers;
}

void fill(std::vector<int>& board) {
    int index = get_empty(board);
    if (index == E) {
        // Yay we have solved it!
        print_board(board);
        return;
    }
    std::set<int> possible_numbers = get_possible_numbers(board, index);
    for (int number : possible_numbers) {
        board[index] = number;
        fill(board);
        board[index] = E;
    }
}

int main() {
    // https://abcnews.go.com/blogs/headlines/2012/06/can-you-solve-the-hardest-ever-sudoku
    std::vector<int> board = {
        8, E, E, E, E, E, E, E, E,
        E, E, 3, 6, E, E, E, E, E,
        E, 7, E, E, 9, E, 2, E, E,
        E, 5, E, E, E, 7, E, E, E,
        E, E, E, E, 4, 5, 7, E, E,
        E, E, E, 1, E, E, E, 3, E,
        E, E, 1, E, E, E, E, 6, 8,
        E, E, 8, 5, E, E, E, 1, E,
        E, 9, E, E, E, E, 4, E, E
    };
    fill(board);
    return 0;
}
