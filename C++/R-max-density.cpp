#include <cstring>
#include <iostream>
#include <vector>

#define infinity 1e10
#define epsilon 1e-7
#define max_n 2005
#define max_m 20005

std::vector<int> vtx(max_m), ne(max_m), L(max_n);
int tot;
std::vector<double> f(max_m);
bool mk[max_n], seen[max_n], cut[max_m];

void inside(int u, int v, double fl) {
    vtx[++tot] = v;
    f[tot] = fl;
    ne[tot] = L[u];
    L[u] = tot;

    vtx[++tot] = u;
    f[tot] = 0;
    ne[tot] = L[v];
    L[v] = tot;
}

std::vector<int> q(max_n), pre(max_n), x(max_n), y(max_n);
int S,  T,  now,  n,  m,  count, d[max_n];
double min_cut;

void push() {
    double fl = infinity;
    int i = T;
    while (i != S) {
      fl = std::min(fl, f[pre[i]]);
      i = vtx[pre[i] ^ 1];
    }

    min_cut += fl;
    i = T;
    while (i != S) {
      f[pre[i]] -= fl,  f[pre[i] ^ 1] += fl;
      if (f[pre[i]] < epsilon) now = vtx[pre[i] ^ 1];
      i = vtx[pre[i] ^ 1];
    }
}

void dinic(int u) {
    if (u == T) push();
    else {
        int p = L[u], v = vtx[p];
        while (p) {
          if (f[p] > epsilon && d[u] + 1 == d[v]) {
              pre[v] = p;
              dinic(v);
              if (d[now] < d[u])  return;
              now = T;
          }
          v = vtx[p = ne[p]];
        }
        d[u] = -1;
    }
}

bool expand() {
    memset(d,  63,  sizeof(d));
    q[1] = S;
    d[S] = 0;
    int h = 1,  t = 1,  u = q[h];
    while (h <= t) {
        int p = L[u], v = vtx[p];
        while (p) {
            if (f[p] > epsilon && d[v] > (1 << 29)) {
                d[v] = d[u] + 1;
                if (v == T) return 1;
                q[++t] = v;
            }
            v = vtx[p = ne[p]];
        }
        u = q[++h];
    }
    return 0;
}

double check(double g) {
    L = std::vector<int>(L.size(), 0);
    tot = 1;
    for (int i = 1; i <= n; ++i)
        inside(S, i, g);
    for (int i = 1; i <= m; ++i)
        inside(x[i], n + i, infinity), inside(y[i], n + i, infinity), inside(n + i, T, 1);
    for (min_cut = 0; expand(); dinic(S));
    return min_cut < m;
}

void find_cut() {
    q[1] = S;
    seen[S] = 1;
    int h = 1, t = 1, u = q[h];
    while (h <= t) {
        int p = L[u], v = vtx[p];
        while (p) {
            if (f[p] > 1e-4 && !seen[v]) seen[q[++t] = v] = 1;
            v = vtx[p = ne[p]];
        }
        u = q[++h];
    }
    for (int u = 1; u <= T; ++u) {
        int p = L[u], v = vtx[p];
        while (p) {
            if (!(p & 1) && (seen[u] ^ seen[v])) cut[p] = 1;
            v = vtx[p = ne[p]];
        }
    }
}
int main() {
    std::cin >> n >> m;
    if (!m) {
        std::cout << "1\n1\n";
        return 0;
    }
    for (int i = 1; i <= m; ++i)
        std::cin >> x[i] >> y[i];
    S = n + m + 1;
    T = S + 1;
    int time = 24;
    double left = 1 / n, right = m;
    double mid = (left + right) / 2;
    while(time) {
        if (check(mid)) left = mid;
        else right = mid;
        mid = (left + right) / 2;
        --time;
    }
    check(left);
    find_cut();
    int p = L[S], v = vtx[p];
    while (p) {
        if (cut[p]) {
            ++count;
            mk[v] = 1;
        }
        v = vtx[p = ne[p]];
    }

    std::cout << count << "\n";
    for (int i = 1;  i <= n;  ++i) {
        if (mk[i]) std::cout << i << "\n";
    }
    return 0;
}
