#include <cstdio>
#include <cinttypes>
#include <vector>
#include <iostream>

template <class T>
T max (T a, T b) {
    return a > b ? a : b;
}

template <class T>
T min (T a, T b) {
    return a < b ? a : b;
}

void build_seg_tree (std::vector<int64_t>& a, std::vector<std::pair<int64_t,int64_t>>& seg_tree,
                                        int64_t node, int64_t lnow, int64_t rnow) {
    if (lnow == rnow) {
        seg_tree[node].second = a[lnow];
        return;
    }
    int64_t m = (lnow + rnow) / 2;
    build_seg_tree (a, seg_tree, node * 2 + 1, lnow, m);
    build_seg_tree (a, seg_tree, node * 2 + 2, m + 1, rnow);
    seg_tree[node].second = max(seg_tree[node * 2 + 1].second,
                                 seg_tree[node * 2 + 2].second);
    return;
}

void push (std::vector<std::pair<int64_t,int64_t>>& seg_tree,
                                        int64_t node) {
    if (seg_tree[node].first != 0)
    {
        seg_tree[node * 2 + 1].first += seg_tree[node].first;
        seg_tree[node * 2 + 2].first += seg_tree[node].first;
        seg_tree[node].first = 0;
    }
}

void update_seg_tree (std::vector<std::pair<int64_t,int64_t>>& seg_tree, int64_t l, int64_t r, int64_t value,
                  int64_t node, int64_t nowl, int64_t nowr) {
    if (l > r)
        return;
    if (l == nowl && r == nowr) {
        seg_tree[node].first += value;
        return;
    }
    push (seg_tree, node);
    int64_t nowm = (nowl + nowr) / 2;
    update_seg_tree (seg_tree, l, min(nowm, r), value, node * 2 + 1, nowl, nowm),
    update_seg_tree (seg_tree, max(l, nowm + 1), r, value, node * 2 + 2, nowm + 1, nowr);
    seg_tree[node].second =
           max (seg_tree[2 * node + 1].second + seg_tree[2 * node + 1].first,
                seg_tree[2 * node + 2].second + seg_tree[2 * node + 2].first);
}

int64_t find_max (std::vector<std::pair<int64_t,int64_t>>& seg_tree,
              int64_t l, int64_t r, int64_t node, int64_t lnow, int64_t rnow) {
    if (l > r)
        return 0;
    if (lnow == rnow)
    {
        seg_tree[node].second = seg_tree[node].first + seg_tree[node].second;
        seg_tree[node].first = 0;
        return seg_tree[node].second;
    }
    if (l == lnow && r == rnow)
    {
        push(seg_tree, node);
        return (seg_tree[node].second =
               max (seg_tree[2 * node + 1].second + seg_tree[2 * node + 1].first,
                    seg_tree[2 * node + 2].second + seg_tree[2 * node + 2].first));

    }
    push (seg_tree, node);
    int64_t m = (lnow + rnow) / 2;
    return max (find_max (seg_tree, l, min(m, r), node * 2 + 1, lnow, m),
           find_max (seg_tree, max(l, m + 1), r, node * 2 + 2, m + 1, rnow));


}


int main () {
    int64_t N;
    scanf ("%ld", &N);
    std::vector<int64_t> a(N);
    std::vector<std::pair<int64_t,int64_t>> seg_tree(4 * N);
    for (int i = 0; i < N; ++i)
        scanf("%ld", &a[i]);
    build_seg_tree (a, seg_tree, 0, 0, N - 1);

    int64_t M;
    scanf ("%ld", &M);
    char ch;
    for (int i = 0; i < M; ++i) {
        std::cin >> ch;
        if (ch == 'm') {
            int64_t l, r;
            scanf("%ld%ld", &l, &r);
            printf ("%ld\n", find_max (seg_tree, l - 1, r - 1, 0, 0, N - 1));
        }
        if (ch == 'a') {
            int64_t l, r, value;
            scanf ("%ld %ld %ld", &l, &r, &value);
            update_seg_tree (seg_tree, l - 1, r - 1, value, 0, 0, N - 1);
            //for (int k = 0; k < 4 * N; ++k)
            //    printf("%d plus = %ld max = %ld\n", k, seg_tree[k].first, seg_tree[k].second);
            //printf ("!!!%ld + %ld %ld + %ld!!!!\n", seg_tree[3].second, seg_tree[3].first, seg_tree[8].second, seg_tree[8].first);
        }
    }


    return 0;
}
