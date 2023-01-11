#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>


std::vector<std::string> next_positions(std::string& str) {
    int sp = str.find(' ');
    std::vector<std::string> moves;
    std::string t;
    if (sp - 3 >= 0) {
        t = str;
        std::swap(t[sp], t[sp - 3]);
        moves.push_back(t);
    }
    if (sp - 1 >= 0 && sp % 3) {
        t = str;
        std::swap(t[sp], t[sp - 1]);
        moves.push_back(t);
    }
    if (sp + 3 < str.length()) {
        t = str;
        std::swap(t[sp], t[sp + 3]);
        moves.push_back(t);
    }
    if (sp + 1 < str.length() && (sp + 1) % 3) {
        t = str;
        std::swap(t[sp], t[sp + 1]);
        moves.push_back(t);
    }
    return moves;
}

int find_ans(std::string& start) {
    std::string end = "12345678 ";
    std::unordered_map<std::string, int> seen;
    seen[start] = 0;

    std::queue<std::string> q;
    q.push(start);
    int i = 0;
    while (q.size()) {
        std::string elem = q.front();
        q.pop();
        std::vector<std::string> moves = next_positions(elem);
        for (auto& x : moves) {
            if (seen.find(x) == seen.end()) {
                seen[x] = seen[elem] + 1;
                q.push(x);
            }
            if (x == end)  {
              return seen[x];
            }
        }
        //std::cout << elem <<"\n";
        ++i;
    }

    return -1;
}

int main() {
    std::string start;
    for (int i = 0; i < 3; ++i) {
        std::string s_temp;
        getline(std::cin, s_temp);
        start += s_temp;
    }
    std::cout << find_ans(start);

    return 0;
}
