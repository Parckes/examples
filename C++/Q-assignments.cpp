/*Дана целочисленная матрица C размера N × N. Требуется выбрать N ячеек так,
 чтобы в каждой строке и каждом столбце была выбрана ровно одна ячейка и
 сумма значений в выбранных ячейках было минимальна.


 An integer matrix C of size N × N is given. You need to select N cells like this,
so that exactly one cell is selected in each row and each column and
the sum of the values in the selected cells is minimal.
 */

#include <iostream>
#include <vector>
#include <climits>

void vengr(std::vector<std::vector<int>> &matr, int &n);

int main(){
    int N;
    std::cin >> N;
    std::vector<std::vector<int>> matr(N, std::vector<int>(N));
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            std::cin>>matr[i][j];
    vengr(matr, N);
    return 0;
}

void vengr(std::vector<std::vector<int>> &matr, int &n){
    std::vector<int> u(n, 0), v(n, 0), mark(n, -1);

    for(int i = 0; i < n; i++){
        std::vector<int> link(n, -1), mins(n, INT_MAX), used(n, 0);
        int idI = i, idJ = -1, j;
        while (idI != -1){
            j = -1;
            for(int k = 0; k < n; k++)
                if(!used[k]){
                    if(matr[idI][k] - u[idI] - v[k] < mins[k]){
                        mins[k] = matr[idI][k] - u[idI] - v[k];
                        link[k] = idJ;
                    }
                    if(j == -1 || mins[k] < mins[j])
                        j = k;
                }
            int delta = mins[j];
            for(int k = 0; k < n; k++)
                if(used[k]){
                    u[mark[k]] += delta;
                    v[k] -= delta;
                }
                else
                    mins[k] -= delta;
            u[i] += delta;

            used[j] = 1;
            idJ = j;
            idI = mark[j];
        }
        for(; link[j] != -1; j = link[j])
            mark[j] = mark[link[j]];
        mark[j] = i;
    }

    std::vector<std::pair<int, int>> ans;
    int sum = 0;
    for(int i = 0; i < n; i++)
        if(mark[i] != -1){
            ans.push_back({mark[i] + 1, i + 1});
            sum += matr[mark[i]][i];
        }
    std::cout << sum << std::endl;
    for(int i = 0; i < ans.size(); i++)
        std::cout << ans[i].first << " " << ans[i].second << std::endl;

}
