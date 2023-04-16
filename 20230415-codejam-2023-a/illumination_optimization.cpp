// https://codingcompetitions.withgoogle.com/codejam/round/0000000000c95b94/0000000000cad086

#include <algorithm>
#include <iostream>
#include <set>
#include <vector>


const int IMPOSSIBLE = -1;

int calc_bulb_index(int m, int R, const std::vector<int>& bulbs, int start_index) {
    // Given m, determine which light bulb should light.
    // Should give priority to the right-most bulb.
    int index = IMPOSSIBLE;
    for (int i = start_index; i < bulbs.size(); i++) {
        int bulb = bulbs[i];
        if (bulb - R <= m && m < bulb + R) {
            index = i;
            // This is a good answer, however keep continue to the right
            continue;
        }
        break;
    }
    return index;
}

int solve(int M, int R, int N, std::vector<int> bulbs) {
    std::set<int> answers;
    int start_index = 0;            // to speed up
    int m = 0;
    while (m < M) {
        int index = calc_bulb_index(m, R, bulbs, start_index);
        if (index == IMPOSSIBLE) {
            return IMPOSSIBLE;
        }
        int bulb = bulbs[index];
        answers.insert(bulb);
        // Next bulb
        m = bulb + R;
        start_index = index + 1;
    }
    return answers.size();
}

int main() {
    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        int M; std::cin >> M;       // road length
        int R; std::cin >> R;       // illumination radius
        int N; std::cin >> N;       // number of lights
        std::vector<int> bulbs;
        for (int n = 0; n < N; n++) {
            int bulb; std::cin >> bulb;
            bulbs.push_back(bulb);
        }
        std::sort(bulbs.begin(), bulbs.end());

        int answer = solve(M, R, N, bulbs);
        if (answer == IMPOSSIBLE) {
            std::cout << "Case #" << t + 1 << ": IMPOSSIBLE" << std::endl;
        } else {
            std::cout << "Case #" << t + 1 << ": " << answer << std::endl;
        }
    }
    return 0;
}
