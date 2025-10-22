// https://www.facebook.com/codingcompetitions/hacker-cup/2025/round-1/problems/A2

#include <cassert>
#include <iostream>
#include <queue>
#include <vector>


const int UNDEFINED = -1;


int solve(const std::vector<int>& A) {
    int answer = UNDEFINED;
    int N = A.size();
    // Platforms that have been solved
    int num_solved = 0;
    std::vector<bool> path_to_ground(N, false);
    // Priority queue
    struct Queue {
        int ladder_height;
        int pos;
        bool operator<(const Queue& other) const {
            return ladder_height > other.ladder_height;
        }
    };
    std::priority_queue<Queue> queues;
    for (int pos = 0; pos < N; pos++) {
        // There are three ways to reach this platform
        if (pos - 1 >= 0)
            queues.push({ std::abs(A.at(pos) - A.at(pos - 1)), pos });
        if (pos + 1 < N)
            queues.push({ std::abs(A.at(pos) - A.at(pos + 1)), pos });
        queues.push({ A.at(pos), pos });
    }
    while (num_solved < N) {
        Queue queue = queues.top(); queues.pop();
        if (path_to_ground.at(queue.pos)) continue;

        // Update answer
        answer = queue.ladder_height;

        // See if we can have a path to the ground
        bool is_path_to_ground = false;
        if (queue.ladder_height >= A.at(queue.pos)) {
            is_path_to_ground = true;
        }
        if (! is_path_to_ground) {
            for (int i = queue.pos + 1; i < N; i++) {
                if (std::abs(A.at(i) - A.at(i - 1)) > queue.ladder_height) break;
                if (path_to_ground.at(i)) {
                    is_path_to_ground = true;
                    break;
                }
            }
        }
        if (! is_path_to_ground) {
            for (int i = queue.pos - 1; i >= 0; i--) {
                if (std::abs(A.at(i) - A.at(i + 1)) > queue.ladder_height) break;
                if (path_to_ground.at(i)) {
                    is_path_to_ground = true;
                    break;
                }
            }
        }

        // If we have path to the ground, then mark all reachable platforms from starting point "pos" as solved
        if (is_path_to_ground) {
            if (! path_to_ground.at(queue.pos)) { path_to_ground.at(queue.pos) = true; num_solved++; }
            for (int i = queue.pos + 1; i < N; i++) {
                if (std::abs(A.at(i) - A.at(i - 1)) > queue.ladder_height) break;
                if (! path_to_ground.at(i)) { path_to_ground.at(i) = true; num_solved++; }
            }
            for (int i = queue.pos - 1; i >= 0; i--) {
                if (std::abs(A.at(i) - A.at(i + 1)) > queue.ladder_height) break;
                if (! path_to_ground.at(i)) { path_to_ground.at(i) = true; num_solved++; }
            }
        }
    }
    assert(answer != UNDEFINED);
    return answer;
}


void test() {
    {
        std::vector<int> A = { 2, 4, 5, 1, 4 };
        int answer = solve(A);
        assert(answer == 3);
    }
    {
        std::vector<int> A = { 13, 10, 11 };
        int answer = solve(A);
        assert(answer == 10);
    }
    {
        std::vector<int> A = { 1, 3, 3, 7 };
        int answer = solve(A);
        assert(answer == 4);
    }
    {
        std::vector<int> A = { 42 };
        int answer = solve(A);
        assert(answer == 42);
    }
    {
        std::vector<int> A = { 5, 50, 42 };
        int answer = solve(A);
        assert(answer == 42);
    }
    {
        std::vector<int> A = { 4, 2, 5, 6, 4, 2, 1 };
        int answer = solve(A);
        assert(answer == 2);
    }
}


int main() {
    test();
    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        int N; std::cin >> N;
        std::vector<int> A;
        for (int n = 0; n < N; n++) {
            int a; std::cin >> a;
            A.push_back(a);
        }
        int answer = solve(A);
        std::cout << "Case #" << t + 1 << ": " << answer << std::endl;
    }
    return 0;
}
