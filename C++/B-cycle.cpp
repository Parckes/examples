#include <iostream>
#include <cinttypes>
#include <vector>
#include <unordered_set>
#include <stack>

void input_graph(std::vector<std::vector<int>>& graph, int m) {
    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        graph[a - 1].push_back(b - 1);
    }
}

void print_stack(std::stack<int> s) {
    while (s.size()) {
        std::cout << s.top() << " ";
        s.pop();
    }
}

void print_cycle(std::vector<int>& parent, int i, int j) {
    std::stack<int> s;
    while (i != j) {

        s.push(i + 1);
        //std::cout << i + 1 << " ";
        i = parent[i];
    }
    s.push(j + 1);
    print_stack(s);
    //std::cout << j + 1 << "\n";
}

std::vector<std::vector<int>> graph;
std::vector<char> colour;
std::vector<int> parent;

int is_cycled(int i) {

    colour[i] = 1;
    for (int j = 0; j < graph[i].size(); ++j) {
        if (colour[graph[i][j]] == 1) {
            std::cout << "YES\n";
            print_cycle(parent, i, graph[i][j]);
            return 1;
        }
        else if (colour[graph[i][j]] == 0) {
            parent[graph[i][j]] = i;
            if (is_cycled(graph[i][j])) return 1;
        }
    }
    colour[i] = 2;
    return 0;
}

int main() {

    size_t n, m;
    std::cin >> n >> m;

    graph = std::vector<std::vector<int>>(n, std::vector<int>());
    input_graph(graph, m);
    parent = std::vector<int>(n, -1);
    colour = std::vector<char>(n, 0);

    //std::vector<int> colour(n, 0);
    //std::vector<int> parent(n, -1);

    for (int i = 0; i < n; ++i) {
        if (colour[i] != 2 && is_cycled(i)) return 0;
    }
    std::cout << "NO\n";
    return 0;
}
