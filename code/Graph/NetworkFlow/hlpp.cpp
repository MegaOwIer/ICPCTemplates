/**
 * Max flow in O(V^2 sqrt(E)).
 * WARNING: the points should be labled from 1 to V.
 */
template<typename flow_t = int>
struct MaxFlow_HLPP {
    const flow_t MAX_VAL = numeric_limits<flow_t>::max();

    struct edge_t {
        int to, rev;
        flow_t f;
        edge_t() {}
        edge_t(int _t, int _r, flow_t _f) : to(_t), rev(_r), f(_f) {}
    };

    vector<vector<edge_t>> G;
    int V, maxH, labelcnt, H;
    vector<int> cnt, h;
    vector<vector<int>> hq, gap;
    vector<flow_t> exflow;

    MaxFlow_HLPP(int v) : V(v), H(v + 1) {
        G.resize(v + 1);
        hq.resize(v + 2), gap.resize(v + 2);
        exflow.resize(v + 1);
    }

    void add_edge(int u, int v, flow_t f) {
        G[u].emplace_back(v, G[v].size(), f);
        G[v].emplace_back(u, G[u].size() - 1, 0);
    }
    
    void update(int u, int newH) {
        ++labelcnt;
        if(h[u] != H) --cnt[h[u]];
        h[u] = newH;
        if(newH == H) return;
        ++cnt[maxH = newH];
        gap[newH].push_back(u);
        if(exflow[u] > 0)
            hq[newH].push_back(u);
    }
    void globalRelabel(int T) {
        for(int i = 0; i <= H; i++)
            hq[i].clear(), gap[i].clear();
        h.assign(H, H);
        cnt.assign(H, 0);
        queue<int> Q;
        Q.push(T), h[T] = maxH = labelcnt = 0;
        while(!Q.empty()) {
            int u = Q.front();
            Q.pop();
            for(edge_t &e : G[u])
                if(h[e.to] == H && G[e.to][e.rev].f) {
                    update(e.to, h[u] + 1);
                    Q.push(e.to);
                }
            maxH = h[u];
        }
    }

    void push(int u, edge_t &e) {
        if(!exflow[e.to])
            hq[h[e.to]].push_back(e.to);
        flow_t delta = min(exflow[u], e.f);
        e.f -= delta, G[e.to][e.rev].f += delta;
        exflow[u] -= delta, exflow[e.to] += delta;
    }
    void discharge(int u) {
        if(h[u] == H) return;
        int mn = H;
        for(edge_t &e : G[u]) if(e.f) {
            if(h[u] - 1 == h[e.to]) {
                push(u, e);
                if(exflow[u] <= 0) return;
            } else {
                mn = min(mn, h[e.to] + 1);
            }
        }
        if(cnt[h[u]] > 1) update(u, mn);
        else {
            while(maxH >= h[u]) {
                for(int j : gap[maxH]) update(j, H);
                gap[maxH--].clear();
            }
        }
    }

    flow_t operator() (int S, int T) {
        fill(exflow.begin(), exflow.end(), 0);
        exflow[S] = MAX_VAL, exflow[T] = -MAX_VAL;
        globalRelabel(T);
        for(edge_t &e : G[S]) push(S, e);
        for(; ~maxH; --maxH) {
            while(!hq[maxH].empty()) {
                int u = hq[maxH].back();
                hq[maxH].pop_back();
                discharge(u);
                if(labelcnt > 4 * V) globalRelabel(T);
            }
        }
        return exflow[T] + MAX_VAL;
    }
};
