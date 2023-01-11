#include <iostream>
#include <vector>
#include <map>
#include <queue>

int prim_algo(int &N, int &M, std::map<int, std::vector<std::pair<int, int>>> &graph ,int &s);

int main(){

    int N, M;
    std::cin >> N >> M;
    std::map<int, std::vector<std::pair<int, int>>> graph;
    int S, W, E;
    for(int i = 0; i < M; i++) {
        std::cin >> S >> E >> W;
        graph[S].push_back(std::make_pair(W, E));
        graph[E].push_back(std::make_pair(W, S));
    }

    std::cout << prim_algo(N, M, graph, S);
    return 0;
}

int prim_algo(int &N, int &M, std::map<int, std::vector<std::pair<int, int>>> &graph, int &s){
    std::vector<bool> used(N);
    int ans = 0;
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> q;
    q.push({0, s});

    while (!q.empty()){
        std::pair<int, int> cnt = q.top();
        q.pop();

        if(used[cnt.second])
            continue;

        used[cnt.second] = true;
        ans += cnt.first;

        for(int i = 0; i < graph[cnt.second].size(); i++) {
            if(used[graph[cnt.second][i].second])
                continue;
            q.push(graph[cnt.second][i]);
        }
    }
    return ans;
}
