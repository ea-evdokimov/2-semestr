/*
 F. Рассадка гостей (3 балла)
 рассадить их всех на две непустые части так, чтобы количество знакомств между двумя частями было минимальным.
 Выведите в выходной файл две строки. На первой выведите номера вершин, попавших в левую часть,
 через пробел, а на второй — номера вершин, попавших в правую часть,
 также через пробел. Номера вершин можно выводить в любом порядке.
 */
#include <iostream>
#include <vector>

class Graph{
private:
    std::vector<std::vector<int>> matrix;
public:
    void create_matrix(std::vector<int> &line){
        matrix.push_back(line);
    }

    void find_min_cut(std::vector<int> &cut) {
        int min_cut_cost = 100;
        int n = matrix.size();

        //iformation about verts that exist separately or merged with other
        std::vector<bool> separate(n, true);

        //list of verts merged in i
        std::vector<std::vector<int>> merged(n);

        for (int i = 0; i < n; ++i)
            merged[i].push_back(i);

        //connectivity - sum of weights with verts in set
        std::vector<int> w(n, 0);

        //if vert included in first part
        std::vector<bool> set1(n, false);

        //n-1 big iterations
        for (int it = 0; it < n - 1; ++it) {
            for (int i = 0; i < n; ++i)
                w[i] = 0;

            for (int i = 0; i < n; ++i)
                set1[i] = false;

            for (int cur = 0, prev; cur < n - it; ++cur) {
                //finding the best vert(next) not in set with the biggest connectivity
                int next = -1;
                for (int i = 0; i < n; ++i)
                    if (separate[i] && !set1[i])
                        if (w[i] > w[next] || next == -1)
                            next = i;


                if (cur != n - it - 1) {
                    //pushing in set
                    set1[next] = true;

                    //update connectivity
                    for (int i = 0; i < n; ++i)
                        w[i] += matrix[next][i];

                    // go to next iteration, prev will be penultimate, finally next will be the last
                    prev = next;
                }
                    //if last iteration
                else {
                    //if this cut is better
                    if (w[next] < min_cut_cost) {
                        min_cut_cost = w[next];
                        cut.clear();
                        cut.insert(cut.end(), merged[next].begin(), merged[next].end());
                    }

                    separate[next] = false;
                    //merging
                    merged[prev].insert(merged[prev].end(), merged[next].begin(), merged[next].end());
                    //updating matrix
                    for (int i = 0; i < n; ++i) {
                        if (i != prev) {
                            matrix[prev][i] += matrix[next][i];
                            matrix[i][prev] += matrix[i][next];
                        }
                    }
                }
            }
        }
    }
};

int main() {
    int n; std::cin >> n;
    Graph G;
    std::vector<int> line;
    char one;

    for(int i = 0; i < n; ++i){
        for(int i = 0; i < n; ++i){
            std::cin >> one;
            line.push_back(one == '1');
        }
        G.create_matrix(line);
        line.clear();
    }


    G.find_min_cut(line);
    std::vector<bool> printed(n);

    for(int i = 0; i < line.size(); ++i){
        std::cout << line[i] + 1 << " ";
        printed[line[i]] = true;
    }

    std::cout << "\n";

    for(int i = 0; i < n; ++i)
        if(!printed[i])
            std::cout << i + 1 << " ";
}
/*
4
0110
1010
1101
0010
 */

/*
6
011000
101000
110100
001011
000101
000110
 */

/*
5
01101
10100
11010
00101
10010
 */
