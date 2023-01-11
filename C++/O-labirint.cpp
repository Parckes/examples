#include <iostream>
#include <vector>
#include <climits>

void cin_matrix(std::vector<std::vector<char>>& matrix) {
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[0].size(); ++j) {
            std::cin >> matrix[i][j];
        }
    }
}

std::pair<int, int> find_start(std::vector<std::vector<char>>& matrix) {
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[0].size(); ++j) {
            if (matrix[i][j] == '@') return {i, j};
        }
    }
    return {-1, -1};
}

std::pair<int, int> find_finish(std::vector<std::vector<char>>& matrix) {
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[0].size(); ++j) {
            if (matrix[i][j] == '#') return {i, j};
        }
    }
    return {-1, -1};
}

int check_cell(std::vector<std::vector<char>>& maze, int i, int j) {
    return i >= 0 && j >= 0 && i < maze.size() && j < maze[0].size();
}


uint64_t count_paths(std::vector<std::vector<std::vector<uint64_t>>>& count,
                                        std::vector<std::vector<char>>& maze) {
    std::pair<int, int> start = find_start(maze);
    std::pair<int, int> finish = find_finish(maze);
    ++count[start.first][start.second][0];
    //std :: cout << "here " << count[start.first][start.second][0] << " \n";
    for (int k = 1; k < count[0][0].size(); ++k) {
        for (int i = 0; i < count.size(); ++i) {
            for (int j = 0; j < count[0].size(); ++j) {
                uint64_t plus = 0;
                if (maze[i][j] == '+') continue;
                if (check_cell(maze, i + 1, j)) plus += count[i + 1][j][k - 1];
                if (check_cell(maze, i, j + 1)) plus += count[i][j + 1][k - 1];
                if (check_cell(maze, i - 1, j)) plus += count[i - 1][j][k - 1];
                if (check_cell(maze, i, j - 1)) plus += count[i][j - 1][k - 1];
                count[i][j][k] += plus;
            }
        }
    }
    return count[finish.first][finish.second].back();
}




int main () {
    size_t n, m, k;
    uint64_t w;
    std::cin >> n >> m >> k;
    std::vector<std::vector<char>> maze(n, std::vector<char>(m));
    cin_matrix(maze);
    //count[i][j][k] - кол-во способов пройти в клетку i j за k ходов
    std::vector<std::vector<std::vector<uint64_t>>> count(n,
                                              std::vector<std::vector<uint64_t>>(m,
                                                    std::vector<uint64_t> (k + 1, 0)));

    std::cout << count_paths(count, maze);
    return 0;
}
