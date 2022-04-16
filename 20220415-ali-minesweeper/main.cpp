// Minesweeper Board : https://twitter.com/PuzzleKlasik/status/1514876219510206467

#include <array>
#include <iostream>
#include <queue>
#include <vector>


// HELPER

template<typename T>
std::vector<std::vector<T>> combinations(const std::vector<T>& p, int r) {
   std::vector<std::vector<T>> output;
    if (r <= 0) {
        return output;
    }
    std::queue<std::vector<int>> queue;
    for (int i = 0; i < p.size(); i++) {
        // First we generate "A", "B", "C" and "D"
        std::vector<int> el = {i};
        queue.push(el);
    }
    while (! queue.empty()) {
        std::vector<int> el = queue.front(); queue.pop();
        if (el.size() == r) {
            std::vector<T> output_each;
            for (int i = 0; i < el.size(); i++) {
                output_each.push_back(p[el[i]]);
            }
            output.push_back(output_each);
        } else if (el.size() < r) {
            for (int i = el.back() + 1; i < p.size(); i++) {
                // From "A", we generate "AB", "AC", and "AD"
                // From "B", we generate "BC", and "BD"
                // From "C", we generate "CD"
                // From "D", we don't generate anything
                std::vector<int> el_new = el; el_new.push_back(i);
                queue.push(el_new);
            }
        } else {
            throw std::runtime_error("Should not happen!");
        }
    }
    return output;
}


// SOLUTION
static const int E = -1;
static const int BOMB = -2;
static const int BLOCK = -3;

typedef struct {
    int y, x;
} COORD_T;

typedef struct {
    COORD_T coord;
    int nbombs;
} HINT_T;


void print_board(const std::vector<std::vector<int>>& board) {
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            switch (board[i][j]) {
                case E:
                    std::cout << " ";
                    break;
                case BOMB:
                    std::cout << "*";
                    break;
                case BLOCK:
                    std::cout << "X";
                    break;
                default:
                    std::cout << board[i][j];
                    break;
            }
        }
        std::cout << std::endl;
    }
}


int count_bombs(const std::vector<std::vector<int>>& board, COORD_T coord) {
    int nbombs = 0;
    for (int i = coord.y - 1; i <= coord.y + 1; i++) {
        for (int j = coord.x - 1; j <= coord.x + 1; j++) {
            if (i >= 0 && i < board.size() && j >= 0 && j < board[0].size()) {
                if (board[i][j] == BOMB) {
                    nbombs++;
                }
            }
        }
    }
    return nbombs;
}


std::vector<COORD_T> get_blanks(const std::vector<std::vector<int>>& board, COORD_T coord) {
    std::vector<COORD_T> blanks;
    for (int i = coord.y - 1; i <= coord.y + 1; i++) {
        for (int j = coord.x - 1; j <= coord.x + 1; j++) {
            if (i >= 0 && i < board.size() && j >= 0 && j < board[0].size()) {
                if (board[i][j] == E) {
                    COORD_T blank = {i, j};
                    blanks.push_back(blank);
                }
            }
        }
    }
    return blanks;
}


bool solve(std::vector<std::vector<int>>& board, std::vector<HINT_T>& hints) {
    if (hints.size() == 0) {
        // Yay, we have solved the puzzle!
        print_board(board);
        exit(0);
    }

    HINT_T hint = hints.back(); hints.pop_back();
    int nbombs = count_bombs(board, hint.coord);
    if (nbombs > hint.nbombs) {
        hints.push_back(hint);
        return false;
    }

    std::vector<COORD_T> blanks = get_blanks(board, hint.coord);
    for (COORD_T coord : blanks) {
        // block others from trying to fill
        board[coord.y][coord.x] = BLOCK;
    }
    if (nbombs == hint.nbombs) {
        solve(board, hints);
    }
    std::vector<std::vector<COORD_T>> blank_combinations = combinations(blanks, hint.nbombs - nbombs);
    for (const auto& blank_combinations_each : blank_combinations) {
        for (COORD_T coord : blank_combinations_each) {
            board[coord.y][coord.x] = BOMB;
        }
        solve(board, hints);
        for (COORD_T coord : blank_combinations_each) {
            board[coord.y][coord.x] = BLOCK;
        }
    }
    for (COORD_T coord : blanks) {
        board[coord.y][coord.x] = E;
    }

    hints.push_back(hint);
    return false;
}


int main() {
    std::vector<std::vector<int>> board = {
        { E, E, E, E, E, 4, E, E, E, 4, E, E, E, E, E },
        { E, 3, E, 3, E, E, E, 2, E, E, E, 4, E, 4, E },
        { E, E, E, E, E, 4, E, 2, E, 3, E, E, E, E, E },
        { E, 6, E, 5, E, E, E, 2, E, E, E, 6, E, 3, E },
        { E, E, E, E, E, 3, E, 2, E, 3, E, E, E, E, 2 },
        { E, 2, E, 4, E, E, 2, E, 2, E, E, 3, E, 5, E },
        { E, E, E, E, E, 2, E, E, E, 3, E, E, E, E, E },
        { 4, E, 2, 2, 2, E, E, 3, E, E, 2, 2, 2, E, 3 },
        { E, E, E, E, E, 3, E, E, E, 2, E, E, E, E, E },
        { E, 3, E, 5, E, E, 2, E, 2, E, E, 3, E, 5, E },
        { 1, E, E, E, E, 4, E, 2, E, 5, E, E, E, E, E },
        { E, 5, E, 5, E, E, E, 2, E, E, E, 4, E, 4, E },
        { E, E, E, E, E, 4, E, 2, E, 5, E, E, E, E, E },
        { E, 2, E, 2, E, E, E, 2, E, E, E, 2, E, 3, E },
        { E, E, E, E, E, 4, E, E, E, 4, E, E, E, E, E }
    };

    std::vector<HINT_T> hints;
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            if (board[i][j] >= 0 && board[i][j] <= 8) {
                HINT_T hint;
                hint.coord = {i, j};
                hint.nbombs = board[i][j];
                hints.push_back(hint);
            }
        }
    }

    solve(board, hints);

    return 0;
}
