#include <iostream>
#include <cinttypes>
#include <vector>
#include <queue>
#include <unordered_set>

void input_graph(std::vector<std::vector<int>>& graph, int m) {
    for (int i = 0; i < m; ++i) {
        int a, b;
        std::cin >> a >> b;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }
}

int is_bipartite(std::vector<std::vector<int>>& graph) {
    int not_visited = graph.size();
    std::vector<int> colour(graph.size(), -1); //odd, even, not visited
    while (not_visited) {
        int start = -1;
        for (int i = 0; i < graph.size(); ++i) {
            if (colour[i] == -1) {
                start = i;
                break;
            }
        }
        //index of next vertice and colour of parent
        std::queue<std::pair<int, int>> q;
        q.push({start, 2});
        std::unordered_set<int> seen;
        seen.insert(start);
        while (q.size()) {
            std::pair<int, int> cur_elem = q.front();
            q.pop();
            --not_visited;
            if (colour[cur_elem.first] == -1) {
                colour[cur_elem.first] = cur_elem.second + 1;
            }
            else if (colour[cur_elem.first] % 2 == cur_elem.second % 2) return 0;
            
            for (int i = 0; i < graph[cur_elem.first].size(); ++i) {
                if (!seen.count(graph[cur_elem.first][i])) {
                    seen.insert(graph[cur_elem.first][i]);
                    q.push({graph[cur_elem.first][i], cur_elem.second + 1});
                }
                else if (colour[graph[cur_elem.first][i]] % 2 == colour[cur_elem.first] % 2)  {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    std::vector<std::vector<int>> graph(n, std::vector<int>());
    input_graph(graph, m);
    is_bipartite(graph) ? std::cout << "YES\n" : std::cout << "NO\n";
    return 0;
}
