/* E. Глупая ссора (4 балла)
 Шрек и Осёл уже были на пути домой. Им оставалось только преодолеть лес, который отделял их от болота.
 Но они поссорились, поэтому не хотят идти вместе.
 Сейчас друзья находятся на опушке с номером s, а болото Шрека — на опушке с номером t.
 Помогите Шреку и Ослу добраться до болота.*/
#include <iostream>
#include <vector>

//ребро
struct Edge{
    int from, to;
    //пропусная способность и текущий поток
    int c, cur_flow;

    Edge(int x, int y, int c) : from(x), to(y), c(c), cur_flow(0) {}

    void increase_c(int delta){
        c += delta;
    }

    void decrease_flow(int delta){
        cur_flow -= delta;
    }

    void increase_flow(int delta){
        cur_flow += cur_flow + delta;
    }
};

class Graph{
private:
    //массив ребер
    std::vector<std::vector<Edge>> graph;
    // для записи пути
    std::vector<int> path1;
public:
    Graph(int n){
        graph.resize(n);
    }

    void add_edges(int from, int to){
        //петли не нужны
        //если рубро уже есть, можем просто увеличить его пропускную способность
        if(from != to){
            bool flag = false;

            for(int i = 0; i < graph[from].size(); ++i) {
                if (graph[from][i].to == to) {
                    graph[from][i].increase_c(1);
                    flag = true;
                    break;
                }
            }
            // если не нашли, добавляем ребро
            if (!flag)
                graph[from].push_back(Edge(from, to, 1));

            flag = false;
            //добавляем обратное ребро
            for(auto i : graph[to]) {
                if (i.to == from) {
                    flag = true;
                    break;
                }
            }
            //если не нашли
            if (!flag)
                graph[to].push_back(Edge(to, from, 0));
        }
    }

    //сам алгоритм фф
    bool max_flow(int s, int t){
        int max_flow = 0;
        int n = graph.size();
        int run = 1;

        while(run && max_flow < 2){
            std::vector<bool> used(n, 0);
            run = find_flow(s, t, 2, used);
            max_flow += run;
        }

        return max_flow >= 2;
    }

    int find_flow(int from, int t, int flow, std::vector<bool> &used) {
        //на это раз используем dfs для нахождения пути, по которому мы можем увеличить поток
        //если дошли до target
        if (from == t) {
            return flow;
        }

        //dfs
        used[from] = true;
        int capacity = 0;

        for(auto &i : graph[from]){
            capacity = i.c;
            if(!used[i.to]) {
                //если можем увеличить поток
                if (capacity > i.cur_flow){
                    int last_cap = capacity - i.cur_flow;
                    // поток ограничивается сверху остаточной пропускной сопсобностью ребра
                    flow = last_cap < flow ? last_cap : flow;

                    int final_flow = find_flow(i.to, t, flow, used);
                    //если смогли дойти до target, с положительным потоком
                    //персчитываем текущие потоки
                    if(final_flow) {
                        i.increase_flow(final_flow);
                        decr_reversed(from, i.to, final_flow);
                        return final_flow;
                    }
                }
            }
        }
        return 0;
    }

    void decr_reversed(int from, int to, int flow){
        for(auto &i : graph[to]){
            if(i.to == from) {
                i.decrease_flow(flow);
                break;
            }
        }
    }

    void print_path(int s, int t){
        //осталось найти 2 этих пути
        //смотрим на остаточную сеть, там, где поток больше нуля, есть дорога

        std::vector<bool> used(graph.size(), 0);
        dfs(s, t, used);
        for(int i = 0; i < used.size(); ++i){
            used[i] = false;
        }
        //второй раз
        dfs(s, t, used);
        for(int i = path1.size() - 1; i >= 0; --i){
            std::cout << path1[i] + 1 << " ";
            if(path1[i] == t)
                std::cout << std::endl;
        }
    }

    bool dfs(int from, int t, std::vector<bool> &used){
        if(from == t) {
            path1.push_back(from);
            return 1;
        }

        if(used[from])
            return 0;
        used[from] = true;

        for(auto &i : graph[from]){
            if(!used[i.to] && i.cur_flow > 0){
                if(dfs(i.to, t, used)) {
                    i.decrease_flow(1);
                    path1.push_back(from);
                    return 1;
                }
            }
        }
        return false;
    }
};

int main() {
    int n, m, s, t, from, to;
    std::cin >> n >> m >> s >> t;

    Graph G(n);

    // считывание ребер
    for(int i = 0; i < m; ++i){
        std::cin >> from >> to;
        G.add_edges(from - 1, to - 1);
    }

    if(!G.max_flow(s - 1, t - 1))
        std::cout << "NO" << std::endl;
    else{
        std::cout << "YES" << std::endl;
        G.print_path(s - 1, t - 1);
    }

    return 0;
}


/*
3 3 1 3
 1 3
 1 2
 2 3
 */

/*
4 6 1 4
 1 2
 2 3
 3 4
 1 3
 3 2
 2 4
 */

/*
 4 5 1 4
 1 2
 1 3
 3 2
 2 4
 2 4
 */

/*
 4 5 1 4
 1 2
 2 4
 3 2
 1 3
 3 4
 */

/*
 5 8 1 5
 1 3
 3 2
 2 3
 1 3
 3 4
 4 5
 2 4
 4 5
 */

/*
 8 9 1 8
 1 2
 2 3
 3 6
 6 8
 1 4
 4 5
 5 7
 7 8
 4 6
 */
