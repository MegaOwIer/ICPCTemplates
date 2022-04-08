namespace BSGS {
    using type_t = long long;

    type_t P, base, sqrtP;
    unordered_map<type_t, type_t> G;

    type_t Pow(type_t a, type_t b) {
        type_t ans = 1;
        for(; b; b >>= 1, a = a * a % P)
            if(b & 1) ans = ans * a % P;
        return ans;
    }
    void init(type_t _P, type_t _B) {
        P = _P, base = _B;
        sqrtP = static_cast<type_t>(sqrt(P) + 1);
        G.clear();
        type_t w = Pow(base, sqrtP), cur = w;
        for(int i = 1; i <= sqrtP; i++) {
            if(G.count(cur)) break;
            G[cur] = i * sqrtP;
            cur = cur * w % P;
        }
    }

    type_t getLog(type_t N) {
        type_t ans = P;
        for(int i = 0; i <= sqrtP; i++) {
            if(G.count(N)) ans = min(ans, G[N] - i);
            N = N * base % P;
        }
        return ans == P ? -1 : ans;
    }
}
