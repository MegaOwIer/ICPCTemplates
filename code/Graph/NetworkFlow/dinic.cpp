/**
 * Max flow in O(V^2 sqrt(E)).
 * WARNING: the points should be labled from 1 to V.
 */
template<int V, int E>
struct MaxFlow_Dinic {
    using LL = long long;
    struct edge_t {
        int to, nxt;
        long long f;
        edge_t() {}
        edge_t(int t, int n, LL _f) : to(t), nxt(n), f(_f) {}
    };

    int tot, head[V + 1], cur[V + 1], dis[V + 1];
    bool vis[V + 1];
    edge_t e[E * 2 + 1];

    MaxFlow_Dinic() {reset();}
    void reset(int v = V) {
        tot = 1;
        memset(head + 1, 0, sizeof(int[v]));
    }

    void add_edge(int u, int v, LL f) {
        e[++tot] = edge_t(v, head[u], f), head[u] = tot;
        e[++tot] = edge_t(u, head[v], 0), head[v] = tot;
    }

    bool BFS(int S, int T) {
        queue<int> Q;
        memset(vis + 1, false, sizeof(bool[V]));
        memset(dis + 1, 0x3f, sizeof(int[V]));
        Q.push(S), vis[S] = true, dis[S] = 0;
        while(!Q.empty()) {
            int u = Q.front();
            Q.pop();
            for(int i = head[u]; i; i = e[i].nxt)
                if(!vis[e[i].to] && e[i].f) {
                    Q.push(e[i].to), vis[e[i].to] = true;
                    dis[e[i].to] = dis[u] + 1;
                }
        }
        return vis[T];
    }

    long long DFS(int u, LL flow, int T) {
        if(u == T || !flow) return flow;
        LL w, used = 0;
        for(int &i = cur[u]; i; i = e[i].nxt)
            if(dis[e[i].to] == dis[u] + 1 && e[i].f) {
                w = DFS(e[i].to, min(flow - used, e[i].f), T);
                e[i].f -= w, e[i ^ 1].f += w;
                used += w;
            }
        return used;
    }

    long long operator() (int S, int T) {
        long long ans = 0;
        while(BFS(S, T)) {
            memcpy(cur + 1, head + 1, sizeof(int[V]));
            ans += DFS(S, LLONG_MAX, T);
        }
        return ans;
    }
};
