#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>



void print_ans(std::vector<std::vector<int>>& arr, int length) {
    std::vector<int> answer;
    int elem = arr[1][length];
    while (elem != -1) {
        answer.push_back(elem);
        elem = arr[2][elem];
    }
    std::cout << answer.size() << "\n";
    for (int i = 0; i < answer.size(); ++i) {
        std::cout << answer[answer.size() - 1 - i] << " ";
    }
}

//upper bound
int bin_search(int x, std::vector<int>& arr) {
    int left = 0;
    int right = arr.size();
    while (right - left > 1) {
        int middle = (left + right) / 2;
        if (arr[middle] >= x) {
            right = middle;
        }
        else {
            left = middle;
        }
    }
    return right;
}


void find_las(std::vector<int>& seq) {
    // arr[0][i] - lowest element from seq which can be the end of AS of length i
    // arr[1][i] - pos of arr[0][i]
    // arr[2][i] - elem the goes before arr[0][i]
    std::vector<std::vector<int>> arr(3, std::vector<int> (seq.size() + 1, INT_MAX));
    arr[0][0] = INT_MIN;
    arr[1][0] = -1;
    int length = 0;
    for (int i = 0; i < seq.size(); ++i) {
        int j = bin_search(seq[i], arr[0]);
        //right = std::upper_bound(arr[0].begin(), arr[0].end(), seq[i]) - arr[0].begin();
        if (arr[0][j - 1] < seq[i] && seq[i] < arr[0][j]) {
            arr[0][j] = seq[i];
            arr[1][j] = i;
            arr[2][i] = arr[1][j - 1];
            length = std::max(length, j);
        }
    }
    print_ans(arr, length);
}

int main() {
    int n;
    std::cin >> n;
    std::vector<int> seq(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> seq[i];
    }
    find_las(seq);
    return 0;
}
