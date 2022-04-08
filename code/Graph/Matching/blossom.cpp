/**
 * General matching in O(V^3).
 * WARNING: nodes should be labeled from 0 to V-1.
 */
class Blossom {
    int V;
    vector<vector<int>> G;
    vector<int> path, ufs, pre;

    int ufs_find(int u) { return u == ufs[u] ? u : ufs[u] = ufs_find(ufs[u]); }

public:
    vector<int> match;

    Blossom(int _v) : V(_v) {
        G.resize(_v);
        path.resize(_v);
        pre.assign(_v, -1);
        match.assign(_v, -1);
    }

    void add_edge(int u, int v) {
        G[u].push_back(v);
        G[v].push_back(u);
    }

    bool augment(int u) {
        vector<int> type(V, -1);
        
        queue<int> que;
        que.push(u), type[u] = 0;

        ufs.resize(V);
        for(int i = 0; i < V; i++) ufs[i] = i;

        function<int(int, int)> lca = [&] (int x, int y) {
            static int T = 0;
            for(T++; ; swap(x, y)) if(x != -1) {
                x = ufs_find(x);
                if(path[x] == T) return x;
                path[x] = T;
                x = match[x] == -1 ? -1 : pre[match[x]];
            }
        };

        function<void(int, int, int)> shrink = [&](int x, int y, int k) {
            while(ufs_find(x) != k) {
                pre[x] = y;
                int z = match[x];
                if(type[z] == 1) que.push(z), type[z] = 0;
                if(ufs_find(x) == x) ufs[x] = k;
                if(ufs_find(z) == z) ufs[z] = k;
                y = z, x = pre[y];
            }
        };

        while(!que.empty()) {
            int x = que.front();
            que.pop();
            for(int y : G[x]) {
                if(type[y] == -1) {
                    type[y] = 1, pre[y] = x;
                    if(match[y] == -1) {
                        for(int now = y, tmp, c; now != -1; now = tmp)
                            tmp = match[c = pre[now]], match[now] = c, match[c] = now;
                        return true;
                    }
                    type[match[y]] = 0;
                    que.push(match[y]);
                } else if (!type[y] && ufs_find(x) != ufs_find(y)) {
                    int p = lca(x, y);
                    shrink(x, y, p), shrink(y, x, p);
                }
            }
        }
        return false;
    }

    int work() {
        int ans = 0;
        for(int i = 0; i < V; i++) {
            if(match[i] == -1 && augment(i)) {
                ans++;
            }
        }
        return ans;
    }
};
