#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <cinttypes>


int64_t bfs(int start_pos, int end_pos, std::vector<std::vector<int64_t>>& connections) {
    int n = connections.size();
    if (start_pos == -1 || end_pos == -1) return -1;

    std::vector<int64_t> to_left(n, -1);
    std::vector<int64_t> to_right(n, -1);
    to_left[start_pos] = 0;
    to_right[end_pos] = 0;
    std::queue<int64_t> q_left;
    std::queue<int64_t> q_right;
    q_left.push(start_pos);
    q_right.push(end_pos);

    int64_t left, right;

    while (q_left.size() && q_right.size()) {
        left = q_left.front();
        right = q_right.front();

        if (to_right[right] >= to_left[left]) {
            q_left.pop();
            if (to_right[left] != -1) {
                return to_left[left] + to_right[left];
            }

            for (auto x : connections[left]) {
                if (to_left[x] == -1) {
                    to_left[x] = to_left[left] + 1;
                    q_left.push(x);
                }
                if (to_right[x] != -1) {
                    return to_left[x] + to_right[x];
                }
            }
        }
        else
        {
            q_right.pop();
            if (to_left[right] != -1) {
                return to_left[right] + to_right[right];
            }
            for (auto x : connections[right]) {
                if (to_right[x] == -1) {
                    to_right[x] = to_right[right] + 1;
                    q_right.push(x);
                }
                if (to_left[x] != -1) {
                    return to_left[x] + to_right[x];
                }
            }
        }
    }
    return -1;
}


int which_ind(std::string& str, std::vector<std::string>& words_list) {
    for (int i = 0; i < words_list.size(); ++i) {
        if (str == words_list[i]) {
            return i;
        }
    }
    return -1;
}

int cross(int f, int s, std::vector<std::string>& words_list) {
    int res = 0;
    for (int i = 0; i < words_list[f].size(); ++i) {
        if (words_list[f][i] != words_list[s][i]) ++res;
    }
    return res;
}



int main() {
    int n, m;
    std::cin >> n;
    std::vector<std::string> words_list(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> words_list[i];
    }
    std::vector<std::vector<int64_t>> connections(n);
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (cross(i, j, words_list) == 1) {
                connections[i].push_back(j);
                connections[j].push_back(i);
            }
        }
    }
    std::cin >> m;
    std::vector<int> res;
    for (int i = 0; i < m; ++i) {
        std::string from, to;
        std::cin >> from >> to;
        res.push_back(bfs(which_ind(from, words_list), which_ind(to, words_list), connections));
    }
    for (auto x : res) std::cout << x << "\n";
    return 0;
}S
