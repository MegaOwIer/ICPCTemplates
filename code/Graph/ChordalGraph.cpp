/** Chordal Graph: A graph without simple cycle of length >=4.
 * WARNING: the points should be labled from 0 to V-1.
 */
template<int V>
class ChordalGraph {
    using pii = pair<int, int>;
    using Heap = __gnu_pbds::priority_queue<pii, less<pii>, __gnu_pbds::thin_heap_tag>;

    set<int> G[V];
    vector<int> elimination_array;
    int deg[V], index[V];

public:

    void add_edge(int u, int v) {
        G[u].insert(v), G[v].insert(u);
    }

    void get_elimination_array(int n) {
        static Heap::point_iterator pos[V];
        static bool vis[V];
        Heap heap;
        memset(vis, false, sizeof(bool[n]));
        for(int i = 0; i < n; i++) pos[i] = heap.push({deg[i] = 0, i});
        while(!heap.empty()) {
            int u = heap.top().second;
            heap.pop();
            elimination_array.push_back(u);
            vis[u] = true;
            for(int v : G[u]) if(!vis[v]) {
                heap.modify(pos[v], {++deg[v], v});
            }
        }
        reverse(elimination_array.begin(), elimination_array.end());
        for(int i = 0; i < n; i++) index[elimination_array[i]] = i;
    }

    bool judge() {
        if(elimination_array.empty()) {
            get_elimination_array();
        }
        for(int u : elimination_array) {
            vector<int> pts;
            for(int v : G[u]) if(index[v] > index[u]) {
                pts.push_back(v);
                if(index[v] < index[pts.front()]) {
                    swap(pts.front(), pts.back());
                }
            }
            if(pts.size() < 2) continue;
            int v = pts.front();
            for(size_t i = 1; i < pts.size(); i++) {
                if(!G[v].count(pts[i])) return false;
            }
        }
        return true;
    }
};
