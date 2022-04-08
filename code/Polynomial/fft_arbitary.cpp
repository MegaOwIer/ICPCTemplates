void multiply(int *res, const int *A, const int n, const int *B, const int m) {
    static complex _A[MXLEN], _B[MXLEN];
    static complex a[MXLEN], b[MXLEN], c[MXLEN], d[MXLEN];
    int X = 1;
    for(; X < n + m - 1; X <<= 1);
    for(int i = 0; i < n; i++) _A[i] = complex(A[i] >> 16, A[i] & 65535);
    fill(_A + n, _A + X + 1, complex(0, 0));
    for(int i = 0; i < m; i++) _B[i] = complex(B[i] >> 16, B[i] & 65535);
    fill(_B + m, _B + X + 1, complex(0, 0));
    DFT(_A, X), DFT(_B, X);
    for(int i = 0; i < X; i++) {
        int j = i ? X - i : 0;
        complex a1 = (_A[i] + ~_A[j]) * complex(0.5, 0), 
        a2 = (_A[i] - ~_A[j]) * complex(0, -0.5), 
        b1 = (_B[i] + ~_B[j]) * complex(0.5, 0), 
        b2 = (_B[i] - ~_B[j]) * complex(0, -0.5);
        a[i] = a1 * b1, b[i] = a1 * b2;
        c[i] = a2 * b1, d[i] = a2 * b2;
    }
    for(int i = 0; i < X; i++) {
        _A[i] = a[i] + b[i] * complex(0, 1);
        _B[i] = c[i] + d[i] * complex(0, 1);
    }
    DFT(_A, X, -1), DFT(_B, X, -1);
    for(int i = 0; i < n + m - 1; i++) {
        LL a, b, c, d;
        a = LL(_A[i].x + 0.5) % mod;
        b = LL(_A[i].y + 0.5) % mod;
        c = LL(_B[i].x + 0.5) % mod;
        d = LL(_B[i].y + 0.5) % mod;
        res[i] = (((a << 32) + ((b + c) << 16) + d) % mod + mod) % mod;
    }
}
