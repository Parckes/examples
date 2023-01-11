#include <iostream>
#include <vector>
#include <string>


void fill_palindromes_matrix(std::vector<std::vector<int>>& pals_found, std::string& input_string) {
    size_t length = pals_found.size();
    for (int i = 0; i < length; ++i) {
        pals_found[i][i] = 1;
    }
    for (int i = 2; i <= length; ++i) {
        for (int left = 0; left <= length - i; ++left) {

            int right = left + i - 1;
            if (input_string[right] == input_string[left]) {
                pals_found[left][right] = 1 + pals_found[left][right - 1] +
                                              pals_found[left + 1][right];
            }
            else {
                pals_found[left][right] = pals_found[left][right - 1] +
                                          pals_found[left + 1][right] -
                                          pals_found[left + 1][right - 1];
            }
        }
    }
}

int main() {
    std::string input_string;
    std::cin >> input_string;
    size_t length = input_string.length();

    //pals_found[i][j] -- amount of palindromes, found in input_string[i:j]
    std::vector<std::vector<int>> pals_found(length, std::vector<int> (length, 0));


    fill_palindromes_matrix(pals_found, input_string);

    std::cout << pals_found[0][length - 1] << "\n";
    return 0;
}
