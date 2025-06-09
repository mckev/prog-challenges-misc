// https://leetcode.com/problems/minimum-moves-to-clean-the-classroom/

#include <bitset>
#include <cassert>
#include <deque>
#include <string>
#include <tuple>
#include <vector>


class Solution {
private:
    static const int LITTER = 'L';
    static const int START = 'S';
    static const int OBSTACLE = 'X';
    static const int RESET = 'R';
    static const int EMPTY = '.';

    static std::tuple<int, int> get_start_coord(const std::vector<std::string>& classroom) {
        for (int y = 0; y < classroom.size(); y++) {
            for (int x = 0; x < classroom[y].size(); x++) {
                if (classroom[y][x] == Solution::START) {
                    return {y, x};
                }
            }
        }
        assert (false);
    }

public:
    static const int NO_SOLUTION = -1;
    static const int MAX_LITTERS = 10;

    int minMoves(const std::vector<std::string>& classroom, int full_energy) {
        auto [start_y, start_x] = get_start_coord(classroom);

        // Convert all litter positions into index (0, 1, 2, 3, ...). We will use this later to convert into bits.
        int total_litters;
        std::vector<std::vector<int>> litter_indexes(classroom.size(), std::vector<int>(classroom[0].size(), -1));
        {
            int index = 0;
            for (int y = 0; y < classroom.size(); y++) {
                for (int x = 0; x < classroom[y].size(); x++) {
                    if (classroom[y][x] == Solution::LITTER) {
                        litter_indexes[y][x] = index;
                        index++;
                    }
                }
            }
            total_litters = index;
            assert (total_litters <= MAX_LITTERS);
        }
        if (total_litters == 0) {
            return 0;
        }

        // To avoid BFS from processing previously explored path
        std::vector<std::vector<std::vector<int>>> optimizers(classroom.size(), std::vector<std::vector<int>>(classroom[0].size(), std::vector<int>(1 << MAX_LITTERS)));

        // BFS
        struct State {
            int y, x;
            int moves;
            int energy;
            int litters;
            std::bitset<MAX_LITTERS> explored;
        };
        std::deque<State> states;
        states.push_back({ start_y, start_x, 0, full_energy, 0, {} });
        while (! states.empty()) {
            State state = states.front(); states.pop_front();
            int y = state.y;
            int x = state.x;
            int moves = state.moves;
            int energy = state.energy;
            int litters = state.litters;
            std::bitset<MAX_LITTERS> explored = state.explored;
            if (y < 0 || y >= classroom.size()) continue;
            if (x < 0 || x >= classroom[y].size()) continue;
            if (classroom[y][x] == OBSTACLE) {
                continue;
            } else if (classroom[y][x] == RESET) {
                energy = full_energy;
            } else if (classroom[y][x] == LITTER && !explored[litter_indexes[y][x]]) {
                litters++;
                if (litters == total_litters) {
                    return moves;
                }
                explored[litter_indexes[y][x]] = true;
            }

            // Hint #2: Maintain a 3D array bestEnergy[x][y][mask] storing the maximum e seen for each (x,y,mask) and skip any new state with e <= bestEnergy[x][y][mask] to prune
            if (energy <= optimizers[y][x][explored.to_ulong()]) {
                continue;
            }
            optimizers[y][x][explored.to_ulong()] = energy;

            if (energy == 0) continue;
            energy--;
            moves++;
            states.push_back({ y - 1, x, moves, energy, litters, explored });
            states.push_back({ y + 1, x, moves, energy, litters, explored });
            states.push_back({ y, x - 1, moves, energy, litters, explored });
            states.push_back({ y, x + 1, moves, energy, litters, explored });
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
    {
        std::vector<std::string> classroom = {
            "..LR",
            "RL.X",
            "..L.",
            "X..S",
        };
        Solution solution = Solution();
        int answer = solution.minMoves(classroom, 5);
        assert (answer == 8);
    }
    {
        std::vector<std::string> classroom = {
            "LRLRXX",
            "R.R.LL",
            "L...R.",
            ".RL.XL",
            "X.XRRR",
            ".L.RXL",
            "...SLX",
            ".RXX.R",
            "..XR..",
        };
        Solution solution = Solution();
        int answer = solution.minMoves(classroom, 10);
        assert (answer == 23);
    }
    {
        std::vector<std::string> classroom = {
            "S.RXL.XRR.",
            "R.XRRRRX.X",
            "LX..XL..RR",
            "RXL.L.RLX.",
            "RR.L.XRXLL",
            "L.RRXXRRRX",
            "RXRRXX.RXX",
            "XXXRX..XR.",
            "X.XR.XXR.R",
        };
        Solution solution = Solution();
        int answer = solution.minMoves(classroom, 7);
        assert (answer == 30);
    }
    {
        std::vector<std::string> classroom = {
            "LRRX.LXR.LX",
            ".RLRRLXRXX.",
            "L..RRXLXXR.",
            "X.R.RRXRXX.",
            "XSRXXRRXLL.",
            "RRXR.RXRRRX",
            "RRRX.XXRR.X",
            "XX.R....XRR",
            "RXXXXXXR..R",
            "X.RXRX.RX.R",
            ".XX.X.X..XR",
        };
        Solution solution = Solution();
        int answer = solution.minMoves(classroom, 8);
        assert (answer == -1);
    }
    {
        std::vector<std::string> classroom = {
            ".XR.X.RR.XR.XX.XXXRR",
            "R..RRL.RXXXRXRXXXRX.",
            "RRRR.RRXX.X.RX.R..XX",
            "R...R.RX.L..RRL..R.L",
            ".X..RX.XRRRX..X.R.R.",
            "R.XRXRXX..R.R..R.X.R",
            "R.X.RX.RR..X.X.RL.R.",
            "LXRX.RRXXRRLRXRX.RXR",
            "X.XR.RRR..RXX.X.XRXR",
            "XXXR..XRXRR.RR..RX.R",
            "RR..XXRR..XXX.X..R.R",
            "RR.RRR.X.RRRX...XRRR",
            "R...S.XXLX.XRRX.XRRX",
            "X..X.X.RXRX.X.XXXR.R",
            "R.LXRR.RX.XR.RRXX.RX",
            "XX.XR.R.R.XR.X.R..RR",
            "..XXR.R..RXX.R..RRXX",
            ".XR.R....XR.R.XX..RX",
            "XXRRRRXXXRRX.RXLRXXR",
            "X.XXXXRRXR.RXRXRXX.R",
        };
        Solution solution = Solution();
        int answer = solution.minMoves(classroom, 11);
        assert (answer == 103);
    }
    return 0;
}