#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <string>
#include <algorithm>

std::vector<std::vector<int>> graph;

void build_graph(std::vector<std::string>& words, int n) {
    for (int i = 0; i < words.size(); ++i) {
        for (int j = i + 1; j < words.size(); ++j) {
            for (int l = 0; l < n; ++l) {
                if (words[i].substr(0, l) == words[j].substr(0, l) && l < words[i].size() &&
                              words[j].size() && words[i][l] != words[j][l] ) {
                    graph[words[i][l] - 'A'].push_back(words[j][l] - 'A');
                    //if (l == 0) {std::cout << words[j] << " " <<words[j][l] - 'A'<< "\n" ;}
                }
            }
        }
    }
}

std::vector<int> seen;
std::vector<int> answer;


void dfs (int v) {
  	seen[v] = 1;
  	for (int i = 0; i < graph[v].size(); ++i) {
    		int next = graph[v][i];
    		if (!seen[next])
      			dfs(next);
  	}
  	answer.push_back(v);
}

void topological_sort() {
  	for (int i = 0; i < graph.size(); ++i)
  		  seen[i] = 0;
  	answer.clear();
  	for (int i = 0; i < graph.size(); ++i)
    		if (!seen[i])
      			dfs (i);
  	std::reverse(answer.begin(), answer.end());
}

int main() {
    int n, m;
    std::cin >> n >> m;
    std::vector<std::string> words(m);
    for (int i = 0; i < m; ++i) std::cin >> words[i];

    graph = std::vector<std::vector<int>>(n, std::vector<int>());
    seen = std::vector<int>(n, 0);
    build_graph(words, n);
    topological_sort();
    
    for (auto x : answer) {
        std::cout << char(x + 'A');
    }
    std::cout << "\n";
    return 0;
}
