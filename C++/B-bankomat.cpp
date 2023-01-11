#include <iostream>
#include <vector>
#include <climits>



void fill_variants(std::vector<int>& variants, std::vector<int>& values, int s) {
    variants[0] = 0;
    for (int i = 1; i <= s; ++i) {
        for (int j = 0; j < values.size(); ++j) {
            if (i >= values[j] && variants[i - values[j]] < variants[i])
                variants[i] = variants[i - values[j]] + 1;
        }
    }
}

void print_res(std::vector<int>& variants, std::vector<int>& values, int s) {
    if (variants[s] == INT_MAX) {
        std::cout << "-1\n";
        return;
    }
    std::cout << variants[s] << "\n";
    while (s) {
        for (int i = 0; i < values.size(); ++i) {
            if (s - values[i] >= 0 && variants[s - values[i]] == variants[s] - 1) {
                std::cout << values[i] << " ";
                s -= values[i];
                break;
            }
        }
    }
}


int main() {

    size_t n;
    std::cin >> n;
    std::vector<int> values(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> values[i];
    }

    int s;
    std::cin >> s;
    std::vector<int> variants(s + 1, INT_MAX);
    fill_variants(variants, values, s);

    print_res(variants, values, s);
    std::cout << "\n";
    return 0;
}
