首先要把 split graph 的团和独立集分出来。思路如下，把点按度数从大到小排序，然后把度数相等的点一批一批加进来。如果有一批不能完全加入，
则至多只能加入一个点，而且后面的批次都不能加入了。证明如下：假设团的大小为 k，则团内点的度数 ≥k−1，团外点的度数 ≤k−1。
如果出现了第一批点（度数为 d）不能完全加进来，则至多只能加进来一个
分完以后就比较直观了。最大团计数初始有一个，然后枚举外面的点，一旦发现该点与团内 n−1 个点相连，它就可以跟团内剩下的那个点换，
计数加 1。最大独立集计数枚举团内的点，要看存不存在有一个点跟外面没有点相连，如果有独立集大小就是 n−clique+1，
计数是团内不跟外面相连的点的个数；否则独立集大小是 n−clique，计数是团内跟外面只有一个点相连的点的个数 + 1。 
#include <bits/stdc++.h>
using namespace std;
using LL = long long;
#define FOR(i, x, y) for (decay<decltype(y)>::type i = (x), _##i = (y); i < _##i; ++i)
#define FORD(i, x, y) for (decay<decltype(x)>::type i = (x), _##i = (y); i > _##i; --i)
#ifdef zerol
#define dbg(args...) do { cout << "\033[32;1m" << #args << " -> "; err(args); } while (0)
#else
#define dbg(...)
#endif
void err() { cout << "\033[39;0m" << endl; }
template<template<typename...> class T, typename t, typename... Args>
void err(T<t> a, Args... args) { for (auto x: a) cout << x << ' '; err(args...); }
template<typename T, typename... Args>
void err(T a, Args... args) { cout << a << ' '; err(args...); }
// -----------------------------------------------------------------------------

const int N = 1E5 + 100;
vector<int> g[N];
int deg[N];
vector<pair<int, int>> es;
vector<int> d[N];
bool clique[N];
int n, m, inside[N], outside[N];


int gr[20][20];
LL count_clique() {
    int cnt = 0, maxcliq = 0;
    FOR (i, 0, 1 << n) {
        bool ok = true;
        FOR (j, 0, n) if ((1 << j) & i) {
            FOR (k, j + 1, n) if ((1 << k) & i) {
                if (!gr[j][k]) ok = false;
            }
        }
        if (ok) {
            int sz = __builtin_popcount(i);
            if (sz > maxcliq) { maxcliq = sz; cnt = 0; }
            if (sz == maxcliq) cnt++;
        }
    }
    return cnt;
}

void check(LL ans1, LL ans2) {
    memset(gr, 0, sizeof gr);
    for (auto& e: es) {
        int u = e.first, v = e.second;
        gr[u - 1][v - 1] = gr[v - 1][u - 1] = 1;
    }
    // dbg(count_clique());
    assert (count_clique() == ans1);
    FOR (i, 0, n) FOR (j, 0, n) gr[i][j] = 1 - gr[i][j];
    // dbg(count_clique());
    assert (count_clique() == ans2);
}

int main() {
    int T; cin >> T;
    while (T--) {
        cin >> n >> m;
        fill(deg, deg + n + 1, 0);
        fill(inside, inside + n + 1, 0);
        fill(outside, outside + n + 1, 0);
        fill(clique, clique + n + 1, 0);

        int clique_size = 0;
        FOR (i, 1, n + 1) g[i].clear();
        FOR (i, 0, n + 1) d[i].clear();
        es.clear();
        FOR (i, 0, m) {
            int u, v; scanf("%d%d", &u, &v);
            g[u].push_back(v); g[v].push_back(u);
            es.push_back({u, v}); es.push_back({v, u});
            deg[u]++; deg[v]++;
        }

        FOR (i, 1, n + 1)
            d[deg[i]].push_back(i);
        FORD (dd, n, -1) {
            bool fail = false;
            for (int& v: d[dd]) {
                int cnt = 0;
                for (int &x: g[v]) {
                    if (clique[x]) cnt++;
                }
                if (cnt == clique_size) { clique[v] = 1; clique_size++; }
                else fail = true;
            }
            //v if (fail) break;
        }
        // dbg(clique);
        // FOR (i, 1, n + 1) dbg(i, clique[i]);

        LL ans1 = 1, ans2 = 1;

        for (auto& e: es) {
            int u = e.first, v = e.second;
            if (clique[u] && !clique[v]) {
                outside[v]++;
                inside[u]++;
            }
        }

        FOR (v, 1, n + 1) {
            if (clique[v]) continue;
            if (outside[v] == clique_size - 1)
                ans1++;
        }

        LL c0 = 0, c1 = 0;
        FOR (v, 1, n + 1) {
            if (!clique[v]) continue;
            // if (inside[v] == 0) get_avail = 1;
            c0 += inside[v] == 0;
            c1 += inside[v] == 1;
        }
        if (c0) ans2 = c0;
        else ans2 = c1 + 1;

        printf("%lld %lld\n", ans1, ans2);

        // check(ans1, ans2);
    }
    return 0;
}
