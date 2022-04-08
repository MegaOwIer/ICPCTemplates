/** Optimize EK by replacing SPFA to Johnson-Dijkstra.
 * Min cost max flow in O(VE + E \log E f), where f is the max-flow of the graph.
 * WARNING: the points should be labled from 0 to V-1.
 */
template<typename flow_t = int, typename cost_t = int>
class MinCostMaxFlow_Johnson {
    struct edge_t {
        int to, rev;
        flow_t flow;
        cost_t cost;
        edge_t(int t, flow_t f, cost_t c, int r) : to(t), rev(r), flow(f), cost(c) {}
    };

    vector<vector<edge_t>> G;
    vector<pair<int, int>> from;
    vector<cost_t> dis, p;
    vector<bool> vis;
    int V;

    bool upd_min(cost_t &u, cost_t v) {
        if(u > v) return u = v, true;
        return false;
    }
    void SPFA(int T) {
        p.assign(V, numeric_limits<cost_t>::max() >> 1);
        vis.assign(V, false);
        queue<int> q;
        q.push(T), vis[T] = true, p[T] = 0;
        while(!q.empty()) {
            int u = q.front();
            q.pop(), vis[u] = false;
            for(edge_t &cur : G[u]) {
                int v = cur.to, r = cur.rev;
                if(G[v][r].flow && upd_min(p[v], p[u] + G[v][r].cost)) {
                    if(!vis[v]) q.push(v), vis[v] = true;
                }
            }
        }
    }
    bool Dijkstra(int T, int S) {
        dis.assign(V, numeric_limits<cost_t>::max() >> 1);
        vis.assign(V, false);
        using pii = pair<int, int>;
        priority_queue<pii, vector<pii>, greater<pii>> heap;
        heap.emplace(dis[T] = 0, T);
        while(!heap.empty()) {
            int u = heap.top().second;
            heap.pop();
            if(vis[u]) continue;
            vis[u] = true;
            for(size_t i = 0; i < G[u].size(); i++) {
                int v = G[u][i].to, r = G[u][i].rev;
                if(G[v][r].flow && upd_min(dis[v], dis[u] + G[v][r].cost + p[u] - p[v])) {
                    heap.emplace(dis[v], v);
                    from[v] = {u, static_cast<int>(i)};
                }
            }
        }
        return vis[S];
    }

public:
    MinCostMaxFlow_Johnson(int _V) : V(_V) {
        G.resize(V);
        from.resize(V);
    }

    void add_edge(int u, int v, flow_t f, cost_t c) {
        G[u].emplace_back(v, f, c, static_cast<int>(G[v].size()));
        G[v].emplace_back(u, 0, -c, static_cast<int>(G[u].size() - 1));
    }
    pair<flow_t, cost_t> operator() (int S, int T) {
        flow_t total_flow = 0;
        cost_t total_cost = 0;
        SPFA(T);
        while(Dijkstra(T, S)) {
            for(int i = 0; i < V; i++) p[i] += dis[i];
            flow_t cur_flow = numeric_limits<flow_t>::max();
            for(int i = S; i != T; i = from[i].first) {
                int u = from[i].first, id = from[i].second;
                cur_flow = min(cur_flow, G[i][G[u][id].rev].flow);
            }
            total_flow += cur_flow;
            total_cost += cur_flow * p[S];
            for(int i = S; i != T; i = from[i].first) {
                int u = from[i].first, id = from[i].second;
                G[u][id].flow += cur_flow;
                G[i][G[u][id].rev].flow -= cur_flow;
            }
        }
        return {total_flow, total_cost};
    }
};