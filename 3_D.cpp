/* D. Сломанный мост (4 балла)
Мост представляет собой поле из n × m клеток, причем некоторые клетки ещё остались целыми.
 У Шрека есть только дощечки размера 1 × 2, установка которых занимает у него a секунд, и 1 × 1,
 установка которых занимает b секунд. Ему необходимо закрыть все пустые клетки, причем только их,
 не накладывая дощечки друг на друга.
 Определите, через какое минимальное количество секунд Шрек и Осёл смогут продолжить свой путь дальше.*/
#include <iostream>
#include <vector>

class Graph {
private:
    std::vector<std::vector<bool>> matrix;
    std::vector<int> nums;
public:
    void add_line(int num, std::vector<bool> &line) {
        matrix.push_back(line);
        nums.push_back(num);
    }

    //нахождение минимальной стоимости заделки моста
    int find_min_cost(int a, int b){
        int res = 0;

        std::vector<int> neib;
        // в таком случае выгоднее замостить все дырки единичными клетками
        if(a >= 2 * b){
            for(int i = 0; i < matrix.size(); ++i)
                res += nums[i];
            res *= b;
            return res;
        } else {
            int n = matrix.size();
            int m = matrix[0].size();
            //иначе построим раскрасим мост в шахматную раскраску и составим двудольный граф
            // из черный клеток и белых клеток
            //в этом случае нам выгоднее всего максимизировать количество двушек
            //ребра веса 1 будут между соседними клетками
            //достаточно просто пронумеровать все клетки по порядку
            //0123
            //4567..
            std::vector<int> left_part, right_part;
            //на i позиции стоит список ребер из i клетки в праваую долю
            std::vector<std::vector<int>> edges(n * m);

            for(int i = 0; i < n * m; ++i) {
                //если клетка испорчена
                if (!matrix[i / m][i % m]) {
                    //проверим теперь соседние вершины на сломанность
                    neib.clear();
                    //если они есть
                    if (check_neib(neib, i)) {
                        edges[i].insert(edges[i].end(), neib.begin(), neib.end());
                        if ((i / m + i % m) % 2 == 0)
                            left_part.push_back(i);
                        else
                            right_part.push_back(i);
                    }
                    else
                        res += b;//если она единственная в округе, сразу чиним ее за b секунд
                }
            }
            //имеем две доли и ребра между ними, далее - алгоритм куна

            //запускаем алгоритм куна с обходом в глубину
            // вектор для паросочетаний
            //на i месте вершина из левой доли, которая соединена ребром с i правой доли
            if(left_part.size() != 0 && right_part.size() != 0) {
                std::vector<int> pairs(m * n, -1);

                Kuna(edges, pairs);

                //число использованных двушек b однерок
                int used_a = 0, used_b = 0;
                for(int i = 0; i < right_part.size(); ++i) {
                    if (pairs[right_part[i]] != -1)
                        ++used_a;
                }
                res += (a * used_a + b * (right_part.size() + left_part.size() - 2 * used_a));
            }

            return res;
        }
    }

    void Kuna(std::vector<std::vector<int>> &edges, std::vector<int> &pairs){
        for(int i = 0; i < edges.size(); ++i) {
            if(edges[i].size() != 0) {
                std::vector<bool> used(edges.size(), false);
                dfs_kuna(i, used, edges, pairs);
            }
        }
    }

    // сам обход в глубину
    bool dfs_kuna(int from, std::vector<bool> &used, std::vector<std::vector<int>> &edges, std::vector<int> &pairs){
        // если уже были
        if(used[from])
            return false;

        used[from] = true;

        for (auto to : edges[from]) {
            if (pairs[to] == -1 || dfs_kuna(pairs[to], used, edges, pairs)) {
                pairs[to] = from;
                return true;
            }
        }
        return false;
    }

    bool check_neib(std::vector<int> &res, int cur){
        int n = matrix.size(), m = matrix[0].size();
        int x = cur % m, y = cur / m;

        if (x != 0){
            if(!matrix[y][x - 1])
                res.push_back(cur - 1);
        }

        if (y != 0){
            if(!matrix[y - 1][x])
                res.push_back(cur - m);
        }

        if (x != m - 1){
            if(!matrix[y][x + 1])
                res.push_back(cur + 1);
        }

        if (y != n - 1){
            if(!matrix[y + 1][x])
                res.push_back(cur + m);
        }

        return res.size() != 0;
    }
};

int main() {
    int n, m, a, b;
    std::cin >> n >> m >> a >> b;
    std::vector<bool> line;

    Graph G;

    char one;
    int num;

    for(int i = 0; i < n; ++i) {
        num = 0;
        for (int j = 0; j < m; ++j) {
            std::cin >> one;
            if (one == '*')
                ++num;
            line.push_back(one == '.');
        }
        G.add_line(num, line);
        line.clear();
    }

    std::cout << G.find_min_cost(a, b);

    return 0;
}
/*
2 2 2 2
.*
*.
 */

/*
2 4 3 2
.**.
.*.*
 */

/*
2 4 1 1
.**.
.***
 */

/*
 2 3 3 2
.**
.*.
 */

/*
2 4 1 1
.*.*
*.*.
 */

/*
2 4 3 2
.**.
*..*
 */

/*
2 4 3 2
..*.
.*.*
 */
