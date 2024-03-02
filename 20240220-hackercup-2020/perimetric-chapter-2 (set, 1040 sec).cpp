// https://www.facebook.com/codingcompetitions/hacker-cup/2020/round-1/problems/A2

#include <cassert>
#include <iostream>
#include <set>
#include <unordered_set>
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
        MAX_L = std::max(MAX_L, L[n] + W[n]);
    }
    std::vector<int> height_map(MAX_L + 1);

    // To speed up
    std::set<int> processed;                                            // As we know height is getting decreased, we can skip "l" that has been previously processed
    for (int l = 0; l < MAX_L; l++) {
        processed.insert(processed.end(), l);                           // Use hint "processed.end()" to make insertion much faster
    }

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
        std::unordered_set<int> processing;
        for (auto l_it = processed.lower_bound(L[n]); l_it != processed.end() && *l_it < L[n] + W[n]; ) {
            // Version without lower_bound():  for (int l = L[n]; l < L[n] + W[n]; l++) {
            int l = *l_it;
            if (H[n] > height_map[l]) {
                processing.insert(processing.end(), l);
                processing.insert(processing.end(), l + 1);
            }
            l_it = processed.erase(l_it);                               // iterator points to the next location, so no need for it++
        }
        long long prev_p = 0;
        for (int l : processing) {
            int height_diff = (l > 0) ? std::abs(height_map[l] - height_map[l - 1]) : height_map[l];
            prev_p += height_diff;                                      // Changes in height
            if (height_map[l] > 0) prev_p += 2;                         // Add base and top
        }
        for (int l : processing) {
            if (l >= L[n] && l < L[n] + W[n] && H[n] > height_map[l]) {
                height_map[l] = H[n];
            }
        }
        long long cur_p = 0;
        for (int l : processing) {
            int height_diff = (l > 0) ? std::abs(height_map[l] - height_map[l - 1]) : height_map[l];
            cur_p += height_diff;
            if (height_map[l] > 0) cur_p += 2;
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
    // std::lower_bound - returns iterator to first element in the given range which is EQUAL_TO or Greater than val.
    // std::upper_bound - returns iterator to first element in the given range which is Greater than val.
    // Ref: https://stackoverflow.com/questions/41958581/difference-between-upper-bound-and-lower-bound-in-stl

    // - Segment tree stores intervals, and optimized for "which of these intervals contains a given point" queries.
    //   https://cp-algorithms.com/data_structures/segment_tree.html
    // - Interval tree stores intervals as well, but optimized for "which of these intervals overlap with a given interval" queries. It can also be used for point queries - similar to segment tree.
    // - Range tree stores points, and optimized for "which points fall within a given interval" queries.
    // - Binary indexed tree stores items-count per index, and optimized for "how many items are there between index m and n" queries.
    // Ref: https://stackoverflow.com/questions/17466218/what-are-the-differences-between-segment-trees-interval-trees-binary-indexed-t

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
        std::cout << "Internal test case #4 passed" << std::endl;
    }

    {
        // Test Case #7
        int N = 600;
        int K = 62;
        std::vector<int> L = {1474, 2301, 2678, 1532, 3139, 793, 300, 3123, 2532, 1001, 2579, 1730, 3115, 580, 3153, 1477, 2997, 3149, 1641, 2803, 3191, 1339, 1783, 932, 1229, 2219, 481, 112, 1920, 2222, 1229, 640, 1452, 2873, 2255, 839, 1701, 272, 626, 2271, 2953, 2856, 601, 1203, 3044, 2071, 93, 2495, 2680, 635, 1399, 1191, 2037, 1610, 1257, 2265, 2550, 2159, 1794, 2829, 703, 1662};
        assert(L.size() == K);
        long long Al = 359465800, Bl = 448903324, Cl = 384499499, Dl = 69553;
        std::vector<int> W = {867, 778, 911, 642, 13, 535, 302, 134, 499, 182, 685, 140, 795, 949, 203, 389, 798, 764, 699, 168, 586, 764, 373, 503, 733, 628, 223, 175, 274, 317, 727, 681, 788, 791, 547, 419, 499, 640, 68, 803, 583, 827, 657, 785, 9, 146, 944, 647, 590, 158, 614, 902, 847, 86, 788, 733, 920, 72, 593, 740, 174, 597};
        assert(W.size() == K);
        long long Aw = 172661098, Bw = 458259686, Cw = 739759323, Dw = 1451;
        std::vector<int> H = {14451710, 14451707, 14451706, 14451699, 14451695, 14451689, 14451687, 14451678, 14451674, 14451667, 14451660, 14451651, 14451642, 14451635, 14451631, 14451623, 14451620, 14451614, 14451607, 14451604, 14451596, 14451592, 14451592, 14451586, 14451577, 14451571, 14451570, 14451570, 14451562, 14451560, 14451555, 14451554, 14451550, 14451549, 14451541, 14451536, 14451528, 14451524, 14451523, 14451523, 14451517, 14451513, 14451512, 14451507, 14451498, 14451492, 14451483, 14451482, 14451481, 14451480, 14451475, 14451471, 14451470, 14451464, 14451459, 14451452, 14451450, 14451450, 14451450, 14451442, 14451439, 14451433};
        assert(H.size() == K);
        long long Ah = 0, Bh = 1, Ch = 20004515, Dh = 20004520;
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
        assert(answer == 941855890);
        std::cout << "Internal test case #7 passed" << std::endl;
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
        std::cout << "Internal test case #8 passed" << std::endl;
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
        std::cout << "Internal test case #9 passed" << std::endl;
    }

    {
        // Test Case #10
        int N = 1000000;
        int K = 2;
        std::vector<int> L = {1, 10000000};
        assert(L.size() == K);
        long long Al = 0, Bl = 1, Cl = 10000000, Dl = 500000000;
        std::vector<int> W = {2, 3};
        assert(W.size() == K);
        long long Aw = 0, Bw = 1, Cw = 0, Dw = 10000000;
        std::vector<int> H = {387697077, 387697077};
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
        assert(answer == 646192635);
        std::cout << "Internal test case #10 passed" << std::endl;
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
