#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <string>
#include <climits>

bool check_indexes(int i, int j, int n) {
    return i >= 0 && j >= 0 && i < n && j < n;
}

void print_stack(std::stack<std::pair<int, int>>& s) {
    std::cout << s.size() << "\n";
    while (s.size()) {
        std::cout << char(s.top().first + 'A') << s.top().second + 1 << "\n";
        s.pop();
    }
    std::cout << "\n";
}

void restore_path(std::vector<std::vector<std::pair<int, int>>>& from
                                          , std::pair<int, int> finish) {

    std::stack<std::pair<int, int>> s;
    while (from[finish.first][finish.second].first != -1) {
      //std::cout <<"nigger is here\n";
        s.push(finish);
        finish = from[finish.first][finish.second];
    }

    print_stack(s);
}

void bfs(std::vector<std::vector<std::pair<int, int>>>& from
                                 , std::pair<int, int>& start
                                 , std::pair<int, int>& finish) {

    std::queue<std::pair<int, int>> q;
    std::vector<std::pair<int, int>> moves = {
        {1, 2}, {2, 1}, {1, -2}, {2, -1}
        , {-1, -2}, {-2, -1}, {-2, 1}, {-1, 2}
    };

    for (int i = 0; i <  moves.size(); ++i) {
        int next_i = start.first + moves[i].first;
        int next_j = start.second + moves[i].second;

        if (check_indexes(next_i, next_j, from.size())) {
            q.push({next_i, next_j});
            from[next_i][next_j] = start;
        }

    }
    while (q.size() && from[finish.first][finish.second].first == -1) {
        std::pair<int, int> elem = q.front();
        q.pop();
        for (int i = 0; i <  moves.size(); ++i) {
            int next_i = elem.first + moves[i].first;
            int next_j = elem.second + moves[i].second;

            if (check_indexes(next_i, next_j, from.size()) && start != std::make_pair(next_i, next_j)
                                                           && from[next_i][next_j].first == -1) {
                q.push({next_i, next_j});
                from[next_i][next_j] = elem;
            }

        }
    }

    if (from[finish.first][finish.second].first != -1) {

        restore_path(from, finish);
    }
    else {
        std::cout << "Wrong answer!!!!!!!\n";
    }
}

int main() {
    size_t n;
    std::cin >> n;
    std::string start_s;
    std::string finish_s;
    std::cin >> start_s >> finish_s;

    //from[i][j] - where did we get here from
    std::vector<std::vector<std::pair<int, int>>> from(n, std::vector<std::pair<int, int>>(n, std::make_pair(-1, -1)));

    std::pair<int, int> start  = {start_s[0] - 'A', std::stoi(start_s.substr(1, start_s.length() - 1)) - 1};
    std::pair<int, int> finish = {finish_s[0] - 'A', std::stoi(finish_s.substr(1, finish_s.length() - 1)) - 1};

    //std::cout << start.first << start.second << "\n" << finish.first << finish.second << "\n";
    if (start == finish) {
        std::cout << "0\n\n";
        return 0;
    }
    bfs(from, start, finish);

    return 0;
}
