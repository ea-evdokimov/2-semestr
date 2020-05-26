/*
 C. Сделай связным (4 балла)
 Вам дан неориентированный граф, состоящий из n вершин.
 На каждой вершине записано число; число, записанное на вершине i, равно ai.
 Сколько монет минимально вам потребуется, чтобы сделать граф связным? */
#include <iostream>
#include <vector>
#include <set>
#include <map>

class Graph{
private:
    int v_size;
    //веса в вершинах
    std::vector<size_t> w_verts;
    //специальные веса, ключ - пара вершин
    std::map<std::pair<int, int>, size_t> w_special;
    //сами ребра
    std::vector<std::vector<std::pair<size_t, int>>> graph;

public:
    Graph(int n) : v_size(n){
        graph.resize(n);
    }

    ~Graph() = default;

    void add_weights(size_t a) {
        w_verts.push_back(a);
    }

    void set_special(int b, int e, size_t w){
        //учитывается перезапись на лучший результат
        if (w_special.find(right_pair(b, e)) == w_special.end() || w < w_special[right_pair(b, e)])
            w_special[right_pair(b, e)] = w;
    }

    size_t MinCost(){
        size_t res = 0;
        //найдем вершину с минимальным весом, очевидно, если ее соеденить со всеми с остальными, получим оптимальное
        //остовное дерево без учета специальных предложений
        size_t min = 1e12;
        int num_min = 0;
        for(int i = 0; i <v_size; ++i) {
            if (min > w_verts[i]) {
                min = w_verts[i];
                num_min = i;
            }
        }

        //добавляем все ребра с минимальной по весу вершиной в наш graph с учетом спец предложений
        for(int i = 0; i < v_size; ++i){
            if (i != num_min) {
                size_t weight = min + w_verts[i];
                if (w_special.find(right_pair(num_min, i)) != w_special.end()) {
                    weight = weight < w_special[right_pair(num_min, i)] ? weight : w_special[right_pair(num_min, i)];
                    // то есть спец предложение мы прошли, можем про него забыть
                    w_special.erase(right_pair(num_min, i));
                }
                graph[num_min].push_back({weight, i});
                graph[i].push_back({weight, num_min});
            }
        }

        for(auto para : w_special){
            graph[para.first.first].push_back({para.second, para.first.second});
            graph[para.first.second].push_back({para.second, para.first.first});
        }

        //а теперь простой алгоритм Прима
        std::vector<bool> used(v_size, false);
        std::set<std::pair<size_t, int>> s;
        //выбираем проивзольное начало остовного дерева
        s.insert(std::make_pair(0, 0));
        int num = 0;

        while(num < v_size){
            //вытаскиваем наименьшее по весу ребро
            int cur = s.begin()->second;
            size_t weight_to_cur = s.begin()->first;
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

    //ставит большее число на первое место
    std::pair<int, int> right_pair(const int& a, const int& b){
        return a < b ? std::make_pair(b, a) : std::make_pair(a, b);
    }
};

int main() {
    int n, m;
    std::cin >> n >> m;
    int b, e;
    size_t w;

    Graph G(n);

    for (int i = 0; i < n; ++i) {
        std::cin >> w;
        G.add_weights(w);
    }

    for (int i = 0; i < m; ++i) {
        std::cin >> b >> e >> w;
        G.set_special(b - 1, e - 1, w);
    }

    std::cout << G.MinCost() << std::endl;
    return 0;
}

/*
 3 2
1 3 3
2 3 5
2 1 1
 */

/*
 4 0
1 3 3 7
 */

/*
 5 4
1 2 3 4 5
1 2 8
1 3 10
1 4 7
1 5 15
 */
