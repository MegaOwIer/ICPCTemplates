/**
 * Min cost max flow in O(VEf), where f is the max-flow of the graph.
 * WARNING: the points should be labled from 0 to V-1.
 */
template<typename flow_t = int, typename cost_t = int>
class MinCostMaxFlow_EK {
    struct edge_t {
        int to, rev;
        flow_t flow;
        cost_t cost;
        edge_t(int t, flow_t f, cost_t c, int r) : to(t), rev(r), flow(f), cost(c) {}
    };

    vector<vector<edge_t>> G;
    vector<pair<int, int>> from;
    vector<cost_t> dis;
    vector<bool> in_queue;
    int V;

    bool upd_min(cost_t &u, cost_t v) {
        if(u > v) return u = v, true;
        return false;
    }
    bool SPFA(int T, int S) {
        dis.assign(V, numeric_limits<cost_t>::max() >> 1);
        in_queue.assign(V, false);
        queue<int> q;
        q.push(T), in_queue[T] = true, dis[T] = 0;
        while(!q.empty()) {
            int u = q.front();
            q.pop(), in_queue[u] = false;
            for(size_t i = 0; i < G[u].size(); i++) {
                int v = G[u][i].to, r = G[u][i].rev;
                if(G[v][r].flow && upd_min(dis[v], dis[u] + G[v][r].cost)) {
                    from[v] = {u, static_cast<int>(i)};
                    if(!in_queue[v]) q.push(v), in_queue[v] = true;
                }
            }
        }
        return dis[S] != numeric_limits<cost_t>::max() >> 1;
    }

public:
    MinCostMaxFlow_EK(int _V) : V(_V) {
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
        while(SPFA(T, S)) {
            flow_t cur_flow = numeric_limits<flow_t>::max();
            for(int i = S; i != T; i = from[i].first) {
                int u = from[i].first, id = from[i].second;
                cur_flow = min(cur_flow, G[i][G[u][id].rev].flow);
            }
            total_flow += cur_flow;
            total_cost += cur_flow * dis[S];
            for(int i = S; i != T; i = from[i].first) {
                int u = from[i].first, id = from[i].second;
                G[u][id].flow += cur_flow;
                G[i][G[u][id].rev].flow -= cur_flow;
            }
        }
        return {total_flow, total_cost};
    }
};