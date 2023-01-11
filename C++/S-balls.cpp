#include <iostream>
#include <vector>

int find_min_attempts(int n, int k) {
    if (n == 1) return 0;
    if (!k) return -1;
    std::vector<int> attempts(k + 1, 0);
    int res = 0;
    while (attempts[k] < n - 1) {
        for (int i = k; i > 0; --i) {
            attempts[i] += attempts[i - 1] + 1;
        }
        ++res;
    }
    return res;
}

int main() {
    int n, k;
    std::cin >> n >> k;
    std::cout << find_min_attempts(n, k);
    return 0;
}
