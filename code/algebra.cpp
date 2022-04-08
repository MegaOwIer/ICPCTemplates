#include <bits/stdc++.h>
using namespace std;

namespace Linear_Recursion {
    using LL = long long;
    constexpr int mod = 65537;
    constexpr int MXLEN = 100000;

    void update(int *res, const int *cur, const int *p, int n) {
        using Polynomial_NTT::multiply;
        using Polynomial_NTT::doDivision;
        static int tmp[MXLEN];
        multiply(tmp, res, n, cur, n);
        doDivision(nullptr, res, tmp, n + n - 1, p, n);
    }

    void Pow(int *res, LL b, const int *p, const int n) {
        static int cur[MXLEN];
        memset(cur, 0, sizeof(int[n]));
        cur[0] = 1;
        while(b) {
            if(b & 1) update(cur, res, p, n);
            update(res, res, p, n);
            b >>= 1;
        }
        memcpy(res, cur, sizeof(int[n]));
    }

    int solve(const int *p, const int len, const int *x, LL index) {
        static int res[MXLEN];
        memset(res, 0, sizeof(int[len]));
        res[1] = 1;
        Pow(res, index, p, len);
        int ans = 0;
        for(int i = 0; i < len; i++) {
            ans = (ans + (LL)x[i] * res[i]) % mod;
        }
        return ans;
    }
} // namespace Linear_Recursion