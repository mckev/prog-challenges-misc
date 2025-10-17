// https://www.facebook.com/codingcompetitions/hacker-cup/2025/practice-round/problems/E

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <set>
#include <vector>


const int UNDEFINED = -1;
const bool DEBUG = false;


class SimulatorBruteforce {
private:
    int N;                          // number of robots
    int L;                          // length of the line
    struct Robot {
        int id;                     // robot id starts at 1
        int pos;
        int prev_pos;               // this is used to detect collision in middle. By the end of every step(), it will be equal pos
        bool direction;             // false: left, true: right
        int last_robot_id_to_pay;
    };
    std::vector<Robot> initial_robots;
    std::map<int, int> robot_ids;   // to efficiently find robots within boundary wall

    std::set<int> walls;
    std::vector<Robot> robots;

public:
    SimulatorBruteforce(int N, int L, const std::vector<int>& initial_robots_pos) {
        this->N = N;
        this->L = L;
        add_wall(1);
        add_wall(L);

        assert(initial_robots_pos.size() == N);
        for (int i = 0; i < N; i++) {
            int id = i + 1; assert (1 <= id); assert (id <= N);
            int initial_pos = initial_robots_pos.at(i) - 1; assert (0 <= initial_pos); assert (initial_pos < L);
            Robot robot = { id, initial_pos, initial_pos, false, 0 };
            initial_robots.push_back(robot);
            robot_ids[initial_pos] = robot.id;
        }
        reset_robots();
    }

    void reset_robots() {
        robots = initial_robots;
    }

    void print() const {
        for (int i = 0; i < L; i++) {
            std::cout << "[" << i << "] ";
            for (const Robot& robot : robots) {
                if (robot.pos == i) {
                    std::cout << robot.id << " ";
                }
            }
            std::cout << "  ";
        }
        std::cout << "  ";
        for (const Robot& robot : robots) {
            std::cout << robot.id << " --> " << robot.last_robot_id_to_pay << "  ";
        }
        std::cout << std::endl;
    }

    void add_wall(int x) {
        assert (1 <= x); assert (x <= L);
        walls.insert(x - 1);
    }

    void step(const std::vector<int>& robot_ids) {
        // Simulate 1 step
        // Note: robot_ids must be sorted ascendingly
        for (int robot_id : robot_ids) {
            Robot& robot = robots.at(robot_id - 1);
            if (robot.direction) {
                robot.pos++;
                assert(robot.pos < L);
            } else {
                robot.pos--;
                assert(robot.pos >= 0);
            }
            bool is_robot_hits_wall = walls.find(robot.pos) != walls.end();
            if (is_robot_hits_wall) {
                robot.direction = ! robot.direction;
            }
        }

        // Detect collision with other robots.
        //   - Mark the robot with higher id, that it collides with.
        for (int robot_id : robot_ids) {
            Robot& robot = robots.at(robot_id - 1);
            for (int other_robot_id : robot_ids) {
                const Robot& other_robot = robots.at(other_robot_id - 1);
                if (other_robot.id <= robot.id) continue;
                // Detect collision at pos
                if (robot.pos == other_robot.pos) {
                    robot.last_robot_id_to_pay = other_robot.id;
                    break;
                }
                // Detect collision in middle
                if (robot.pos == other_robot.prev_pos && robot.prev_pos == other_robot.pos) {
                    robot.last_robot_id_to_pay = other_robot.id;
                }
            }
        }

        for (Robot& robot : robots) {
            robot.prev_pos = robot.pos;
        }
    }

    static std::pair<int, int> find_boundary_walls(const std::set<int>& walls, int pos) {
        auto right_it = walls.upper_bound(pos);
        if (right_it == walls.end()) {
            //  Handle edge case when robot pos is at L - 1
            right_it--;
        }
        int right = *right_it;
        auto left_it = std::prev(right_it); int left = *left_it;
        assert(left <= pos); assert(pos <= right);
        std::pair<int, int> result = { left, right };
        return result;
    }

    std::vector<int> get_robot_ids_within_same_boundary_walls(const Robot& robot, const std::pair<int, int>& boundary_walls) const {
        // Note: Outputs are not necessarily sorted!
        std::vector<int> result;
        for (auto it = robot_ids.lower_bound(boundary_walls.first); it != robot_ids.lower_bound(boundary_walls.second); it++) {
            int robot_id = (*it).second;
            result.push_back(robot_id);
        }
        assert(result.size() > 0);
        return result;
    }

    int query(int robot_id, int T) {
        reset_robots();
        // print();
        const Robot& robot = robots.at(robot_id - 1);
        std::pair<int, int> boundary_walls = SimulatorBruteforce::find_boundary_walls(walls, robot.pos);
        std::vector<int> robot_ids = get_robot_ids_within_same_boundary_walls(robot, boundary_walls);
        std::sort(robot_ids.begin(), robot_ids.end());
        for (int t = 0; t < T; t++) {
            step(robot_ids);
            // print();
        }
        return robot.last_robot_id_to_pay;
    }
};




class SimulatorEfficient {
public:
    struct Robot {
        int id;                     // robot id starts at 1
        int initial_pos;
    };
    std::set<int> walls;

private:
    int N;                          // number of robots
    int L;                          // length of the line
    std::vector<Robot> robots;
    std::map<int, int> robot_ids;   // to efficiently find robots within boundary wall

public:
    SimulatorEfficient(int N, int L, const std::vector<int>& initial_robots_pos) {
        this->N = N;
        this->L = L;
        add_wall(1);
        add_wall(L);

        assert(initial_robots_pos.size() == N);
        for (int i = 0; i < N; i++) {
            int id = i + 1; assert (1 <= id); assert (id <= N);
            int initial_pos = initial_robots_pos.at(i) - 1; assert (0 <= initial_pos); assert (initial_pos < L);
            Robot robot = { i + 1, initial_pos };
            robots.push_back(robot);
            robot_ids[initial_pos] = robot.id;
        }
    }

    const Robot& get_robot(int id) const {
        return robots.at(id - 1);
    }

    void add_wall(int x) {
        assert (1 <= x); assert (x <= L);
        walls.insert(x - 1);
    }

    static int compute_robot_pos_at_t(int initial_pos, int t, const std::pair<int, int>& boundary_walls) {
        int left = boundary_walls.first;
        int right = boundary_walls.second;
        int width = right - left;
        int period = 2 * width;
        int offset = (initial_pos - left - t) % period;
        if (offset < 0) offset = period + offset;
        if (offset > width) offset = period - offset;
        int pos = offset + left;
        assert(left <= pos); assert(pos <= right);
        return pos;
    }

    static double find_last_projected_collision_time(int initial_pos1, int initial_pos2, int t, std::pair<int, int> boundary_walls) {
        // Find the last collision time between robot and the other robot
        double last_projected_collision_time = UNDEFINED;
        int left = boundary_walls.first;
        int right = boundary_walls.second;
        int width = right - left;
        int period = 2 * width;
        int prev_pos1 = UNDEFINED, prev_pos2 = UNDEFINED;                                           // To detect collision in middle
        for (long long i = 0; i < period; i++) {
            assert(0 <= i); assert(i < period);
            int pos1 = SimulatorEfficient::compute_robot_pos_at_t(initial_pos1, i, boundary_walls);
            int pos2 = SimulatorEfficient::compute_robot_pos_at_t(initial_pos2, i, boundary_walls);
            // Detect collision at pos
            if (pos1 == pos2) {
                double projected_collision_time = t - ((t - i) % period);
                if (projected_collision_time <= t) {
                    last_projected_collision_time = std::max(last_projected_collision_time, projected_collision_time);
                }
            }
            // Detect collision in middle
            if (std::abs(pos1 - pos2) == 1 && pos1 == prev_pos2 && pos2 == prev_pos1) {
                double projected_collision_time = t - ((t - i) % period) - 0.5;                     // We need to subtract by 0.5!
                if (projected_collision_time <= t) {
                    last_projected_collision_time = std::max(last_projected_collision_time, projected_collision_time);
                }
            }
            // Optimization: As pos1 and pos2 are decremented / incremented by one, we can jump
            if (prev_pos1 != UNDEFINED && prev_pos2 != UNDEFINED) {
                bool pos1_direction = pos1 > prev_pos1;
                bool pos2_direction = pos2 > prev_pos2;
                if (pos1_direction == pos2_direction) {
                    int jump = std::min(std::min(std::min(right - pos1, right - pos2), pos1 - left), pos2 - left) - 1;
                    if (jump >= 0) i += jump;
                } else {
                    if (pos1 < pos2 && pos1_direction == true || pos2 < pos1 && pos2_direction == true) {
                        int jump = std::abs((pos2 - pos1) / 2) - 1;
                        if (jump >= 0) i += jump;
                    } else {
                        int jump = std::min(std::min(std::min(right - pos1, right - pos2), pos1 - left), pos2 - left) - 1;
                        if (jump >= 0) i += jump;
                    }
                }
            }
            prev_pos1 = pos1;
            prev_pos2 = pos2;
        }
        return last_projected_collision_time;
    }

    std::vector<int> get_robot_ids_within_same_boundary_walls(const Robot& robot, const std::pair<int, int>& boundary_walls) const {
        std::vector<int> result;
        for (auto it = robot_ids.lower_bound(boundary_walls.first); it != robot_ids.lower_bound(boundary_walls.second); it++) {
            int robot_id = (*it).second;
            result.push_back(robot_id);
        }
        assert(result.size() > 0);
        return result;
    }

    int query(int robot_id, int t) {
        Robot robot = get_robot(robot_id);
        std::pair<int, int> boundary_walls = SimulatorBruteforce::find_boundary_walls(walls, robot.initial_pos);
        double last_robot_collision_time = UNDEFINED;
        int last_robot_collision_id = UNDEFINED;
        std::vector<int> robot_ids = get_robot_ids_within_same_boundary_walls(robot, boundary_walls);
        if (robot_ids.size() == 1) {
            // If we are the only one in the group
            return 0;
        }
        for (int other_robot_id : robot_ids) {
            if (other_robot_id <= robot.id) continue;
            Robot other_robot = get_robot(other_robot_id);
            double last_projected_collision_time = find_last_projected_collision_time(robot.initial_pos, other_robot.initial_pos, t, boundary_walls);
            // Last robot collided
            if ((last_projected_collision_time > last_robot_collision_time) || (last_projected_collision_time == last_robot_collision_time && other_robot.id > last_robot_collision_id)) {
                last_robot_collision_time = last_projected_collision_time;
                last_robot_collision_id = other_robot.id;
            }
        }
        if (last_robot_collision_time == UNDEFINED) {
            return 0;
        }
        assert (last_robot_collision_id >= 1);
        return last_robot_collision_id;
    }
};




void unit_test() {
    {
        std::set<int> walls;
        walls.insert(0);
        int L = 17; walls.insert(L - 1);
        walls.insert(10 - 1);

        std::pair<int, int> boundary_walls;
        boundary_walls = SimulatorBruteforce::find_boundary_walls(walls, 3);
        assert(SimulatorEfficient::compute_robot_pos_at_t(3, 0, boundary_walls) == 3);
        assert(SimulatorEfficient::compute_robot_pos_at_t(3, 1, boundary_walls) == 2);
        assert(SimulatorEfficient::compute_robot_pos_at_t(3, 2, boundary_walls) == 1);
        assert(SimulatorEfficient::compute_robot_pos_at_t(3, 3, boundary_walls) == 0);
        assert(SimulatorEfficient::compute_robot_pos_at_t(3, 4, boundary_walls) == 1);
        assert(SimulatorEfficient::compute_robot_pos_at_t(3, 5, boundary_walls) == 2);
        assert(SimulatorEfficient::compute_robot_pos_at_t(3, 11, boundary_walls) == 8);
        assert(SimulatorEfficient::compute_robot_pos_at_t(3, 12, boundary_walls) == 9);
        assert(SimulatorEfficient::compute_robot_pos_at_t(3, 13, boundary_walls) == 8);
        assert(SimulatorEfficient::compute_robot_pos_at_t(3, 14, boundary_walls) == 7);
        assert(SimulatorEfficient::compute_robot_pos_at_t(3, 1000000, boundary_walls) == 7);

        boundary_walls = SimulatorBruteforce::find_boundary_walls(walls, 0);
        assert(SimulatorEfficient::compute_robot_pos_at_t(0, 0, boundary_walls) == 0);
        assert(SimulatorEfficient::compute_robot_pos_at_t(0, 1, boundary_walls) == 1);
        assert(SimulatorEfficient::compute_robot_pos_at_t(0, 2, boundary_walls) == 2);
        assert(SimulatorEfficient::compute_robot_pos_at_t(0, 7, boundary_walls) == 7);
        assert(SimulatorEfficient::compute_robot_pos_at_t(0, 8, boundary_walls) == 8);
        assert(SimulatorEfficient::compute_robot_pos_at_t(0, 9, boundary_walls) == 9);
        assert(SimulatorEfficient::compute_robot_pos_at_t(0, 10, boundary_walls) == 8);
        assert(SimulatorEfficient::compute_robot_pos_at_t(0, 11, boundary_walls) == 7);
        assert(SimulatorEfficient::compute_robot_pos_at_t(0, 1000000, boundary_walls) == 8);

        boundary_walls = SimulatorBruteforce::find_boundary_walls(walls, 9);
        assert(SimulatorEfficient::compute_robot_pos_at_t(9, 0, boundary_walls) == 9);
        assert(SimulatorEfficient::compute_robot_pos_at_t(9, 1, boundary_walls) == 10);
        assert(SimulatorEfficient::compute_robot_pos_at_t(9, 1000000, boundary_walls) == 15);

        boundary_walls = SimulatorBruteforce::find_boundary_walls(walls, 12);
        assert(SimulatorEfficient::compute_robot_pos_at_t(12, 0, boundary_walls) == 12);
        assert(SimulatorEfficient::compute_robot_pos_at_t(12, 1, boundary_walls) == 11);
        assert(SimulatorEfficient::compute_robot_pos_at_t(12, 2, boundary_walls) == 10);
        assert(SimulatorEfficient::compute_robot_pos_at_t(12, 3, boundary_walls) == 9);
        assert(SimulatorEfficient::compute_robot_pos_at_t(12, 4, boundary_walls) == 10);
        assert(SimulatorEfficient::compute_robot_pos_at_t(12, 5, boundary_walls) == 11);
        assert(SimulatorEfficient::compute_robot_pos_at_t(12, 9, boundary_walls) == 15);
        assert(SimulatorEfficient::compute_robot_pos_at_t(12, 10, boundary_walls) == 16);
        assert(SimulatorEfficient::compute_robot_pos_at_t(12, 11, boundary_walls) == 15);
        assert(SimulatorEfficient::compute_robot_pos_at_t(12, 12, boundary_walls) == 14);
        assert(SimulatorEfficient::compute_robot_pos_at_t(12, 1000000, boundary_walls) == 14);

        boundary_walls = SimulatorBruteforce::find_boundary_walls(walls, 16);
        assert(SimulatorEfficient::compute_robot_pos_at_t(16, 0, boundary_walls) == 16);
        assert(SimulatorEfficient::compute_robot_pos_at_t(16, 1, boundary_walls) == 15);
        assert(SimulatorEfficient::compute_robot_pos_at_t(16, 7, boundary_walls) == 9);
        assert(SimulatorEfficient::compute_robot_pos_at_t(16, 8, boundary_walls) == 10);
        assert(SimulatorEfficient::compute_robot_pos_at_t(16, 1000000, boundary_walls) == 10);
    }
    {
        int N = 3;
        int L = 17;
        std::vector<int> initial_robots_pos = { 3, 7, 13 };
        SimulatorEfficient simulator_efficient = SimulatorEfficient(N, L, initial_robots_pos);
        {
            SimulatorEfficient::Robot robot = simulator_efficient.get_robot(1);
            std::pair<int, int> boundary_walls = SimulatorBruteforce::find_boundary_walls(simulator_efficient.walls, robot.initial_pos);
            std::vector<int> robot_ids = simulator_efficient.get_robot_ids_within_same_boundary_walls(robot, boundary_walls);
            std::vector<int> expected = { 1, 2, 3 };
            assert(robot_ids == expected);
        }
        {
            SimulatorEfficient::Robot robot = simulator_efficient.get_robot(2);
            std::pair<int, int> boundary_walls = SimulatorBruteforce::find_boundary_walls(simulator_efficient.walls, robot.initial_pos);
            std::vector<int> robot_ids = simulator_efficient.get_robot_ids_within_same_boundary_walls(robot, boundary_walls);
            std::vector<int> expected = { 1, 2, 3 };
            assert(robot_ids == expected);
        }
        {
            SimulatorEfficient::Robot robot = simulator_efficient.get_robot(3);
            std::pair<int, int> boundary_walls = SimulatorBruteforce::find_boundary_walls(simulator_efficient.walls, robot.initial_pos);
            std::vector<int> robot_ids = simulator_efficient.get_robot_ids_within_same_boundary_walls(robot, boundary_walls);
            std::vector<int> expected = { 1, 2, 3 };
            assert(robot_ids == expected);
        }
        simulator_efficient.add_wall(10);
        {
            SimulatorEfficient::Robot robot = simulator_efficient.get_robot(1);
            std::pair<int, int> boundary_walls = SimulatorBruteforce::find_boundary_walls(simulator_efficient.walls, robot.initial_pos);
            std::vector<int> robot_ids = simulator_efficient.get_robot_ids_within_same_boundary_walls(robot, boundary_walls);
            std::vector<int> expected = { 1, 2 };
            assert(robot_ids == expected);
        }
        {
            SimulatorEfficient::Robot robot = simulator_efficient.get_robot(2);
            std::pair<int, int> boundary_walls = SimulatorBruteforce::find_boundary_walls(simulator_efficient.walls, robot.initial_pos);
            std::vector<int> robot_ids = simulator_efficient.get_robot_ids_within_same_boundary_walls(robot, boundary_walls);
            std::vector<int> expected = { 1, 2 };
            assert(robot_ids == expected);
        }
        {
            SimulatorEfficient::Robot robot = simulator_efficient.get_robot(3);
            std::pair<int, int> boundary_walls = SimulatorBruteforce::find_boundary_walls(simulator_efficient.walls, robot.initial_pos);
            std::vector<int> robot_ids = simulator_efficient.get_robot_ids_within_same_boundary_walls(robot, boundary_walls);
            std::vector<int> expected = { 3 };
            assert(robot_ids == expected);
        }
    }
}


void test() {
    {
        int N = 3;
        int L = 17;
        std::vector<int> initial_robots_pos = { 3, 7, 13 };
        SimulatorBruteforce simulator_bruteforce = SimulatorBruteforce(N, L, initial_robots_pos);
        SimulatorEfficient simulator_efficient = SimulatorEfficient(N, L, initial_robots_pos);
        simulator_bruteforce.add_wall(10);
        simulator_efficient.add_wall(10);
        int answer_bruteforce, answer_efficient;
        answer_bruteforce = simulator_bruteforce.query(1, 4);
        answer_efficient = simulator_efficient.query(1, 4);
        assert(answer_bruteforce == 2);
        assert(answer_efficient == answer_bruteforce);
        answer_bruteforce = simulator_bruteforce.query(3, 6);
        answer_efficient = simulator_efficient.query(3, 6);
        assert(answer_bruteforce == 0);
        assert(answer_efficient == answer_bruteforce);
    }
    {
        int N = 2;
        int L = 6;
        std::vector<int> initial_robots_pos = { 3, 4 };
        SimulatorBruteforce simulator_bruteforce = SimulatorBruteforce(N, L, initial_robots_pos);
        SimulatorEfficient simulator_efficient = SimulatorEfficient(N, L, initial_robots_pos);
        simulator_bruteforce.add_wall(2);
        simulator_efficient.add_wall(2);
        int answer_bruteforce, answer_efficient;
        answer_bruteforce = simulator_bruteforce.query(1, 1);
        answer_efficient = simulator_efficient.query(1, 1);
        assert(answer_bruteforce == 0);
        assert(answer_efficient == answer_bruteforce);
        answer_bruteforce = simulator_bruteforce.query(1, 35);
        answer_efficient = simulator_efficient.query(1, 35);
        assert(answer_bruteforce == 2);
        assert(answer_efficient == answer_bruteforce);
    }
    {
        int N = 2;
        int L = 17;
        std::vector<int> initial_robots_pos = { 2, 14 };
        SimulatorBruteforce simulator_bruteforce = SimulatorBruteforce(N, L, initial_robots_pos);
        SimulatorEfficient simulator_efficient = SimulatorEfficient(N, L, initial_robots_pos);
        simulator_bruteforce.add_wall(16);
        simulator_efficient.add_wall(16);
        int answer_bruteforce, answer_efficient;
        answer_bruteforce = simulator_bruteforce.query(1, 70);
        answer_efficient = simulator_efficient.query(1, 70);
        assert(answer_bruteforce == 2);
        assert(answer_efficient == answer_bruteforce);
        simulator_bruteforce.add_wall(11);
        simulator_efficient.add_wall(11);
        answer_bruteforce = simulator_bruteforce.query(1, 78);
        answer_efficient = simulator_efficient.query(1, 78);
        assert(answer_bruteforce == 0);
        assert(answer_efficient == answer_bruteforce);
    }
    {
        int N = 6;
        int L = 10;
        std::vector<int> initial_robots_pos = { 3, 8, 2, 4, 5, 6 };
        SimulatorBruteforce simulator_bruteforce = SimulatorBruteforce(N, L, initial_robots_pos);
        SimulatorEfficient simulator_efficient = SimulatorEfficient(N, L, initial_robots_pos);
        int answer_bruteforce, answer_efficient;
        answer_bruteforce = simulator_bruteforce.query(1, 4);
        answer_efficient = simulator_efficient.query(1, 4);
        assert(answer_bruteforce == 6);
        assert(answer_efficient == answer_bruteforce);
        answer_bruteforce = simulator_bruteforce.query(1, 5);
        answer_efficient = simulator_efficient.query(1, 5);
        assert(answer_bruteforce == 2);
        assert(answer_efficient == answer_bruteforce);
        answer_bruteforce = simulator_bruteforce.query(2, 100);
        answer_efficient = simulator_efficient.query(2, 100);
        assert(answer_bruteforce == 6);
        assert(answer_efficient == answer_bruteforce);
        answer_bruteforce = simulator_bruteforce.query(1, 100);
        answer_efficient = simulator_efficient.query(1, 100);
        assert(answer_bruteforce == 2);
        assert(answer_efficient == answer_bruteforce);
    }
    {
        int N = 48;
        int L = 68;
        std::vector<int> initial_robots_pos = { 32, 37, 42, 47, 52, 57, 62, 67, 6, 11, 16, 21, 26, 31, 36, 41, 46, 51, 56, 61, 66, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 4, 9, 14, 19, 24, 29, 34, 39, 44, 49, 54, 59, 64, 3 };
        SimulatorBruteforce simulator_bruteforce = SimulatorBruteforce(N, L, initial_robots_pos);
        SimulatorEfficient simulator_efficient = SimulatorEfficient(N, L, initial_robots_pos);
        simulator_bruteforce.add_wall(8); simulator_efficient.add_wall(8);
        simulator_bruteforce.add_wall(13); simulator_efficient.add_wall(13);
        simulator_bruteforce.add_wall(18); simulator_efficient.add_wall(18);
        simulator_bruteforce.add_wall(23); simulator_efficient.add_wall(23);
        simulator_bruteforce.add_wall(28); simulator_efficient.add_wall(28);
        simulator_bruteforce.add_wall(33); simulator_efficient.add_wall(33);
        simulator_bruteforce.add_wall(38); simulator_efficient.add_wall(38);
        simulator_bruteforce.add_wall(43); simulator_efficient.add_wall(43);
        simulator_bruteforce.add_wall(48); simulator_efficient.add_wall(48);
        simulator_bruteforce.add_wall(53); simulator_efficient.add_wall(53);
        simulator_bruteforce.add_wall(58); simulator_efficient.add_wall(58);
        simulator_bruteforce.add_wall(63); simulator_efficient.add_wall(63);
        simulator_bruteforce.add_wall(2); simulator_efficient.add_wall(2);
        simulator_bruteforce.add_wall(7); simulator_efficient.add_wall(7);
        simulator_bruteforce.add_wall(12); simulator_efficient.add_wall(12);
        simulator_bruteforce.add_wall(17); simulator_efficient.add_wall(17);
        simulator_bruteforce.add_wall(22); simulator_efficient.add_wall(22);
        simulator_bruteforce.add_wall(27); simulator_efficient.add_wall(27);
        int answer_bruteforce, answer_efficient;
        // t = 0: Wall  -  Robot #40  -  Robot #27  -  Robot #14  -  Robot #1  -  Wall
        // t = 1: Robot #40  -  Robot #27  -  Robot #14  -  Robot #1  -  Empty  -  Wall
        // t = 2: Robot #27  -  Robot #40, Robot #14  -  Robot #1  -  Empty  -  Empty  -  Wall
        // t = 3: Robot #14  -  Robot #27, Robot #1  -  Robot #40  -  Empty  -  Empty  -  Wall
        // Robot 1 actually hits Robot 40 and Robot 27. But it hits Robot 27 at last.
        answer_bruteforce = simulator_bruteforce.query(1, 3);
        answer_efficient = simulator_efficient.query(1, 3);
        assert(answer_bruteforce == 27);
        assert(answer_efficient == answer_bruteforce);
    }
}


void stress_test() {
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<> rnd(0, 1);                     // [0, 1)
        for (int T = 0; T < 100; T++) {
            int L = int(rnd(mt) * 1000) + 3;                            // [3, 1003]
            int N = int(rnd(mt) * 10) + 1;
            std::vector<int> initial_robots_pos;
            std::map<int, char> line;
            for (int n = 0; n < N; n++) {
                int x = int(rnd(mt) * (L - 2)) + 2;                     // [2, L - 1]
                assert(2 <= x); assert(x <= L - 1);
                bool is_exist = line.find(x) != line.end(); if (is_exist) continue; line[x] = 'R';              // No two robots start at the same position
                initial_robots_pos.push_back(x);
            }
            N = initial_robots_pos.size();
            assert(N >= 1);
            SimulatorBruteforce simulator_bruteforce = SimulatorBruteforce(N, L, initial_robots_pos);
            SimulatorEfficient simulator_efficient = SimulatorEfficient(N, L, initial_robots_pos);
            for (int q = 0; q < 100; q++) {
                if (int(rnd(mt) < 0.1)) {
                    int x = int(rnd(mt) * (L - 2)) + 2;                 // [2, L - 1]
                    bool is_exist = line.find(x) != line.end(); if (is_exist) continue; line[x] = 'W';          // No robot starts at the same position as a wall
                    simulator_bruteforce.add_wall(x);
                    simulator_efficient.add_wall(x);
                } else {
                    int r = int(rnd(mt) * N) + 1;                       // [1, N]
                    int s = int(rnd(mt) * 1000) + 1;
                    int answer_bruteforce = simulator_bruteforce.query(r, s);
                    int answer_efficient = simulator_efficient.query(r, s);
                    assert(answer_efficient == answer_bruteforce);
                }
            }
        }
    }
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<> rnd(0, 1);                     // [0, 1)
        for (int T = 0; T < 1; T++) {
            int L = int(rnd(mt) * 1000000000) + 3;
            int N = int(rnd(mt) * 200000) + 1;
            std::vector<int> initial_robots_pos;
            std::map<int, char> line;
            for (int n = 0; n < N; n++) {
                int x = int(rnd(mt) * (L - 2)) + 2;                     // [2, L - 1]
                assert(2 <= x); assert(x <= L - 1);
                bool is_exist = line.find(x) != line.end(); if (is_exist) continue; line[x] = 'R';              // No two robots start at the same position
                initial_robots_pos.push_back(x);
            }
            N = initial_robots_pos.size();
            assert(N >= 1);
            SimulatorEfficient simulator_efficient = SimulatorEfficient(N, L, initial_robots_pos);
            for (int q = 0; q < 100; q++) {
                if (int(rnd(mt) < 0.1)) {
                    int x = int(rnd(mt) * (L - 2)) + 2;                 // [2, L - 1]
                    bool is_exist = line.find(x) != line.end(); if (is_exist) continue; line[x] = 'W';          // No robot starts at the same position as a wall
                    simulator_efficient.add_wall(x);
                } else {
                    int r = int(rnd(mt) * N) + 1;                       // [1, N]
                    int s = int(rnd(mt) * 1000000000) + 1;
                    int answer_efficient = simulator_efficient.query(r, s);
                }
            }
        }
    }
}


void solve(std::string filename) {
    std::istream* input = &std::cin;
    std::ifstream file;
    if (filename != "") {
        file.open("pay_off_input.txt");
        if (!file.is_open()) {
            std::cerr << "Failed to open input file" << std::endl;
            return;
        }
        input = &file;
    }
    int T; *input >> T;
    for (int t = 0; t < T; t++) {
        int N; *input >> N;
        int Q; *input >> Q;
        int L; *input >> L;
        if (DEBUG) {
            std::cout << "Case #" << t + 1 << std::endl;
            std::cout << N << " " << Q << " " << L << std::endl;
        }
        std::vector<int> initial_robots_pos;
        for (int n = 0; n < N; n++) {
            int x; *input >> x;
            if (DEBUG) std::cout << x << " ";
            initial_robots_pos.push_back(x);
        }
        if (DEBUG) std::cout << std::endl;
        SimulatorEfficient simulator_efficient = SimulatorEfficient(N, L, initial_robots_pos);
        SimulatorBruteforce simulator_bruteforce = SimulatorBruteforce(N, L, initial_robots_pos);
        long long answers = 0;
        for (int q = 0; q < Q; q++) {
            int type; *input >> type;
            if (type == 1) {
                int x; *input >> x;
                if (DEBUG) std::cout << 1 << " " << x << std::endl;
                simulator_bruteforce.add_wall(x);
                simulator_efficient.add_wall(x);
            } else if (type == 2) {
                int r; *input >> r;
                int s; *input >> s;
                if (DEBUG) std::cout << 2 << " " << r << " " << s << std::endl;
                int answer = simulator_efficient.query(r, s);
                answers += answer;
            } else {
                assert(false);
            }
        }
        std::cout << "Case #" << t + 1 << ": " << answers << std::endl;
    }
}


int main() {
    unit_test();
    test();
    stress_test();

    solve("pay_off_input.txt");
    return 0;
}
