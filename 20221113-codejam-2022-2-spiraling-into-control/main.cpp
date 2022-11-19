// https://codingcompetitions.withgoogle.com/codejam/round/00000000008778ec/0000000000b15a74

#include <cassert>
#include <iostream>
#include <vector>


struct Coord {
    int y, x;
    Coord operator+(const Coord& other) const {
        return {y + other.y, x + other.x};
    }
};
std::vector<Coord> clockwise = {
    {0, 1},     // RIGHT
    {1, 0},     // DOWN
    {0, -1},    // LEFT
    {-1, 0}     // UP
};
const int EMPTY = -1;


int distance_to_center(int N, const Coord& coord) {
    int middle = (N - 1) / 2;
    return std::abs(coord.y - middle) + std::abs(coord.x - middle);
}

int direction_to_center(const std::vector<std::vector<int>>& board, const Coord& coord) {
    /*
                            |
                        WALK DOWN
                            |
        ---- WALK RIGHT ----+---- WALK LEFT ----
                            |
                         WALK UP
                            |
    */
    int N = board.size();
    int middle = (N - 1) / 2;
    if (coord.x == middle) {
        if (coord.y < middle) {
            return 1;   // WALK DOWN
        } else {
            return 3;   // WALK UP
        }
    }
    if (coord.y == middle) {
        if (coord.x < middle) {
            return 0;   // WALK RIGHT
        } else {
            return 2;   // WALK LEFT
        }
    }
    /*
                            |
             WALK RIGHT     |     WALK DOWN
                            |
        --------------------+-------------------
                            |
              WALK UP       |     WALK LEFT
                            |
    */
    if (coord.x < middle) {
        if (coord.y < middle) {
            return 0;   // WALK RIGHT
        } else {
            return 3;   // WALK UP
        }
    }
    if (coord.x > middle) {
        if (coord.y < middle) {
            return 1;   // WALK DOWN
        } else {
            return 2;   // WALK LEFT
        }
    }
    assert (false);
}

std::vector<int> concat_paths_to_center(const std::vector<std::vector<int>>& board, Coord coord, std::vector<int>& paths) {
    int N = board.size();
    while (board[coord.y][coord.x] != N * N) {
        int direction = direction_to_center(board, coord);
        coord = coord + clockwise[direction];
        paths.push_back(board[coord.y][coord.x]);
    }
    return paths;
}

std::vector<int> solve(int N, int K) {
    std::vector<int> paths;

    // Generate board
    std::vector<std::vector<int>> board(N, std::vector<int>(N, EMPTY));
    int middle = (N - 1) / 2;
    int index = 1;
    for (int i = 0; i < middle; i++) {
        for (int j = 0; j < N - 1 - (i * 2); j++) {
            int y = i, x = i + j;
            board[y][x] = index; index++;
        }
        for (int j = 0; j < N - 1 - (i * 2); j++) {
            int y = i + j, x = N - 1 - i;
            board[y][x] = index; index++;
        }
        for (int j = 0; j < N - 1 - (i * 2); j++) {
            int y = N - 1 - i, x = N - 1 - j - i;
            board[y][x] = index; index++;
        }
        for (int j = 0; j < N - 1 - (i * 2); j++) {
            int y = N - 1 - j - i, x = i;
            board[y][x] = index; index++;
        }
    }
    board[middle][middle] = index;
    // for (int y = 0; y < N; y++) {
    //     for (int x = 0; x < N; x++) {
    //         std::cout << board[y][x] << " ";
    //     }
    //     std::cout << std::endl;
    // }
    // std::cout << std::endl;

    // Walk the spiral and observe if we can take shortcuts to the center
    for (int i = 0; i < middle; i++) {
        for (int j = 0; j < N - 1 - (i * 2); j++) {
            int y = i, x = i + j;
            paths.push_back(board[y][x]);
            if (paths.size() - 1 + distance_to_center(N, {y, x}) == K) {
                return concat_paths_to_center(board, {y, x}, paths);
            }
        }
        for (int j = 0; j < N - 1 - (i * 2); j++) {
            int y = i + j, x = N - 1 - i;
            paths.push_back(board[y][x]);
            if (paths.size() - 1 + distance_to_center(N, {y, x}) == K) {
                return concat_paths_to_center(board, {y, x}, paths);
            }
        }
        for (int j = 0; j < N - 1 - (i * 2); j++) {
            int y = N - 1 - i, x = N - 1 - j - i;
            paths.push_back(board[y][x]);
            if (paths.size() - 1 + distance_to_center(N, {y, x}) == K) {
                return concat_paths_to_center(board, {y, x}, paths);
            }
        }
        for (int j = 0; j < N - 1 - (i * 2); j++) {
            int y = N - 1 - j - i, x = i;
            paths.push_back(board[y][x]);
            if (paths.size() - 1 + distance_to_center(N, {y, x}) == K) {
                return concat_paths_to_center(board, {y, x}, paths);
            }
        }
    }
    return {};
}

int main() {
    int T;
    // std::vector<int> paths = solve(9, 10);
    std::cin >> T;
    for (int t = 0; t < T; t++) {
        int N; std::cin >> N;
        int K; std::cin >> K;
        std::cout << "Case #" << t + 1 << ": ";
        std::vector<int> paths = solve(N, K);
        if (paths.size() == 0) {
            std::cout << "IMPOSSIBLE" << std::endl;
        } else {
            int counter = 0;
            for (int i = 0; i < paths.size() - 1; i++) {
                if (paths[i + 1] != paths[i] + 1) counter++;
            }
            std::cout << counter << std::endl;
            for (int i = 0; i < paths.size() - 1; i++) {
                if (paths[i + 1] != paths[i] + 1) {
                    std::cout << paths[i] << " " << paths[i + 1] << std::endl;
                }
            }
        }
    }
    return 0;
}
