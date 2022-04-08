/**
 * Build suffix array in O(n) time.
 * WARNING: make sure S[n] is accessable and set to 0.
 * @param SIGMA size of alphabet
 */
class SuffixArray_SAIS {
    enum class suffix_type : int {
        L_type = 0,
        S_type = 1
    };

    vector<suffix_type> type;

    bool isLMS(size_t pos) {
        return pos && type[pos - 1] == suffix_type::L_type 
                   && type[pos] == suffix_type::S_type;
    }

    bool check_equal(const int *S, int x, int y) {
        do {
            if(S[x++] != S[y++]) return false;
        } while(!isLMS(x) && !isLMS(y));
        return S[x] == S[y];
    }

    void induced_sort(const int *S, const vector<int> &bucket, size_t n, size_t SIGMA) {
        sbucket = bucket;
        for(size_t i = 0; i <= n; i++) {
            if(SA[i] > 0 && type[SA[i] - 1] == suffix_type::L_type) {
                SA[lbucket[S[SA[i] - 1]]++] = SA[i] - 1;
            }
        }
        for(size_t i = n; i <= n; i--) {
            if(SA[i] > 0 && type[SA[i] - 1] == suffix_type::S_type) {
                SA[--sbucket[S[SA[i] - 1]]] = SA[i] - 1;
            }
        }
    }

public:
    vector<int> SA, rank, Height;
    vector<int> lbucket, sbucket;

    SuffixArray_SAIS(const int *S, size_t n, size_t SIGMA = 127) {
        type.resize(n + 1);
        type[n] = suffix_type::S_type;
        for(size_t i = n - 1; i < n; i--) {
            if(S[i] == S[i + 1]) {
                type[i] = type[i + 1];
            } else {
                type[i] = S[i] < S[i + 1] ? suffix_type::S_type : suffix_type::L_type;
            }
        }

        vector<size_t> position;
        for(size_t i = 1; i <= n; ++i) if(isLMS(i)) {
            position.push_back(i);
        }

        vector<int> bucket(SIGMA + 1, 0);
        for(size_t i = 0; i <= n; i++) bucket[S[i]]++;

        partial_sum(bucket.begin(), bucket.end(), bucket.begin());
        lbucket = {0}, sbucket = {1};
        lbucket.insert(lbucket.end(), bucket.begin(), bucket.begin() + SIGMA);
        sbucket.insert(sbucket.end(), bucket.begin() + 1, bucket.begin() + SIGMA + 1);

        SA.assign(n + 1, -1);
        for(size_t pos : position) SA[--sbucket[S[pos]]] = pos;
        induced_sort(S, bucket, n, SIGMA);

        vector<int> name(n + 1, -1);
        int lastx = -1, namecnt = 1;
        bool multiple = false;
        for(size_t i = 1; i <= n; i++) {
            int x = SA[i];
            if(isLMS(x)) {
                if(lastx != -1) {
                    if(!check_equal(S, x, lastx)) {
                        ++namecnt;
                    } else {
                        multiple = true;
                    }
                }
                name[x] = namecnt;
                lastx = x;
            }
        }
        name[n] = 0;

        vector<int> S1;
        copy_if(name.begin(), name.end(), back_inserter(S1), [](int val){ return val != -1; });

        vector<int> SA1;
        if(!multiple) {
            SA1.resize(position.size());
            for(size_t i = 0; i < position.size(); i++) {
                SA1[S1[i]] = i;
            }
        } else {
            SuffixArray_SAIS tmp(S1.data(), position.size() - 1, namecnt);
            SA1 = tmp.SA;
        }

        lbucket = {0}, sbucket = {1};
        lbucket.insert(lbucket.end(), bucket.begin(), bucket.begin() + SIGMA);
        sbucket.insert(sbucket.end(), bucket.begin() + 1, bucket.begin() + SIGMA + 1);

        SA.assign(n + 1, -1);
        reverse(SA1.begin(), SA1.end());
        for(int val : SA1) SA[--sbucket[S[position[val]]]] = position[val];
        induced_sort(S, bucket, n, SIGMA);
    }

    void calc_full(const int *S, size_t n) {
        rank.resize(n + 1);
        for(size_t i = 0; i <= n; i++) rank[SA[i]] = i;
        Height.resize(n + 1);
        for(size_t i = 0, j, k = 0; i < n; Height[rank[i++]] = k) {
            for(k = k ? k - 1 : k, j = SA[rank[i] - 1]; S[i + k] == S[j + k]; ++k);
        }
    }
};
