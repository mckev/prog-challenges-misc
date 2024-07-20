// https://www.facebook.com/codingcompetitions/hacker-cup/2020/round-1/problems/A3
// How to run:
//    $ ulimit -s unlimited
//    $ cat main.cpp; rm -f main.out; g++ -Ofast -o main.out main.cpp; cat perimetric_chapter_3_input.txt | time ./main.out

#include <algorithm>
#include <cassert>
#include <iostream>
#include <set>
#include <vector>


long long modulo(long long x) {
    if (x < 0) {
        x = x % 1000000007 + 1000000007;
    }
    return x % 1000000007;
}


struct BuildingBoundary {
    int x;
    mutable int height;
    bool operator<(const BuildingBoundary& other) const {
        // For set
        return x < other.x;
    }
    bool operator<(int other_x) const {
        // For lower_bound()
        return x < other_x;
    }
    static bool upper_bound_comparator(int x, const BuildingBoundary& other) {
        // For upper_bound()
        return x < other.x;
    }
};


int compute_height(const std::set<BuildingBoundary>& building_boundaries, int x) {
    // Range: [ ..., x ]
    auto it = std::upper_bound(building_boundaries.begin(), building_boundaries.end(), x, BuildingBoundary::upper_bound_comparator);
    if (it != building_boundaries.begin()) {
        it--;
        return (*it).height;
    }
    return 0;
}


int solve(const std::vector<int>& L, const std::vector<int>& W, const std::vector<int>& H) {
    std::vector<long long> P;
    int N = L.size();
    std::set<BuildingBoundary> building_boundaries;

    long long perimeter = 0;
    for (int n = 0; n < N; n++) {
        long long old_perimeter = 0, new_perimeter = 0;
        int prev_x = 0, prev_old_height = 0, prev_new_height = 0;

        // Set building boundaries at L[n] and L[n] + W[n] with old heights
        building_boundaries.insert({ L[n], compute_height(building_boundaries, L[n]) });
        building_boundaries.insert({ L[n] + W[n], compute_height(building_boundaries, L[n] + W[n]) });

        // Range: [ 1 point before L[n], L[n] + W[n] ]
        auto it_start = std::lower_bound(building_boundaries.begin(), building_boundaries.end(), L[n]); if (it_start != building_boundaries.begin()) it_start--;
        for (auto it = it_start; it != building_boundaries.end() && (*it).x <= L[n] + W[n]; ) {
            BuildingBoundary building_boundary = *it;
            int x = building_boundary.x;
            int old_height = building_boundary.height;
            int new_height = old_height;
            if (x >= L[n] && x < L[n] + W[n]) {
                new_height = H[n];
            }

            // Detect changes in perimeters
            if (prev_old_height > 0) {
                old_perimeter += 2 * (x - prev_x);
            }
            if (prev_new_height > 0) {
                new_perimeter += 2 * (x - prev_x);
            }
            old_perimeter += std::abs(prev_old_height - old_height);
            new_perimeter += std::abs(prev_new_height - new_height);
            prev_x = x;
            prev_old_height = old_height;
            prev_new_height = new_height;

            // Remove previous boundaries, as the new building is taller
            if (x >= L[n] && x < L[n] + W[n]) {
                it = building_boundaries.erase(it);
            } else {
                it++;
            }
        }

        // Set building boundaries at L[n] with new height
        building_boundaries.insert({ L[n], H[n] });

        perimeter += new_perimeter - old_perimeter;
        perimeter = modulo(perimeter);
        P.push_back(perimeter);
    }
    long long answer = 1;
    for (long long p : P) {
        answer = modulo(answer * p);
    }
    return answer;
}


int main() {
    {
        // Test Case #4
        int N = 10;
        int K = 3;
        std::vector<int> L = {14, 5, 14};
        assert(L.size() == K);
        long long Al = 4, Bl = 7, Cl = 2, Dl = 47;
        std::vector<int> W = {6, 4, 13};
        assert(W.size() == K);
        long long Aw = 2, Bw = 3, Cw = 17, Dw = 33;
        std::vector<int> H = {11, 13, 14};
        assert(H.size() == K);
        long long Ah = 1, Bh = 0, Ch = 2, Dh = 31;
        for (int n = K; n < N; n++) {
            int l = ((Al * L[L.size() - 2] + Bl * L[L.size() - 1] + Cl) % Dl) + 1;
            L.push_back(l);
            int w = ((Aw * W[W.size() - 2] + Bw * W[W.size() - 1] + Cw) % Dw) + 1;
            W.push_back(w);
            int h = ((Ah * H[H.size() - 2] + Bh * H[H.size() - 1] + Ch) % Dh) + 1;
            H.push_back(h);
        }
        assert(L.size() == N);
        assert(W.size() == N);
        assert(H.size() == N);
        int answer = solve(L, W, H);
        assert(answer == 842325442);
    }
    {
        // Test Case #8
        // int N = 200000;
        // int K = 2;
        // std::vector<int> L = {1, 3};
        // assert(L.size() == K);
        // long long Al = 0, Bl = 1, Cl = 100, Dl = 500000000;
        // std::vector<int> W = {100, 100};
        // assert(W.size() == K);
        // long long Aw = 0, Bw = 0, Cw = 99, Dw = 500000000;
        // std::vector<int> H = {300000000, 300000000};
        // assert(H.size() == K);
        // long long Ah = 1, Bh = 0, Ch = 0, Dh = 500000000;
        // for (int n = K; n < N; n++) {
        //     int l = ((Al * L[L.size() - 2] + Bl * L[L.size() - 1] + Cl) % Dl) + 1;
        //     L.push_back(l);
        //     int w = ((Aw * W[W.size() - 2] + Bw * W[W.size() - 1] + Cw) % Dw) + 1;
        //     W.push_back(w);
        //     int h = ((Ah * H[H.size() - 2] + Bh * H[H.size() - 1] + Ch) % Dh) + 1;
        //     H.push_back(h);
        // }
        // assert(L.size() == N);
        // assert(W.size() == N);
        // assert(H.size() == N);
        // int answer = solve(L, W, H);
        // assert(answer == 341232474);
    }
    {
        // Test Case #9
        // int N = 1000000;
        // int K = 2;
        // std::vector<int> L = {1, 2};
        // assert(L.size() == K);
        // long long Al = 0, Bl = 1, Cl = 0, Dl = 500000000;
        // std::vector<int> W = {2, 3};
        // assert(W.size() == K);
        // long long Aw = 0, Bw = 1, Cw = 0, Dw = 10000000;
        // std::vector<int> H = {1, 2};
        // assert(H.size() == K);
        // long long Ah = 0, Bh = 1, Ch = 0, Dh = 500000000;
        // for (int n = K; n < N; n++) {
        //     int l = ((Al * L[L.size() - 2] + Bl * L[L.size() - 1] + Cl) % Dl) + 1;
        //     L.push_back(l);
        //     int w = ((Aw * W[W.size() - 2] + Bw * W[W.size() - 1] + Cw) % Dw) + 1;
        //     W.push_back(w);
        //     int h = ((Ah * H[H.size() - 2] + Bh * H[H.size() - 1] + Ch) % Dh) + 1;
        //     H.push_back(h);
        // }
        // assert(L.size() == N);
        // assert(W.size() == N);
        // assert(H.size() == N);
        // int answer = solve(L, W, H);
        // assert(answer == 177384201);
    }
    {
        // Test Case #10
        // int N = 1000000;
        // int K = 2;
        // std::vector<int> L = {1, 10000000};
        // assert(L.size() == K);
        // long long Al = 0, Bl = 1, Cl = 10000000, Dl = 500000000;
        // std::vector<int> W = {2, 3};
        // assert(W.size() == K);
        // long long Aw = 0, Bw = 1, Cw = 0, Dw = 10000000;
        // std::vector<int> H = {1, 2};
        // assert(H.size() == K);
        // long long Ah = 0, Bh = 1, Ch = 0, Dh = 500000000;
        // for (int n = K; n < N; n++) {
        //     int l = ((Al * L[L.size() - 2] + Bl * L[L.size() - 1] + Cl) % Dl) + 1;
        //     L.push_back(l);
        //     int w = ((Aw * W[W.size() - 2] + Bw * W[W.size() - 1] + Cw) % Dw) + 1;
        //     W.push_back(w);
        //     int h = ((Ah * H[H.size() - 2] + Bh * H[H.size() - 1] + Ch) % Dh) + 1;
        //     H.push_back(h);
        // }
        // assert(L.size() == N);
        // assert(W.size() == N);
        // assert(H.size() == N);
        // int answer = solve(L, W, H);
        // assert(answer == 59533825);
    }
    {
        // Test Case #14
        // int N = 1000000;
        // int K = 2;
        // std::vector<int> L = {100000000, 100000001};
        // assert(L.size() == K);
        // long long Al = 0, Bl = 1, Cl = 0, Dl = 500000000;
        // std::vector<int> W = {100000000, 100000001};
        // assert(W.size() == K);
        // long long Aw = 0, Bw = 1, Cw = 499999999, Dw = 500000000;
        // std::vector<int> H = {1, 2};
        // assert(H.size() == K);
        // long long Ah = 0, Bh = 1, Ch = 0, Dh = 500000000;
        // for (int n = K; n < N; n++) {
        //     int l = ((Al * L[L.size() - 2] + Bl * L[L.size() - 1] + Cl) % Dl) + 1;
        //     L.push_back(l);
        //     int w = ((Aw * W[W.size() - 2] + Bw * W[W.size() - 1] + Cw) % Dw) + 1;
        //     W.push_back(w);
        //     int h = ((Ah * H[H.size() - 2] + Bh * H[H.size() - 1] + Ch) % Dh) + 1;
        //     H.push_back(h);
        // }
        // assert(L.size() == N);
        // assert(W.size() == N);
        // assert(H.size() == N);
        // int answer = solve(L, W, H);
        // assert(answer == 985813229);
    }

    int T; std::cin >> T;
    for (int t = 0; t < T; t++) {
        int N, K; std::cin >> N >> K;
        std::vector<int> L;
        for (int k = 0; k < K; k++) {
            int i; std::cin >> i;
            L.push_back(i);
        }
        assert(L.size() == K);
        long long Al, Bl, Cl, Dl; std::cin >> Al >> Bl >> Cl >> Dl;
        std::vector<int> W;
        for (int k = 0; k < K; k++) {
            int i; std::cin >> i;
            W.push_back(i);
        }
        assert(W.size() == K);
        long long Aw, Bw, Cw, Dw; std::cin >> Aw >> Bw >> Cw >> Dw;
        std::vector<int> H;
        for (int k = 0; k < K; k++) {
            int i; std::cin >> i;
            H.push_back(i);
        }
        assert(H.size() == K);
        long long Ah, Bh, Ch, Dh; std::cin >> Ah >> Bh >> Ch >> Dh;

        // Compute L from Al, Bl, Cl, Dl. Compute W from Aw, Bw, Cw, Dw. Compute H from Ah, Bh, Ch, Dh.
        for (int n = K; n < N; n++) {
            int l = ((Al * L[L.size() - 2] + Bl * L[L.size() - 1] + Cl) % Dl) + 1;
            L.push_back(l);
            int w = ((Aw * W[W.size() - 2] + Bw * W[W.size() - 1] + Cw) % Dw) + 1;
            W.push_back(w);
            int h = ((Ah * H[H.size() - 2] + Bh * H[H.size() - 1] + Ch) % Dh) + 1;
            H.push_back(h);
        }
        assert(L.size() == N);
        assert(W.size() == N);
        assert(H.size() == N);

        int answer = solve(L, W, H);
        std::cout << "Case #" << t + 1 << ": " << answer << std::endl;
    }
    return 0;
}
