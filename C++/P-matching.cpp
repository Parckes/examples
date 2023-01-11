#include <iostream>
#include <vector>
#include <string>
#include <climits>

int is_match(std::string text, std::string pattern) {
    int i = 0, j = 0;
    int star_t = -1, star_p = -1;
    while (i < text.length()) {
        if (j < pattern.length() && (text[i] == pattern[j] || pattern[j] == '?')) {
            ++i;
            ++j;
        }
        else if (j < pattern.length() && pattern[j] == '*') {
            star_t = star_p = j;
            ++j;
        }
        else if (star_p != -1) {
            j = star_p + 1;
            i = star_t + 1;
            ++star_t;
        }
        else {
          return 0;
        }
    }
    while (j < pattern.length() && pattern[j] == '*') {
        ++j;
    }
    if (j == pattern.length()) return 1;
    return 0;
}

int main() {
    std::string text, pattern;
    std::cin >> text >> pattern;
    std::cout << (is_match(text, pattern) ? "YES\n" : "NO\n");
}
