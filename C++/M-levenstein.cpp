#include <iostream>
#include <vector>
#include <string>
#include <climits>

void init_matrix(std::vector<std::vector<int>>& edit_distance) {
    for (int i = 0; i < edit_distance.size(); ++i) {
        edit_distance[i][0] = i;
    }
    for (int j = 0; j < edit_distance[0].size(); ++j) {
        edit_distance[0][j] = j;
    }
}

void fill_distance_matrix(std::vector<std::vector<int>>& edit_distance,
                                            std::string s1, std::string s2) {
    init_matrix(edit_distance);
    char prev_matching_c = '-';
    for (int i = 0; i < s1.length(); ++i) {
        for (int j = 0; j < s2.length(); ++j) {
            int cand = edit_distance[i][j];
            if (s1[i] != s2[j]) {
                ++cand;
            }
            edit_distance[i + 1][j + 1] = std::min(cand,
                            std::min(edit_distance[i + 1][j] + 1, edit_distance[i][j + 1] + 1));
        }
    }
}

int main() {
    std::string first, second;
    std::cin >> first >> second;
    if (first.length() > second.length()) std::swap(first, second);
    /*matrix[i][j] = distance between first[1:i] and second[1:j] (1 - first char,
      not second) */
    std::vector<std::vector<int>> edit_distance(first.length() + 1,
                                  std::vector<int>(second.length() + 1, INT_MAX));

    fill_distance_matrix(edit_distance, first, second);
    std::cout << edit_distance[first.length()][second.length()] << "\n";
    return 0;
}
