namespace LCT {

using ULL = unsigned;
const int MX = 100005;

struct splayNode {
    splayNode *fa, *son[2];
    ULL all, imag;
    int rev;

    splayNode() : all(0), imag(0), rev(0) {
        fa = nullptr;
        son[0] = son[1] = nullptr;
    }

    bool isRoot() {return !fa || (fa -> son[0] != this && fa -> son[1] != this);}
    int dir() {return this == fa -> son[1];}

    void maintain() {
        all = imag;
        if(son[0]) all ^= son[0] -> all;
        if(son[1]) all ^= son[1] -> all;
    }

    void pushdown() {
        if(!rev) return;
        swap(son[0], son[1]);
        if(son[0]) son[0] -> rev ^= 1;
        if(son[1]) son[1] -> rev ^= 1;
        rev = 0;
    }

    /**
     * @param flag: +1 for insert, -1 for delete.
     */
    void modify(splayNode *u, int flag) {
        if(u) imag ^= u -> all;
    }
};

void rotate(splayNode *o) {
    int d = o -> dir();
    splayNode *p = o -> fa;
    o -> fa = p -> fa;
    if(!p -> isRoot()) p -> fa -> son[p -> dir()] = o;
    p -> son[d] = o -> son[d ^ 1];
    if(o -> son[d ^ 1]) o -> son[d ^ 1] -> fa = p;
    p -> fa = o;
    o -> son[d ^ 1] = p;
    p -> maintain();
}

void splay(splayNode *o) {
    static splayNode *stk[MX], **top = stk;
    *++top = o;
    for(auto u = o; !u -> isRoot(); u = u -> fa) *++top = u -> fa;
    while(top != stk) (*top--) -> pushdown();
    while(!o -> isRoot()) {
        splayNode *p = o -> fa;
        if(!p -> isRoot()) {
            if(o -> dir() == p -> dir()) rotate(p);
            else rotate(o);
        }
        rotate(o);
    }
    o -> maintain();
}

void access(splayNode *o) {
    for(splayNode *p = nullptr; o; p = o, o = o -> fa) {
        splay(o);
        o -> modify(o -> son[1], -1);
        o -> modify(p, 1);
        o -> son[1] = p;
        o -> maintain();
    }
}

void makeroot(splayNode *o) {access(o), splay(o), o -> rev = 1;}

void link(splayNode *u, splayNode *v) {
    makeroot(u), makeroot(v);
    u -> fa = v;
    v -> modify(u, 1);
    v -> maintain();
}

void split(splayNode *u, splayNode *v) {makeroot(u), access(v), splay(v);}

void cut(splayNode *u, splayNode *v) {
    split(u, v);
    v -> son[0] = u -> fa = nullptr;
    v -> maintain();
}

} // namespace LCT
