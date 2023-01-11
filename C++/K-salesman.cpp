#include <iostream>
#include <vector>
#include <string>
#include <climits>

void cin_matrix(std::vector<std::vector<std::pair<int, int>>>& matrix) {
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[0].size(); ++j) {
            std::cin >> matrix[i][j].first;
        }
    }
}


void fill_route_matrix(std::vector<std::vector<std::pair<int, int>>>& matrix) {
    for (int j = matrix[0].size() - 2; j >= 0; --j) {
        for (int i = 0; i < matrix.size(); ++i) {

            int cand1 = INT_MAX;
            int cand2 = matrix[i][j + 1].first;
            int cand3 = INT_MAX;
            int min_cand;
            int next_city;

            if (i > 0) cand1 = matrix[i - 1][j + 1].first;
            if (cand2 < cand1) {
                next_city = i;
                min_cand = cand2;
            }
            else {
                next_city = i - 1;
                min_cand = cand1;
            }
            if (i < matrix.size() - 1) cand3 = matrix[i + 1][j + 1].first;

            if (cand3 < min_cand) {
                min_cand = cand3;
                next_city = i + 1;
            }

            matrix[i][j].first += min_cand;
            matrix[i][j].second = next_city;
        }
    }
}

/*void cout_matrix(std::vector<std::vector<std::pair<int, int>>>& matrix) {
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[0].size(); ++j) {
            std::cout << matrix[i][j].second << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}*/

void restore_route(std::vector<std::vector<std::pair<int, int>>>& matrix) {
    int length_of_route = matrix[0][0].first;
    int cur_city = 0;
    int next_city;
    for (int i = 1; i < matrix.size(); ++i) {
        if (matrix[i][0].first < matrix[cur_city][0].first) {
            cur_city = i;
            next_city = matrix[i][0].second;
            length_of_route = matrix[i][0].first;
        }
    }
    std::cout << cur_city + 1 << " ";
    int j = 0;
    while (j < matrix[0].size() - 1) {
        std::cout << matrix[cur_city][j].second + 1 << " ";
        cur_city = matrix[cur_city][j].second;
        ++j;
    }
    std::cout << "\n";
    std::cout << length_of_route << "\n";
}

int main() {
    size_t n, m;
    std::cin >> n >> m;
    std::vector<std::vector<std::pair<int, int>>> matrix(n, std::vector<std::pair<int, int>>(m, {0, -1}));
    cin_matrix(matrix);

    fill_route_matrix(matrix);
    restore_route(matrix);
    //cout_matrix(matrix);
    return 0;
}
