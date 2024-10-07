// https://www.facebook.com/codingcompetitions/hacker-cup/2024/round-1/problems/A

#include <cassert>
#include <iostream>
#include <limits>
#include <vector>


struct AcceptedTime {
    int A;
    int B;
};

double solve(const std::vector<AcceptedTime>& acceptedTimes) {
    double minSpeed = std::numeric_limits<double>::lowest();        // Or -std::numeric_limits<double>::max()
    double maxSpeed = std::numeric_limits<double>::max();
    for (int station = 0; station < acceptedTimes.size(); station++) {
        double distance = station + 1;
        // We can deliver within A to B seconds, so allowed speed is between distance/B to distance/A
        double velocityA = distance / acceptedTimes[station].A;
        double velocityB = distance / acceptedTimes[station].B;
        minSpeed = std::max(minSpeed, velocityB);
        maxSpeed = std::min(maxSpeed, velocityA);
    }
    if (minSpeed > maxSpeed) return -1;
    return minSpeed;
};

void test() {
{
        // Test case #2
        std::vector<AcceptedTime> acceptedTimes = {
            {0, 3}, {1, 4}, {1, 2}, {0, 10}
        };
        double answer = solve(acceptedTimes);
        assert(answer == 1.5);
    }
    {
        // Test case #3
        std::vector<AcceptedTime> acceptedTimes = {
            {5, 10}, {0, 5}
        };
        double answer = solve(acceptedTimes);
        assert(answer == -1);
    }
}

int main() {
    test();
    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        int N; std::cin >> N;
        std::vector<AcceptedTime> acceptedTimes;
        for (int n = 0; n < N; n++) {
            AcceptedTime acceptedTime;
            int A; std::cin >> A; acceptedTime.A = A;
            int B; std::cin >> B; acceptedTime.B = B;
            acceptedTimes.push_back(acceptedTime);
        }
        double answer = solve(acceptedTimes);
        std::cout << "Case #" << t + 1 << ": " << answer << std::endl;
    }
    return 0;
}
