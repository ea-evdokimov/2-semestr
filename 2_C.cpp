/*C. Wake Me Up When Rickember Ends
 * Если Рик знает вероятности быть прижатым к стенке на всех существующих телепортациях,
 * помогите ему посчитать минимальную вероятность, что он всё-таки столкнется с неприятностями.
*/
#include <iostream>
#include <vector>
#include <set>
#include <iomanip>

class Graph{
private:
    std::vector<std::vector<std::pair<double, int>>> graph;
    //храним массив пар, на первом месте вес ребра(в данной задаче вероятность)
    //на втором - вершина, куда ведет ребро
public:
    Graph(int n){
        graph.resize(n);
    }

    ~Graph() = default;

    void add_edges(int s, int f, double p) {
        graph[s].push_back(std::make_pair(p,f));
        graph[f].push_back(std::make_pair(p,s));
    }

    double prob_new(double x, double y){
        return (x + y - x * y);
    }

    double Dejkstra(int from, int to) {
        double big = 1.0;//тк максимум вероятность - 100%
        std::vector<double> probility(graph.size(), big);//массив кратчаших путей от вершины from
        //начинаем из вершины, пришедшей на вход
        probility[from] = 0;

        std::set<std::pair<double, int>> s;
        //в set храним пары из расстоянияи и вершины
        //будет сортировать по вероятности
        s.insert(std::make_pair(probility[from], from));

        //s.begin() - указывает на первый элемент
        while (!s.empty()) {
            int cur = s.begin()->second;
            s.erase(s.begin());

            for (int i = 0; i < graph[cur].size(); ++i) {
                int next = graph[cur][i].second;
                double prob_to_next = graph[cur][i].first;

                if (prob_new(probility[cur], prob_to_next) < probility[next]) {
                    //если можем улучшить расстояние, заменяем эту пару, кладем обратно в set
                    s.erase(std::make_pair(probility[next], next));
                    probility[next] = prob_new(probility[cur], prob_to_next);
                    s.insert(std::make_pair(probility[next], next));
                }
            }
        }
        return probility[to];
    }
};

int main() {
    int N, M, s, f;
    int a, b;
    double p;
    //теперь p - вероятность
    std::cin >> N >> M >> s >> f;

    Graph G(N);
    //считывание ребер графа
    for(int i = 0; i < M; ++i){
        std::cin >> a >> b >> p;
        G.add_edges(a - 1, b - 1, p / 100);
    }

    double res = G.Dejkstra(s - 1, f - 1);
    std::cout << std::setprecision(6) << res;

    return 0;
}