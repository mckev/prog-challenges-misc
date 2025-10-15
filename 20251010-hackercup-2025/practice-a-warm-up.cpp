// https://www.facebook.com/codingcompetitions/hacker-cup/2025/practice-round/problems/A

#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <vector>


const int IMPOSSIBLE = -1;

std::vector<std::pair<int, int>> solve(const std::vector<int>& A, const std::vector<int>& B) {
    std::vector<std::pair<int, int>> answers;
    assert(A.size() == B.size());
    int N = A.size();
    // The target temperature must be larger or equal than the source temperature
    for (int i = 0; i < N; i++) {
        if (B[i] < A[i]) return {{IMPOSSIBLE, IMPOSSIBLE}};
    }
    // Remember all possible source temperatures
    std::map<int, int> temperature_indices;
    for (int i = 0; i < N; i++) temperature_indices[A[i]] = i;
    // We need to process from the smallest source temperature
    std::vector<int> indices(N);
    for (int i = 0; i < N; i++) indices[i] = i;
    std::sort(indices.begin(), indices.end(), [&A](int i1, int i2) {
        return A[i1] < A[i2];
    });
    for (int i : indices) {
        int source_temperature = A[i];
        int target_temperature = B[i];
        if (source_temperature == target_temperature) continue;
        bool is_exist = temperature_indices.find(target_temperature) != temperature_indices.end();
        if (! is_exist) return {{IMPOSSIBLE, IMPOSSIBLE}};
        answers.push_back({i, temperature_indices[target_temperature]});
    }
    return answers;
}


void test() {
    {
        std::vector<int> A = {4, 2, 3, 1};
        std::vector<int> B = {4, 4, 4, 3};
        std::vector<std::pair<int, int>> answers = solve(A, B);
        std::vector<std::pair<int, int>> expected = {
            {3, 2},
            {1, 0},
            {2, 0},
        };
        assert(answers == expected);
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
        std::vector<int> B;
        for (int n = 0; n < N; n++) {
            int b; std::cin >> b;
            B.push_back(b);
        }
        std::vector<std::pair<int, int>> answers = solve(A, B);
        std::cout << "Case #" << t + 1 << ": ";
        if (answers.size() == 1 && answers[0] == std::make_pair(IMPOSSIBLE, IMPOSSIBLE)) {
            std::cout << "-1" << std::endl;
            continue;
        }
        std::cout << answers.size() << std::endl;
        for (const std::pair<int, int>& answer : answers) {
            std::cout << answer.first + 1 << " " << answer.second + 1 << std::endl;
        }
    }
    return 0;
}
