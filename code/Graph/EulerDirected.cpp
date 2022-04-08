/**
 * Return a vector of edge id.
 * WARNING: nodes are labeled from 0 to V-1, edges are labeled from 1 to E.
 */
class Eluer_Directed {
    using edge_t = pair<int, int>;

    size_t E;
    vector<vector<edge_t>> G;
    vector<size_t> ind;
    vector<bool> vis;

    void dfs(int u) {
        while(!G[u].empty()) {
            auto [v, id] = G[u].back();
            G[u].pop_back();
            if(!vis[id]) {
                vis[id] = true;
                dfs(v);
                path.push_back(id);
            }
        }
    }

public:
    vector<int> path;

    Eluer_Directed(int _v) : E(0) {
        G.resize(_v);
        ind.assign(_v, 0);
    }

    void add_edge(int u, int v) {
        ind[v]++, E++;
        G[u].emplace_back(v, E);
    }

    bool work() {
        for(size_t i = 0; i < G.size(); i++) {
            if(ind[i] != G[i].size()) {
                return false;
            }
        }
        vis.assign(E + 1, false);
        for(size_t i = 0; i < G.size(); i++) {
            if(!G[i].empty()) {
                dfs(i);
                break;
            }
        }
        reverse(path.begin(), path.end());
        return path.size() == E;
    }
};
