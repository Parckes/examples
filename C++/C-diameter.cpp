#include <iostream>
#include <stdio.h>
#include <vector>

int diametr(int N, std::vector<std::vector<int>>& graph) {

    std::vector<std::vector<int>> edge(N, std::vector<int>(N));

    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            if(graph[i][j] < 0 || i == j)
                edge[i][j] = INT32_MAX;
            else
                edge[i][j] = graph[i][j];

    for(int k = 0; k < N; k++)
        for(int i = 0; i < N; i++)
            for(int j = 0; j < N; j++) {
                if (i == j)
                    continue;
                if(edge[i][k] == INT32_MAX || edge[k][j] == INT32_MAX)
                    continue;
                edge[i][j] = std::min(edge[i][j], edge[i][k] + edge[k][j]);
            }

    int ans = 0;
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++) {
            if(i == j)
                continue;
            if(edge[i][j] == INT32_MAX)
                continue;
            ans = std::max(ans, edge[i][j]);
        }
    return ans;
}


int main() {

    int N;

    std::cin >> N;
    std::vector<std::vector<int>> graph(N, std::vector<int>(N));

    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            std::cin >> graph[i][j];

    std::cout << diametr(N, graph);
    return 0;
}
