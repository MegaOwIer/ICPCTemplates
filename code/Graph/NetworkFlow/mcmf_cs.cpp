/**
 * Min cost max flow in O(V E^2 log(U)) where U is the upper bound of capacity.
 * WARNING: the points should be labled from 0 to V-1.
 */
struct MinCostMaxFlow_CS {
    using flow_t = long long;
    const flow_t inf = 0x3f3f3f3f3f3f3f3fLL;

    struct edge_t {
        int to, rev, from;
        flow_t cost, raw_cap, cur_cap;
        edge_t() {}
        edge_t(int _u, int _v, int _r, flow_t _cap, flow_t _cost) 
            : to(_v), rev(_r), from(_u), cost(_cost), raw_cap(_cap), cur_cap(0) {}
    };

    vector<vector<edge_t>> G;
    vector<typename vector<edge_t>::iterator> cur, pre;
    vector<flow_t> dis;
    int V;

    MinCostMaxFlow_CS(int _V) : G(_V), cur(_V), V(_V) {}

    void add_edge(int u, int v, flow_t cap, flow_t cost) {
        int pos_id = (int)G[u].size();
        G[u].emplace_back(u, v, 0, cap, cost);
        G[u].back().rev = (int)G[v].size();
        G[v].emplace_back(v, u, pos_id, 0, -cost);
    }

    void SPFA(int s) {
        static vector<bool> inq;
        inq.assign(V, false), inq[s] = true;
        dis.assign(V, inf), dis[s] = 0;
        pre.resize(V);

        queue<int> q;
        q.emplace(s);
        while(!q.empty()) {
            int u = q.front();
            q.pop(), inq[u] = false;
            
            for(auto it = G[u].begin(); it != G[u].end(); ++it) {
                int v = it->to;
                flow_t w = it->cost;
                if(it->cur_cap && dis[v] > dis[u] + w) {
                    dis[v] = dis[u] + w;
                    pre[v] = it;
                    if(!inq[v]) {
                        q.emplace(v), inq[v] = true;
                    }
                }
            }
        }
    }

    void add_cap(edge_t &e) {
        size_t u = e.from, v = e.to;
        if(e.cur_cap) {
            e.cur_cap += 1;
            return;
        }
        SPFA((int)v);
        if(dis[u] != inf && dis[u] + e.cost < 0) {
            G[v][e.rev].cur_cap += 1;
            while(u != v) {
                int f = pre[u]->from;
                pre[u]->cur_cap -= 1;
                G[u][pre[u]->rev].cur_cap += 1;
                u = f;
            }
        } else {
            e.cur_cap += 1;
        }
    }

    pair<flow_t, flow_t> operator() (int s, int t, int max_bit) {
        add_edge(t, s, inf, -inf);
        for(int i = max_bit; i >= 0; i--) {
            for(vector<edge_t> &tmp : G) {
                for(edge_t &e : tmp) e.cur_cap <<= 1;
            }
            for(int u = 0; u < V; u++) {
                for(edge_t &e : G[u]) if((e.raw_cap >> i) & 1) {
                    add_cap(e);
                }
            }
        }

        flow_t tot_cost = 0;
        for(int u = 0; u < V; u++) if(u != t) {
            for(edge_t &e : G[u]) if(e.raw_cap) {
                tot_cost += e.cost * (e.raw_cap - e.cur_cap);
            }
        }
        return {G[s].back().cur_cap, tot_cost};
    }
};
