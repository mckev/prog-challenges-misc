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


std::vector<Coord> walk_in_spiral(int N) {
    std::vector<Coord> output;
    // Walk right N - 1, then walk down N - 1, then walk left N - 1, then
    // walk up N - 2, then walk right N - 2, then
    // walk down N - 3, then walk left N - 3, then
    Coord cur_coord = {0, 0};
    int cur_direction = 0;
    int num_sides = 3;
    int len_side = N - 1;
    while (len_side > 0) {
        for (int side = 0; side < num_sides; side++) {
            for (int i = 0; i < len_side; i++) {
                output.push_back(cur_coord);
                cur_coord = cur_coord + clockwise[cur_direction];
            }
            cur_direction = (cur_direction + 1) % 4;
        }
        num_sides = 2;
        len_side--;
    }
    output.push_back(cur_coord);
    return output;
}


int walk_in_shortcut(const std::vector<std::vector<int>>& board, const Coord& cur_coord) {
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
    if (cur_coord.x == middle) {
        if (cur_coord.y < middle) {
            return 1;   // WALK DOWN
        } else {
            return 3;   // WALK UP
        }
    }
    if (cur_coord.y == middle) {
        if (cur_coord.x < middle) {
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
    if (cur_coord.x < middle) {
        if (cur_coord.y < middle) {
            return 0;   // WALK RIGHT
        } else {
            return 3;   // WALK UP
        }
    }
    if (cur_coord.x > middle) {
        if (cur_coord.y < middle) {
            return 1;   // WALK DOWN
        } else {
            return 2;   // WALK LEFT
        }
    }
    assert (false);
}


std::vector<int> solve(int N, int K) {
    std::vector<int> paths;
    // Generate board
    std::vector<std::vector<int>> board(N, std::vector<int>(N, EMPTY));
    std::vector<Coord> spiral = walk_in_spiral(N);
    int index = 0;
    for (const Coord& coord : spiral) {
        index++;
        board[coord.y][coord.x] = index;
    }
    // for (int y = 0; y < N; y++) {
    //     for (int x = 0; x < N; x++) {
    //         std::cout << board[y][x] << " ";
    //     }
    //     std::cout << std::endl;
    // }

    // Walk the spiral and observe if we can take shortcuts to the center
    int middle = (N - 1) / 2;
    for (const Coord& coord : spiral) {
        paths.push_back(board[coord.y][coord.x]);
        int distance_to_center = std::abs(coord.y - middle) + std::abs(coord.x - middle);
        if (paths.size() + distance_to_center - 1 == K) {
            Coord coord2 = coord;
            while (true) {
                int direction = walk_in_shortcut(board, coord2);
                coord2 = coord2 + clockwise[direction];
                paths.push_back(board[coord2.y][coord2.x]);
                if (board[coord2.y][coord2.x] == N * N) return paths;
            }
            assert (false);
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
