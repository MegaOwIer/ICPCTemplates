namespace Cipolla {
    using type_t = long long;

    type_t P, a, b, w;

    struct Complex {
        type_t r, i;
        Complex(type_t _r, type_t _i) : r(_r), i(_i) {}
        Complex operator * (const Complex& a) {
            return Complex(
                (r * a.r + i * a.i % P * b) %P,
                (r * a.i + i * a.r) % P
            );
        }
    };

    Complex Pow(Complex a, type_t b) {
        Complex ans(1, 0);
        for(; b != 0; a = a * a, b >>= 1)
            if(b & 1) ans = ans * a;
        return ans;
    }
    type_t Pow(type_t a, type_t b, type_t mod) {
        type_t ans = 1;
        for(; b != 0; b >>= 1, a = a * a % P)
            if(b & 1) ans = ans * a % P;
        return ans;
    }
    type_t getSqrt(type_t N, type_t _P, type_t (*Random)()) {
        if(P != _P) P = _P;
        N %= P;
        if(N == 0) return 0;
        if(Pow(N, (P - 1) / 2, P) == P - 1) return -1;
        do w = Random() % P;
        while(Pow(b = (w * w % P - N + P) % P, (P - 1) / 2, P) != P - 1);
        return Pow(Complex(w, 1), (P + 1) / 2).r;
    }
}
