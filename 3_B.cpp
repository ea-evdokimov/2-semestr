/*B. MST-2 (3 балла)
Требуется найти в связном графе остовное дерево минимального веса.
Воспользуйтесь алгоритмом Крускала.*/
#include <iostream>
#include <vector>
#include <algorithm>

class Graph{
private:
    size_t num_verts;
    std::vector<std::pair<int, std::pair<int, int>>> graph;
    //массив пар, на первом месте - вес ребра, на втором - папа вершин, которое оно соединяет
public:
    Graph(size_t n) : num_verts(n) {}

    ~Graph() = default;

    void add_edges(int b, int e, int w) {
        graph.push_back({w, {b, e}});
    }

    size_t Kruskal(){
        size_t res = 0;
        //сортируем массив в порядке увеличения веса ребра
        sort(graph.begin(), graph.end());
        //изначачльно n отдельных поддеревьев

        //у каждой вершины свой номер дерева(которому принадлежит)
        std::vector<int> tree_num(num_verts);

        for(int i = 0; i < num_verts; ++i)
            tree_num[i] = i;

        //бежим по ребрам
        for(auto cur : graph){
            int cur_weight = cur.first;
            int from = cur.second.first;
            int to = cur.second.second;
            //если из разных деревьев
            if(tree_num[from] != tree_num[to]) {
                res += cur_weight;
                int old_num = tree_num[to];
                int new_num = tree_num[from];
                //объединяем все веришны принадлежащие дереву to перетаскиваем в from и меняем их индексы деревьев
                for(int vert = 0; vert < tree_num.size(); ++vert) {
                    if (tree_num[vert] == old_num)
                        tree_num[vert] = new_num;
                }
            }
        }
        return res;
    }
};

int main() {
    size_t n, m; std::cin >> n >> m;
    int b, e, w;

    Graph G(n);

    for(int i = 0; i < m; ++i){
        std::cin >> b >> e >> w;
        G.add_edges(b - 1, e - 1, w);
    }

    std::cout << G.Kruskal();
    return 0;
}

/*
 5 6
1 2 8
1 3 3
2 3 1
1 4 5
2 5 2
4 5 2
 */

/*
3 3
1 2 1
2 3 2
3 1 3
 */
/*
 4 4
1 2 1
2 3 2
3 4 5
4 1 4
 */