/*B. Rick for the Weekend
 Теперь Рик хочет найти путь
 (наименьший по стоимости в бутылках лимонада)
 Выведите одно число - минимальную стоимость пути, подходящего для профессора.
 Если профессор не сможет за K ночей добраться до конференции, выведите число -1.
 */
#include <iostream>
#include <vector>
#include <set>

//заведем струтуру для хранения ребер
struct Edge{
    int start;
    int finish;
    int weight;

    Edge(int s, int f, int w) : start(s), finish(f), weight(w) {}
};

class Graph{
private:
    std::vector<Edge> edges;
public:

    void add_edges(int a, int b, int w){
        edges.push_back(Edge(a, b, w));
    }

    int FordBellman(int N, int K, int from, int to){
        int big = 1e9;
        std::vector<int> distance(edges.size(), big);
        std::set<int> relaxed_verts;
        //начальная вершина
        distance[from] = 0;
        relaxed_verts.insert(from);

        std::vector<int> relaxed_help;
        //если за одну итерацию улучшения расстояния не произошло, выходим из цикла

        for(int i = 0; i < K; ++i){
            //важно то, что мы будем релаксировать не все ребра: на первой итерации только те, которые касаются стартовой вершины,
            // затем те, которые касаются прорелаксироавнных ребер и тд
            for (int j = 0; j < edges.size(); ++j){
                if(relaxed_verts.find(edges[j].start) != relaxed_verts.end()) {
                    //пытаемся произвести релаксацию
                    if (distance[edges[j].finish] > distance[edges[j].start] + edges[j].weight) {
                        distance[edges[j].finish] = distance[edges[j].start] + edges[j].weight;
                        relaxed_help.push_back(edges[j].finish);
                    }
                }
            }
            for(auto i : relaxed_help)
                relaxed_verts.insert(i);
            relaxed_help.clear();
        }

        if (distance[to] == big)
            return -1;
        else
            return distance[to];
    }
};

int main() {
    int N, M, K, s, f;
    int a, b, w;
    std::cin >> N >> M >> K >> s >> f;

    Graph G;
    for(int i = 0; i < M; ++i){
        std::cin >> a >> b >> w;
        G.add_edges(a - 1, b - 1, w);
    }

    std::cout << G.FordBellman( N, K, s - 1, f - 1);
    return 0;
}
/*
7 8 2 1 5
1 2 1
2 3 1
3 4 1
4 5 1
1 5 8
1 6 2
6 7 2
7 5 2
 */