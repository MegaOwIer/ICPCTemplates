/**
 * Max flow in O(V^2 E).
 * WARNING: the points should be labled from 1 to V.
 */
template<typename flow_t = int>
struct MaxFlow_ISAP {
    const flow_t MAX_VAL = numeric_limits<flow_t>::max();

    struct edge_t {
        int to, rev;
        flow_t f;
        edge_t() {}
        edge_t(int _t, int _r, flow_t _f) : to(_t), rev(_r), f(_f) {}
    };

    vector<vector<edge_t>> G;
    vector<typename vector<edge_t>::iterator> cur;
    int V;
    vector<int> gap, dep;

    MaxFlow_ISAP(int v) : G(v + 1), cur(v + 1), V(v) {}

    void add_edge(int u, int v, flow_t f) {
        G[u].emplace_back(v, G[v].size(), f);
        G[v].emplace_back(u, G[u].size() - 1, 0);
    }

    void BFS(int T) {
        gap.assign(V + 2, 0);
        dep.assign(V + 1, 0);
        queue<int> Q;
        Q.push(T), ++gap[dep[T] = 1];
        while(!Q.empty()) {
            int u = Q.front();
            Q.pop();
            for(edge_t &e : G[u]) {
                int v = e.to;
                if(dep[v]) continue;
                Q.push(v), ++gap[dep[v] = dep[u] + 1];
            }
        }
    }

    flow_t dfs(int u, flow_t flow, const int S, const int T) {
        if(u == T || !flow) return flow;
        flow_t w, used = 0;
        for(auto &i = cur[u]; i != G[u].end(); i++) {
            if(i -> f && dep[i -> to] == dep[u] - 1) {
                w = dfs(i -> to, min(flow - used, i -> f), S, T);
                i -> f -= w, G[i -> to][i -> rev].f += w;
                used += w;
            }
            if(used == flow) return flow;
        }
        if(!--gap[dep[u]]) dep[S] = V + 1;
        if(dep[u] <= V) ++dep[u];
        ++gap[dep[u]], cur[u] = G[u].begin();
        return used;
    }

    flow_t operator() (int S, int T) {
        flow_t ans = 0;
        BFS(T);
        while(dep[S] <= V) {
            for(int i = 1; i <= V; i++) cur[i] = G[i].begin();
            ans += dfs(S, MAX_VAL, S, T);
        }
        return ans;
    }
};
