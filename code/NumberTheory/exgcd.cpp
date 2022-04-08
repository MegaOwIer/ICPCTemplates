template<typename type_t>
type_t ex_gcd(type_t a,type_t b,type_t &x,type_t &y) {
    if(b==0) return x = 1, y = 0, a;
    type_t d = ex_gcd(b, a % b, y, x);
    y -= a / b * x;
    return d;
}
