/**
 * Return a vector of edge id (positive for go ahead, negative for go back)
 * WARNING: nodes are labeled from 0 to V-1, edges are labeled from 1 to E.
 */
class Eluer_Undirected {
    using edge_t = pair<int, int>;

    size_t E;
    vector<vector<edge_t>> G;
    vector<bool> vis;

    void dfs(int u) {
        while(!G[u].empty()) {
            auto [v, id] = G[u].back();
            G[u].pop_back();
            if(!vis[abs(id)]) {
                vis[abs(id)] = true;
                dfs(v);
                path.push_back(id);
            }
        }
    }

public:
    vector<int> path;

    Eluer_Undirected(int _v) : E(0) {
        G.resize(_v);
    }

    void add_edge(int u, int v) {
        E++;
        G[u].emplace_back(v, E);
        G[v].emplace_back(u, -E);
    }

    bool work() {
        for(const vector<edge_t> &neighbor : G) {
            if(neighbor.size() & 1) {
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
