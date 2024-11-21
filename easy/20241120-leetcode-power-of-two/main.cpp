// https://leetcode.com/problems/power-of-two/description/

#include <cassert>
#include <cmath>


class SolutionBitwise {
public:
    bool isPowerOfTwo(long long n) {
        return (n != 0) && !(n & (n - 1));
    }
};


class SolutionMath {
public:
    const double EPSILON = 0.0000000001;

    bool isPowerOfTwo(long long n) {
        if (n <= 0) return false;
        // n = 2^p
        // log n = p log 2
        // p = log n / log 2
        double p = std::log(n) / std::log(2);
        // Check if p is an integer
        bool isInteger = std::abs(p - int(p)) < EPSILON;
        return isInteger;
    }
};


class SolutionRecursive {
public:
    bool isPowerOfTwo(long long n) {
        if (n == 0) return false;
        if (n == 1) return true;
        if (n % 2 != 0) return false;
        return isPowerOfTwo(n / 2);
    }
};


int main() {
    {
        SolutionMath solutionMath = SolutionMath();
        assert(solutionMath.isPowerOfTwo(536870912) == true);
        assert(solutionMath.isPowerOfTwo(16777217) == false);
        assert(solutionMath.isPowerOfTwo(0) == false);
        assert(solutionMath.isPowerOfTwo(-16) == false);
    }
    for (int n = -1000000; n < 1000000; n++) {
        SolutionBitwise solutionBitwise = SolutionBitwise();
        SolutionMath solutionMath = SolutionMath();
        SolutionRecursive solutionRecursive = SolutionRecursive();
        bool answerBitwise = solutionBitwise.isPowerOfTwo(n);
        bool answerMath = solutionMath.isPowerOfTwo(n);
        bool answerRecursive = solutionRecursive.isPowerOfTwo(n);
        assert(answerBitwise == answerRecursive);
        assert(answerMath == answerRecursive);
    }
    return 0;
}
