// https://www.facebook.com/codingcompetitions/hacker-cup/2020/round-1/problems/A1

#include <cassert>
#include <iostream>
#include <vector>
// #pragma GCC optimize "trapv"                                         // Detect overflow


long long modulo(long long x) {
    if (x < 0) {
        x = x % 1000000007 + 1000000007;
    }
    return x % 1000000007;
}

int solve(const std::vector<int>& L, const std::vector<int>& W, const std::vector<int>& H) {
    int N = L.size();
    int MAX_L = 0;
    for (int n = 0; n < N; n++) {
        MAX_L = std::max(MAX_L, L[n] + W[n] + 1);
    }
    std::vector<int> cur_height_map(MAX_L);

    // To speed up
    std::vector<int> prev_height_map(MAX_L);
    std::vector<int> skip(MAX_L);                                       // As we know height is getting decreased, so we can skip "l" that has been previously set
    long long p = 0;

    std::vector<long long> P;
    for (int n = 0; n < N; n++) {
        // // Compute perimeter (slow)
        // for (int l = L[n]; l < L[n] + W[n]; l++) {
        //     height_map[l] = std::max(height_map[l], H[n]);
        // }
        // long long p = 0;
        // for (int l = 0; l < MAX_L; l++) {
        //     int height_diff = (l > 0) ? std::abs(height_map[l] - height_map[l - 1]) : height_map[l];
        //     p += height_diff;                                           // Changes in height
        //     if (height_map[l] > 0) {
        //         p += 2;                                                 // Add base and top
        //     }
        // }

        // Compute perimeter (fast)
        long long prev_p = 0;
        long long cur_p = 0;
        for (int l = L[n]; l < L[n] + W[n] + 1; l++) {
            // Previous
            int prev_height_diff = (l > 0) ? std::abs(prev_height_map[l] - prev_height_map[l - 1]) : prev_height_map[l];
            prev_p += prev_height_diff;                                 // Changes in height
            if (prev_height_map[l] > 0) prev_p += 2;                    // Add base and top
            // Current
            if (l >= L[n] && l < L[n] + W[n]) {
                cur_height_map[l] = std::max(cur_height_map[l], H[n]);
            }
            int cur_height_diff = (l > 0) ? std::abs(cur_height_map[l] - cur_height_map[l - 1]) : cur_height_map[l];
            cur_p += cur_height_diff;
            if (cur_height_map[l] > 0) cur_p += 2;
            // Update height map
            prev_height_map[l - 1] = cur_height_map[l - 1];
            // If we have encountered "l" before, then skip.
            if (skip[l] > 0) {
                if (skip[l + skip[l]] > 0) {
                    skip[l] += skip[l + skip[l]];                       // Merge skip
                }
                l += skip[l] - 1;
                continue;
            }
            skip[l] = L[n] + W[n] - l;
        }
        p = p + cur_p - prev_p;                                         // Here we just update the delta (diff) perimeter with addition of the new room (current) vs without (previous)

        p = modulo(p);
        P.push_back(p);
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
        std::vector<int> H = {29, 23, 22};
        assert(H.size() == K);
        long long Ah = 1, Bh = 0, Ch = 27, Dh = 31;
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
        assert(answer == 81786362);
    }

    {
        // Test Case #8
        int N = 200000;
        int K = 2;
        std::vector<int> L = {1, 3};
        assert(L.size() == K);
        long long Al = 0, Bl = 1, Cl = 100, Dl = 500000000;
        std::vector<int> W = {100, 100};
        assert(W.size() == K);
        long long Aw = 0, Bw = 0, Cw = 99, Dw = 500000000;
        std::vector<int> H = {400000000, 400000000};
        assert(H.size() == K);
        long long Ah = 1, Bh = 0, Ch = 499999998, Dh = 500000000;
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
        assert(answer == 126863246);
    }

    {
        // Test Case #9
        int N = 1000000;
        int K = 2;
        std::vector<int> L = {1, 2};
        assert(L.size() == K);
        long long Al = 0, Bl = 1, Cl = 0, Dl = 500000000;
        std::vector<int> W = {2, 3};
        assert(W.size() == K);
        long long Aw = 0, Bw = 1, Cw = 0, Dw = 10000000;
        std::vector<int> H = {391596781, 391596780};
        assert(H.size() == K);
        long long Ah = 0, Bh = 1, Ch = 499999998, Dh = 500000000;
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
        assert(answer == 313131619);
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
