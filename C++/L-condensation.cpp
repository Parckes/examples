#include <iostream>
#include <vector>
#include <map>
#include <set>



std::vector<std::vector<int>> graph, reversed;
std::vector<int> order, components;
std::vector<int> seen;
std::set<std::pair<int, int>> cond;

void input_graph(int m) {
    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        graph[a - 1].push_back(b - 1);
        reversed[b - 1].push_back(a - 1);
    }
}

void dfs_normal(int v) {
    seen[v] = true;
    for (int i = 0; i < graph[v].size(); ++i) {
        int to = graph[v][i];
        if (!seen[to])
            dfs_normal(to);
    }
    order.push_back(v);
}

void dfs_reversed(int v, int comp_n) {
    seen[v] = true;
    components[v] = comp_n;
    for (int i = 0; i < reversed[v].size(); ++i) {
        int to = reversed[v][i];
        if (!seen[to])
            dfs_reversed(to, comp_n);
    }
}

void find_edges() {
    int n = graph.size();
    for (int i = 0; i < n; ++i) {
        if (!seen[i]) {
            dfs_normal(i);
        }
    }

    seen = std::vector<int>(n, 0);
    int comp_n = 1;
    for (int i = 0; i < n; ++i) {
        int v = order[n - 1 - i];
        if (!seen[v]) {
            dfs_reversed(v, comp_n);
            comp_n++;
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < graph[i].size(); j++) {
            int to = graph[i][j];
            if (components[i] != components[to])
                cond.insert({components[i], components[to]});
        }
    }
}

int main() {
    int n, m;
    std::cin >> n >> m;

    graph = std::vector<std::vector<int>>(n, std::vector<int>());
    reversed = std::vector<std::vector<int>>(n, std::vector<int>());

    seen = std::vector<int>(n, 0);
    components = std::vector<int>(n, -1);

    input_graph(m);

    find_edges();

    std::cout << cond.size();
    return 0;
}
