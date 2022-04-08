#include <bits/stdc++.h>

namespace std {

#ifdef __STRICT_ANSI__
template<>
class numeric_limits<__int128_t> {
public:
    static __int128_t max() {
        return (__uint128_t)-1 >> 1;
    }
    static __int128_t min() {
        return -max() - 1;
    }
};
#endif

}
