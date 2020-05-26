/*A. MST-1 (2 балла)
 Требуется найти в связном графе остовное дерево минимального веса.
 Воспользуйтесь алгоритмом Прима.*/
#include <iostream>
#include <vector>
#include <set>

class Graph{
private:
    std::vector<std::vector<std::pair<int, int>>> graph;
    //храним set пар, на первом месте вес ребра, на втором - вершина, куда ведет ребро
    //ребра остортированы по весам
public:
    Graph(int n){
        graph.resize(n);
    }

    ~Graph() = default;

    void add_edges(int b, int e, int w) {
        //граф неоориентированный
        graph[b].push_back(std::make_pair(w, e));
        graph[e].push_back(std::make_pair(w, b));
    }

    int Prim(){
        std::vector<bool> used(graph.size(), false);
        std::set<std::pair<int, int>> s;
        //выбираем проивзольное начало остовного дерева
        int res = 0;
        s.insert(std::make_pair(0, 0));
        int num = 0;

        while(num < graph.size()){
            //вытаскиваем наименьшее по весу ребро
            int cur = s.begin()->second;
            int weight_to_cur = s.begin()->first;
            s.erase(s.begin());

            //убеждаемся, что оно ведет не в остов
            if(!used[cur]) {
                //присодиняем к остову
                used[cur] = true;
                res += weight_to_cur;
                ++num;
                //добавляем ребра из новой вершины, set сортирует по возрастанию веса ребра
                for (auto next : graph[cur])
                    if (!used[next.second])
                        s.insert(next);
            }
        }
        return res;
    }
};

int main() {
    int n, m; std::cin >> n >> m;
    int b, e, w;

    Graph G(n);

    for(int i = 0; i < m; ++i){
        std::cin >> b >> e >> w;
        G.add_edges(b - 1, e - 1, w);
    }

    std::cout << G.Prim();
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