// https://codingcompetitions.withgoogle.com/codejam/round/00000000008778ec/0000000000b15a74

#include <cassert>
#include <iostream>
#include <vector>


struct Coord {
    int y, x;
    Coord operator+(const Coord& other) const {
        return {y + other.y, x + other.x};
    }
    Coord operator*(int n) const {
        return {y * n, x * n};
    }
    bool operator!=(const Coord& other) const {
        return !(y == other.y && x == other.x);
    }
};
std::vector<Coord> clockwise = {
    {0, 1},     // RIGHT
    {1, 0},     // DOWN
    {0, -1},    // LEFT
    {-1, 0}     // UP
};


class WalkCenter {
private:
    int N;
    Coord start_coord;
    int start_dir;
    int start_distance;
    int segment_len;
    Coord coord;

public:
    WalkCenter(int N, Coord start_coord, int start_dir, int start_distance, int segment_len, const Coord& coord) {
        assert (N % 2 == 1);
        this->N = N;
        this->start_coord = start_coord;
        this->start_dir = start_dir;
        this->start_distance = start_distance;
        this->segment_len = segment_len;
        this->coord = coord;
    }

    static int distance_to_center(int N, const Coord& coord) {
        int middle = (N - 1) / 2;
        return std::abs(coord.y - middle) + std::abs(coord.x - middle);
    }

    static int direction_to_center(int N, const Coord& coord) {
        /*
                                |
                            WALK DOWN
                                |
            ---- WALK RIGHT ----+---- WALK LEFT ----
                                |
                            WALK UP
                                |
        */
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
                WALK RIGHT      |     WALK DOWN
                                |
            --------------------+--------------------
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

    static const int NOT_IN_SEGMENT = -1;
    static int distance_in_segment(const Coord& start_coord, const Coord& end_coord, const Coord& coord) {
        if (start_coord.y == end_coord.y && start_coord.y == coord.y) {
            return std::abs(coord.x - start_coord.x);
        } else if (start_coord.x == end_coord.x && start_coord.x == coord.x) {
            return std::abs(coord.y - start_coord.y);
        } else {
            // coord is on different segment
            return NOT_IN_SEGMENT;
        }
    }

    int walk() {
        int direction = direction_to_center(N, coord);
        coord = coord + clockwise[direction];

        // Calculate board[coord.y][coord.x]
        while (true) {
            Coord end_coord = start_coord + clockwise[start_dir] * std::max(0, segment_len - 1);
            int distance_in_segment = WalkCenter::distance_in_segment(start_coord, end_coord, coord);
            if (distance_in_segment != NOT_IN_SEGMENT) {
                int index = start_distance + distance_in_segment + 1;
                return index;
            }
            // Next segment
            start_distance += segment_len;
            if (start_dir < 3) {
                // Case RIGHT, DOWN, LEFT
                start_coord = end_coord + clockwise[start_dir];
                start_dir = (start_dir + 1) % 4;
            } else {
                // Case UP
                start_dir = (start_dir + 1) % 4;
                start_coord = end_coord + clockwise[start_dir];
                segment_len -= 2;
            }
        }
    }
};


class WalkSpiral {
private:
    int N, K;
    Coord start_coord;
    int start_dir;
    int start_distance;
    int segment_len;

public:
    WalkSpiral(int N, int K) {
        assert (N % 2 == 1);
        this->N = N;
        this->K = K;
        this->start_coord = {0, 0};
        this->start_dir = 0;
        this->start_distance = 0;
        this->segment_len = N - 1;
    }

    std::vector<int> answer;

    bool walk_segment() {
        Coord mid_coord = start_coord + clockwise[start_dir] * (segment_len / 2);
        Coord end_coord = start_coord + clockwise[start_dir] * std::max(0, segment_len - 1);
        assert (segment_len >= 0);
        assert (start_coord.y >= 0); assert (start_coord.x >= 0); assert (start_coord.y < N); assert (start_coord.x < N);
        assert (end_coord.y >= 0); assert (end_coord.x >= 0); assert (end_coord.y < N); assert (end_coord.x < N);
        // std::cout << "Segment: (" << start_coord.y << ", " << start_coord.x << ") - (" << end_coord.y << ", " << end_coord.x << ")" << std::endl;

        if (
            (start_distance + WalkCenter::distance_to_center(N, start_coord) <= K && K <= start_distance + segment_len / 2 + WalkCenter::distance_to_center(N, mid_coord)) ||
            (start_distance + segment_len / 2 + WalkCenter::distance_to_center(N, mid_coord) <= K && K <= start_distance + segment_len - 1 + WalkCenter::distance_to_center(N, end_coord))
           ) {
            int distance = 0;
            for (Coord coord = start_coord; coord != end_coord + clockwise[start_dir]; coord = coord + clockwise[start_dir], distance++) {
                // std::cout << "Traversing (" << coord.y << ", " << coord.x << "). Distance: " << start_distance + distance << "." << std::endl;
                if (start_distance + distance + WalkCenter::distance_to_center(N, coord) == K) {
                    int index = start_distance + distance + 1; answer.push_back(index);
                    WalkCenter walk_center = WalkCenter(N, start_coord, start_dir, start_distance, segment_len, coord);
                    while (true) {
                        index = walk_center.walk(); answer.push_back(index);
                        if (index == N * N) break;
                    }
                    // We have found the answer!
                    return false;
                }
            }
        }

        if (segment_len == 0) {
            // We have reached center!
            return false;
        }

        // Next segment
        start_distance += segment_len;
        if (start_dir < 3) {
            // Case RIGHT, DOWN, LEFT
            start_coord = end_coord + clockwise[start_dir];
            start_dir = (start_dir + 1) % 4;
        } else {
            // Case UP
            start_dir = (start_dir + 1) % 4;
            start_coord = end_coord + clockwise[start_dir];
            segment_len -= 2;
        }
        return true;
    }
};


std::vector<int> solve(int N, int K) {
    WalkSpiral walk_spiral = WalkSpiral(N, K);
    while (walk_spiral.walk_segment()) {};
    return walk_spiral.answer;
}


// void test() {
//     {
//         std::vector<int> paths = solve(9, 10);
//     }
//     {
//         std::vector<int> paths = solve(9, 18);
//     }
// }


int main() {
    // test();

    int T;
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
