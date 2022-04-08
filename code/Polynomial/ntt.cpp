#include <bits/stdc++.h>
using namespace std;

// 469762049 -> 26, g = 3
// 998244353 -> 23, g = 3
// 1004535809 -> 21, g = 3
// 1231453023109121 -> 45, g = 3
// 1337006139375617 -> 46, g = 3
// 1945555039024054273 -> 56, g = 5
// 4179340454199820289 -> 57, g = 3
namespace Polynomial_NTT {

using int_t = int;
using long_t = long long;

// make sure 2*P will not cause signed overflow
constexpr int_t g = 3, P = 998244353;
constexpr int_t MXLEN = 1 << 19;
constexpr int_t inv2 = (P + 1) >> 1;
using array = int_t[MXLEN + 1];

inline size_t getLength(int_t val) {
    val |= val >> 1;    val |= val >> 2;
    val |= val >> 4;    val |= val >> 8;
    val |= val >> 16;
    return val + 1;
}

int_t Pow(long_t u, int_t v) {
    long_t ans = 1;
    for(; v; v >>= 1, u = u * u % P)
        if(v & 1) ans = u * ans % P;
    return (int_t)ans;
}

int_t getInv(int_t u) { return Pow(u, P - 2); }

void DFT(int_t *A, const size_t n, const int_t flag) {
    static size_t rev[MXLEN + 1], preN;
    if(preN != n) {
        for(size_t i = 0; i < n; i++) {
            rev[i] = (rev[i >> 1] >> 1) | ((i & 1) * (n >> 1));
        }
        preN = n;
    }
    for(size_t i = 0; i < n; i++)
        if(i < rev[i]) swap(A[i], A[rev[i]]);
    for(size_t i = 1, r = 1; i < n; i *= 2, r++) {
        long_t wn = Pow(g, P - 1 + flag * ((P - 1) >> r));
        for(size_t j = 0; j < n; j += i * 2) {
            long_t w = 1;
            for(size_t k = j; k < j + i; k++) {
                int_t x0 = A[k], x1 = int_t(w * A[i + k] % P);
                A[k] = (x0 + x1) % P;
                A[i + k] = (x0 + P - x1) % P;
                w = wn * w % P;
            }
        }
    }
    if(flag == -1) {
        long_t tmp = getInv(n);
        for(size_t i = 0; i < n; i++) A[i] = int_t(A[i] * tmp % P);
    }
}

void multiply(int_t *res, const int_t *A, const size_t n, const int_t *B, const size_t m) {
    static array _A, _B;
    size_t X = getLength(n + m - 2);
    memcpy(_A, A, sizeof(int_t[n])), memset(_A + n, 0, sizeof(int_t[X - n]));
    memcpy(_B, B, sizeof(int_t[m])), memset(_B + m, 0, sizeof(int_t[X - m]));
    DFT(_A, X, 1), DFT(_B, X, 1);
    for(size_t i = 0; i < X; i++) res[i] = int_t((long_t)_A[i] * _B[i] % P);
    DFT(res, X, -1);
}

void getInverse(int_t *res, const int_t *A, const size_t n) {
    if(n == 1) {
        res[0] = getInv(A[0]);
        return;
    }
    static array T;
    size_t X = getLength(n + n - 2), half = (n + 1) >> 1;
    getInverse(res, A, half), memset(res + half, 0, sizeof(int_t[X - half]));
    memcpy(T, A, sizeof(int_t[n])), memset(T + n, 0, sizeof(int_t[X - n]));
    DFT(res, X, 1), DFT(T, X, 1);
    for(size_t i = 0; i < X; i++)
        res[i] = int_t((2 * res[i] + long_t(P - T[i]) * res[i] % P * res[i]) % P);
    DFT(res, X, -1);
}

void getDerivative(int_t *res, const int_t *A, const size_t n) {
    for(size_t i = 1; i < n; i++) res[i - 1] = int_t((long_t)i * A[i] % P);
    res[n - 1] = 0;
}

void getIntegral(int_t *res, const size_t n, const int_t *A) {
    static size_t preN = 1;
    static int_t inv[MXLEN + 1] = {0, 1};
    if(n > preN) {
        for(size_t i = preN + 1; i <= n; i++)
            inv[i] = int_t(long_t(P - P / i) * inv[P % i] % P);
        preN = n;
    }
    for(size_t i = n - 1; i; i--) res[i] = int_t((long_t)A[i - 1] * inv[i] % P);
    res[0] = 0;
}

void getLogarithm(int_t *res, const int_t *A, const size_t n) {
    static array invF, dF;
    size_t X = getLength(n + n - 2);
    getDerivative(dF, A, n), memset(dF + n, 0, sizeof(int_t[X - n]));
    getInverse(invF, A, n), memset(invF + n, 0, sizeof(int_t[X - n]));
    DFT(invF, X, 1), DFT(dF, X, 1);
    for(int_t i = 0; i < X; i++) invF[i] = int_t((long_t)invF[i] * dF[i] % P);
    DFT(invF, X, -1);
    getIntegral(res, n, invF);
}

void getExponential(int_t *res, const int_t *A, const size_t n) {
    if(n == 1) {
        res[0] = 1;
        return;
    }
    static array T;
    size_t half = (n + 1) >> 1, X = getLength(n + half - 2);
    getExponential(res, A, half), memset(res + half, 0, sizeof(int_t[X - half]));
    getLogarithm(T, res, n);
    for(size_t i = 0; i < n; i++) T[i] = (A[i] + P - T[i]) % P;
    memset(T + n, 0, sizeof(int_t[X - n]));
    DFT(res, X, 1), DFT(T, X, 1);
    for(size_t i = 0; i < X; i++) res[i] = int_t(res[i] * long_t(1 + T[i]) % P);
    DFT(res, X, -1);
}

void getSqrt(int_t *res, const int_t *A, const size_t n) {
    if(n == 1) {
        // int_t tmp = Cipolla::getSqrt(A[0], P);
        // res[0] = min(tmp, P - tmp);
        return;
    }
    static array invF, B;
    size_t X = getLength(n + n - 2), half = (n + 1) >> 1;
    getSqrt(res, A, half), memset(res + half, 0, sizeof(int_t[n - half]));
    getInverse(invF, res, n), memset(invF + n, 0, sizeof(int_t[X - n]));
    memcpy(B, A, sizeof(int_t[n])), memset(B + n, 0, sizeof(int_t[X - n]));
    DFT(B, X, 1), DFT(invF, X, 1);
    for(size_t i = 0; i < X; i++) B[i] = int_t((long_t)B[i] * invF[i] % P);
    DFT(B, X, -1);
    for(size_t i = 0; i < n; i++) res[i] = int_t(long_t(res[i] + B[i]) * inv2 % P);
}

void doDivision(int_t *q, int_t *r, const int_t *A, const size_t n, const int_t *B, const size_t m) {
    static array _r, _q, _A, _B;
    if(q == nullptr) q = _q;
    for(size_t i = 1; i <= n; i++) _A[n - i] = A[i - 1];
    for(size_t i = 1; i <= m; i++) _B[m - i] = B[i - 1];
    size_t lres = n - m + 1;
    for(size_t i = m; i < lres; i++) _B[i] = 0;
    getInverse(_r, _B, lres);
    multiply(q, _A, lres, _r, lres);
    reverse(q, q + lres);
    if(r != nullptr) {
        multiply(_A, q, lres, B, m);
        for(size_t i = 0; i < n; i++) r[i] = (A[i] - _A[i] + P) % P;
    }
}

} // namespace Polynomial_NTT
