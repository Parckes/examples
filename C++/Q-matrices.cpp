#include <iostream>
#include <vector>
#include <climits>

int64_t optimal_operations(std::vector<size_t>& sizes, size_t left, size_t right,
                                std::vector<std::vector<int>>& memo) {
    if (left == right)
        return 0;
    if (memo[left][right] != -1) {
        return memo[left][right];
    }

    int res = INT_MAX;
    int cur_res;


    for (int i = left; i < right; i++) {
        cur_res = optimal_operations(sizes, left, i, memo) +
                    optimal_operations(sizes, i + 1, right, memo) +
                      sizes[left - 1] * sizes[i] * sizes[right];
        res = std::min(cur_res, res);
    }

    return memo[left][right] = res;
}

int main() {
    size_t n;
    std::cin >> n;
    std::vector<size_t> sizes(n);
    for (int i = 0; i < n; ++i)
        std::cin >> sizes[i];
    std::vector<std::vector<int>> memo(n, std::vector<int> (n, -1));
    std::cout << optimal_operations(sizes, 1, n - 1, memo);
}
