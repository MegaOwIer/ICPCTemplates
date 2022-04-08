template<typename type_t, type_t (*Mul)(type_t, type_t, type_t)>
class MillarRabin {
private:
    type_t _pow(type_t a, type_t b, type_t mod) {
        type_t ans = 1;
        a %= mod;
        for(; b; b >>= 1, a = Mul(a, a, mod))
            if(b & 1) ans = Mul(ans, a, mod);
        return ans;
    }
    bool _judge(int prime, type_t res, int vp2, type_t N) {
        type_t p = _pow(prime, res, N);
        if(p == 1) return false;
        for(int i = 0; i < vp2; i++) {
            if(p == N - 1 || p == 1) return false;
            p = Mul(p, p, N);
        }
        return true;
    }
public:
    bool operator () (type_t n) {
        static const int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
        if(n <= 1) return false;
        type_t p = n - 1;
        int vp2 = 0;
        while(!(p & 1)) vp2++, p >>= 1;
        for(int i : primes) {
            if(n % i == 0) return n == i;
            if(_judge(i, p, vp2, n)) return false;
        }
        return true;
    }
};

template<typename type_t, 
    type_t (*Mul)(type_t, type_t, type_t), 
    int (*getctz)(type_t), 
    type_t (*Random)()
>
class PollardPho : public MillarRabin<type_t, Mul> {
private:
    type_t _abs(type_t N) {return N >= 0 ? N : -N;}
    type_t _gcd(type_t a, type_t b) {
        if(a == 0 || b == 0) return a + b;
        int ctz = getctz(a | b);
        a >>= getctz(a);
        while(b != 0) {
            b >>= getctz(b);
            if(a > b)swap(a, b);
            b -= a;
        }
        return a << ctz;
    }
public:
    void Divide(type_t n, vector<type_t> &res) {
        if(n <= 1) return;
        if((*this)(n)) return res.push_back(n);
        type_t a, b, c, del;
        while(true) {
            c = Random() % n;
            a = b = Random() % n;
            b = (Mul(b, b, n) + c) % n;
            while(a != b) {
                del = _gcd(_abs(a - b), n);
                if(del > 1 && del < n) {
                    Divide(del, res);
                    Divide(n / del, res);
                    return;
                }
                a = (Mul(a, a, n) + c) % n;
                b = (Mul(b, b, n) + c) % n;
                b = (Mul(b, b, n) + c) % n;
            }
        }
    }
};
