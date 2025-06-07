// https://leetcode.com/problems/minimum-moves-to-clean-the-classroom/

#include <cassert>
#include <deque>
#include <string>
#include <tuple>
#include <vector>


class Solution {
private:
    const int LITTER = 'L';
    const int START = 'S';
    const int OBSTACLE = 'X';
    const int RESET = 'R';
    const int EMPTY = '.';

    int count_total_litters(const std::vector<std::string>& classroom) {
        int total_litters = 0;
        for (int y = 0; y < classroom.size(); y++) {
            for (int x = 0; x < classroom[y].size(); x++) {
                if (classroom[y][x] == LITTER) {
                    total_litters++;
                }
            }
        }
        return total_litters;
    }

    int count_reachable_litters(const std::vector<std::string>& _classroom, int start_y, int start_x) {
        int litters = 0;
        std::vector<std::string> classroom = _classroom;
        struct State {
            int y, x;
        };
        std::deque<State> states;
        states.push_back({start_y, start_x});
        while (! states.empty()) {
            State state = states.front(); states.pop_front();
            int y = state.y;
            int x = state.x;
            if (y < 0 || y >= classroom.size()) continue;
            if (x < 0 || x >= classroom[y].size()) continue;
            if (classroom[y][x] == OBSTACLE) continue;
            if (classroom[y][x] == LITTER) litters++;
            classroom[y][x] = OBSTACLE;
            states.push_back({ y - 1, x });
            states.push_back({ y + 1, x });
            states.push_back({ y, x - 1 });
            states.push_back({ y, x + 1 });
        }
        return litters;
    }

    std::tuple<int, int> get_start_coord(const std::vector<std::string>& classroom) {
        for (int y = 0; y < classroom.size(); y++) {
            for (int x = 0; x < classroom[y].size(); x++) {
                if (classroom[y][x] == START) {
                    return {y, x};
                }
            }
        }
        assert(false);
    }

public:
    static const int NO_SOLUTION = -1;

    int minMoves(const std::vector<std::string>& classroom, int full_energy) {
        int max_litters = 0;
        int min_moves = 0;
        auto [start_y, start_x] = get_start_coord(classroom);

        // Check if we can reach all litters
        int total_litters = count_total_litters(classroom);
        if (total_litters == 0) {
            return 0;
        }
        int reachable_litters = count_reachable_litters(classroom, start_y, start_x);
        if (reachable_litters < total_litters) return NO_SOLUTION;

        // To avoid BFS from processing previously explored path
        const int UNEXPLORED = -1;
        struct Optimizer {
            int litters;
            int moves;
        };
        std::vector<std::vector<Optimizer>> optimizers(classroom.size(), std::vector<Optimizer>(classroom[0].size(), {
            0, UNEXPLORED
        }));

        // BFS
        struct State {
            int y, x;
            int moves;
            int energy;
            int litters;
            std::vector<std::string> classroom;
            std::vector<std::vector<Optimizer>> optimizers;
        };
        std::deque<State> states;
        states.push_back({ start_y, start_x, 0, full_energy, 0, classroom, optimizers });
        while (! states.empty()) {
            State state = states.front(); states.pop_front();
            int y = state.y;
            int x = state.x;
            int moves = state.moves;
            int energy = state.energy;
            int litters = state.litters;
            std::vector<std::string> classroom = state.classroom;
            if (y < 0 || y >= classroom.size()) continue;
            if (x < 0 || x >= classroom[y].size()) continue;
            if (classroom[y][x] == OBSTACLE) continue;
            if (classroom[y][x] == RESET) {
                energy = full_energy;
            }
            if (classroom[y][x] == LITTER) {
                classroom[y][x] = EMPTY;
                litters++;
                if (litters > max_litters) {
                    max_litters = litters;
                    min_moves = moves;
                }
                if (litters == total_litters) {
                    return min_moves;
                }
            }

            std::vector<std::vector<Optimizer>> optimizers = state.optimizers;
            if (optimizers[y][x].moves != UNEXPLORED && (litters < optimizers[y][x].litters || (litters == optimizers[y][x].litters && moves > optimizers[y][x].moves))) continue;
            optimizers[y][x] = {
                litters, moves
            };

            if (energy == 0) continue;
            energy--;
            moves++;
            states.push_back({ y - 1, x, moves, energy, litters, classroom, optimizers});
            states.push_back({ y + 1, x, moves, energy, litters, classroom, optimizers});
            states.push_back({ y, x - 1, moves, energy, litters, classroom, optimizers});
            states.push_back({ y, x + 1, moves, energy, litters, classroom, optimizers});
        }
        return NO_SOLUTION;
    }
};


int main() {
    {
        std::vector<std::string> classroom = {
            "S.",
            "XL",
        };
        Solution solution = Solution();
        int answer = solution.minMoves(classroom, 2);
        assert (answer == 2);
    }
    {
        std::vector<std::string> classroom = {
            "LS",
            "RL",
        };
        Solution solution = Solution();
        int answer = solution.minMoves(classroom, 4);
        assert (answer == 3);
    }
    {
        std::vector<std::string> classroom = {
            "L.S",
            "RXL",
        };
        Solution solution = Solution();
        int answer = solution.minMoves(classroom, 4);
        assert (answer == 4);
    }
    {
        std::vector<std::string> classroom = {
            "L.S",
            "RXL",
        };
        Solution solution = Solution();
        int answer = solution.minMoves(classroom, 3);
        assert (answer == Solution::NO_SOLUTION);
    }
    {
        std::vector<std::string> classroom = {
            "SR",
        };
        Solution solution = Solution();
        int answer = solution.minMoves(classroom, 1);
        assert (answer == 0);
    }
    {
        std::vector<std::string> classroom = {
            "S",
            "R",
            "L",
            "X",
        };
        Solution solution = Solution();
        int answer = solution.minMoves(classroom, 2);
        assert (answer == 2);
    }
    {
        std::vector<std::string> classroom = {
            "LXX",
            "XL.",
            "RS.",
            "XRR",
        };
        Solution solution = Solution();
        int answer = solution.minMoves(classroom, 8);
        assert (answer == Solution::NO_SOLUTION);
    }
    {
        std::vector<std::string> classroom = {
            "X",
            "L",
            "X",
            "S",
        };
        Solution solution = Solution();
        int answer = solution.minMoves(classroom, 3);
        assert (answer == Solution::NO_SOLUTION);
    }
    {
        std::vector<std::string> classroom = {
            "LS",
            "LX",
            "XL",
        };
        Solution solution = Solution();
        int answer = solution.minMoves(classroom, 2);
        assert (answer == Solution::NO_SOLUTION);
    }
    {
        std::vector<std::string> classroom = {
            "RRX",
            "LRS",
            "R..",
            ".RL",
        };
        Solution solution = Solution();
        int answer = solution.minMoves(classroom, 8);
        assert (answer == 6);
    }
    {
        std::vector<std::string> classroom = {
            "XSRLX",
            "RLR..",
            "R.LXL",
        };
        Solution solution = Solution();
        int answer = solution.minMoves(classroom, 14);
        assert (answer == 9);
    }
    {
        std::vector<std::string> classroom = {
            "LSRXL",
            "RRLRX",
            "XRRL.",
            "X.XRL",
            "RXLXR",
        };
        Solution solution = Solution();
        int answer = solution.minMoves(classroom, 14);
        assert (answer == Solution::NO_SOLUTION);
    }
    {
        std::vector<std::string> classroom = {
            "R.L...",
            "LLLXR.",
            "LXRRS.",
            "..LLXL",
            "XLRX.R",
        };
        Solution solution = Solution();
        int answer = solution.minMoves(classroom, 20);
        assert (answer == 16);
    }
    return 0;
}
