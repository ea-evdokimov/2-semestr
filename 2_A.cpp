/*A. Who let the Rick out?
 * Выведите одно число — количество бутылок лимонада,
 * которые отдаст Рик за такое путешествие.*/
#include <iostream>
#include <vector>
#include <set>

class Graph{
private:
    std::vector<std::vector<std::pair<int, int>>> graph;
    //храним массив пар, на первом месте вес ребра, на втором - вершина, куда ведет ребро
public:
    Graph(int n){
        graph.resize(n);
    }

    ~Graph() = default;

    void add_edges(int a, int b) {
        //first is edge to (i+1)%M with weight a, second is edge to (i*i+1)%M with weight b
        int min = a <= b ? a : b;
        for(size_t i = 0; i < graph.size(); ++i) {
            //если 2 ребра имеет одинаковые начало и конец, выбираем с меньшей длиной
            if ((i + 1) % graph.size() == (i * i + 1) % graph.size())
                graph[i].push_back(std::make_pair(min, (i + 1) % graph.size()));
            else {
                graph[i].push_back(std::make_pair(a, (i + 1) % graph.size()));
                graph[i].push_back(std::make_pair(b, (i * i + 1) % graph.size()));
            }
        }
    }

    int Dejkstra(int from, int to) {
        int big = 1e8;//тк максимум 1e6 путей весом 100
        std::vector<int> distance(graph.size(), big);//массив кратчаших путей от вершины from

        //начинаем из вершины, пришедшей на вход
        distance[from] = 0;

        std::set<std::pair<int, int>> s;
        //в set храним пары из расстоянияи и вершины
        //будет сортировать по расстоянию
        s.insert(std::make_pair(distance[from],  from));

        //s.begin() - указывает на первый элемент
        while(!s.empty()){
            int cur = s.begin()->second;
            s.erase(s.begin());
            //выбераем ребро с наименьшей длиной
            for(int i = 0; i < graph[cur].size(); ++i){
                int next = graph[cur][i].second;
                int dist_to_next = graph[cur][i].first;

                if (distance[cur] + dist_to_next < distance[next]){
                    //если можем улучшить расстояние, заменяем эту пару, кладем обратно в set
                    s.erase(std::make_pair(distance[next], next));
                    distance[next] = distance[cur] + dist_to_next;
                    s.insert(std::make_pair(distance[next], next));
                }
            }
        }
        return distance[to];
    }
};

int main() {
    int a, b, M, x, y;
    std::cin >> a >> b >> M >> x >> y;

    //creating solver tree
    Graph G(M);

    G.add_edges(a, b);

    std::cout <<  G.Dejkstra(x, y);

    return 0;
}