#include <iostream>
#include <vector>

int graph[52][52];
bool exist[52], chosen[52], ans[52];
int w[52];
std::vector<int> v[52];
int best_cost = (int)1e9;
std::vector<int> bestcut;

int main() {
	int n;
    char c;
    std::cin>>n;
    for(int i = 0; i < n; i ++){
        for(int j = 0; j < n; j ++){
            std::cin>>c;
            if(c == '1')graph[i][j] = 1;
            else graph[i][j] = 0;
        }
        exist[i] = true;
        v[i].assign (1, i);
    }
	for (int ph = 0; ph < n - 1; ph ++) {
        for(int i = 0; i < n; i++){
            w[i] = 0;
            chosen[i] = false;
        }
        int prev;
		for (int i = 0; i < n - ph; i ++ ) {
			int selected = -1;
			for (int j = 0; j < n; j++)
				if (exist[j] && !chosen[j] && (selected == -1 || w[j] > w[selected]))
					selected = j;
			if (i == n - ph - 1) {
				if (w[selected] < best_cost){
					best_cost = w[selected];
                    bestcut = v[selected];
                }
				v[prev].insert (v[prev].end(), v[selected].begin(), v[selected].end());
				for (int j = 0; j < n; j++){
                    graph[j][prev] += graph[selected][j];
					graph[prev][j] = graph[j][prev];
                }
				exist[selected] = false;
			}
			else {
				chosen[selected] = true;
				for (int j = 0; j < n; j++)
					w[j] += graph[selected][j];
				prev = selected;
			}
		}
	}
	for(int i = 0; i < (int)bestcut.size(); i++){
	    ans[bestcut[i]] = true;
	}
	for(int i = 0; i < n; i++){
	    if(!ans[i])
	    std::cout<<i+1<<" ";
	}
	std::cout<<std::endl;
	for(int i = 0; i < n; i++){
	    if(ans[i])
	    std::cout<<i+1<<" ";
	}
	std::cout<<std::endl;
	return 0;
}
