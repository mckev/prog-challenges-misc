// https://www.spoj.com/problems/PRATA/

#include <cassert>
#include <iostream>
#include <queue>
#include <vector>


struct Job {
    int time;
    int rank;
    int num_cooks;
    int multiplier;
    bool operator<(const Job& other) const {
        return time > other.time;
    }
};

const int MAX_COOKS = 8;

int solve(int target, const std::vector<int>& num_ranks) {
    std::priority_queue<Job> jobs;
    for (int rank = 1; rank <= MAX_COOKS; rank++) {
        if (num_ranks[rank] > 0) {
            jobs.push({rank, rank, num_ranks[rank], 2});
        }
    }
    int completed = 0;
    int time = 0;
    while (completed < target) {
        Job job = jobs.top(); jobs.pop();
        completed += job.num_cooks;
        time = job.time;
        jobs.push({job.time + job.rank * job.multiplier, job.rank, job.num_cooks, job.multiplier + 1});
    }
    return time;
}


int main() {
    {
        const int UNDEFINED = -999;
        assert(solve(1000, std::vector<int>{UNDEFINED, 0, 0, 0, 0, 0, 0, 0, 1}) == 4004000);
        assert(solve(10, std::vector<int>{UNDEFINED, 1, 1, 1, 1, 0, 0, 0, 0}) == 12);
        assert(solve(8, std::vector<int>{UNDEFINED, 1, 0, 0, 0, 0, 0, 0, 0}) == 36);
        assert(solve(8, std::vector<int>{UNDEFINED, 8, 0, 0, 0, 0, 0, 0, 0}) == 1);
    }
    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        int target; std::cin >> target;
        std::vector<int> num_ranks(MAX_COOKS + 1);
        int num_cooks; std::cin >> num_cooks;
        for (int i = 0; i < num_cooks; i++) {
            int rank;
            std::cin >> rank;
            num_ranks[rank]++;
        }
        int min_time = solve(target, num_ranks);
        std::cout << min_time << std::endl;
    }
    return 0;
}
