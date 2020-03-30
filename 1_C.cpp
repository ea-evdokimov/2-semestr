/*C. Районы, кварталы, жилые массивы
 Выведите одно число — количество дорог, которые нужно добавить,
 чтобы весь город стал одним кварталом.*/
#include <iostream>
#include <vector>
#include <set>
#include <stack>

using namespace std;

class Graph {
private:
    vector< vector<int> > graph, invert_graph;
public:
    Graph(int n){
        graph.resize(n);
        invert_graph.resize(n);
    }

    ~Graph() = default;

    void add(int from, int to) {
        graph[from].push_back(to);
        invert_graph[to].push_back(from);
    }

    void dfs(int from, vector<int>& colors, vector<int>& out) {
        stack<int> s;
        s.push(from);

        while(!s.empty()) {
            int cur = s.top();

            if (colors[cur] == 0) {
                colors[cur] = 1;
                for (auto u : graph[cur])
                    if (colors[u] == 0)
                        s.push(u);
            }

            else if (colors[cur] == 1) {
                colors[cur] = 2;
                out.push_back(cur);
            } else {
                s.pop();
            }
        }
    }

    void dfs_invert(int from, vector<int>& colors, vector<int>& component){
        stack<int> s;
        s.push(from);

        while(!s.empty()) {
            int cur = s.top();

            if (colors[cur] == 0) {
                colors[cur] = 1;
                for (auto u : invert_graph[cur])
                    if (colors[u] == 0)
                        s.push(u);
            }
            else if (colors[cur] == 1) {
                colors[cur] = 2;
                component.push_back(cur);
                s.pop();
            } else {
                s.pop();
            }
        }
    }

    int function() {
        vector<int> colors(graph.size(), 0);
        vector<int> out, component;
        vector<int> csc_colors(graph.size());

        //разбиение на компоненты сильной связности по алгоритму Косарайю
        for (int i = 0; i < graph.size(); ++i) {
            if (!colors[i])
                dfs(i, colors, out);
        }

        for (int i = 0; i < colors.size(); ++i)
            colors[i] = 0;

        //csc - компонента сильной связности
        //пронумеруем их, чтобы различать
        int num_of_csc = 0;

        for (int i = 0; i < invert_graph.size(); ++i) {
            int v = out[invert_graph.size() - i - 1];

            if (!colors[v]) {
                dfs_invert(v, colors, component);

                for (int j = 0; j < component.size(); ++j)
                    csc_colors[component[j]] = num_of_csc;

                num_of_csc++;
                component.clear();
            }
        }

        return CSC(num_of_csc, csc_colors);
    }

    int CSC(int num_of_csc, vector<int> & csc_colors){
        vector<set<int>> CSC_graph(num_of_csc);//граф из компонент связности

        //ищем связи между компонентами
        for(int i = 0; i < graph.size(); ++i)
            for(auto to : graph[i])
                if(csc_colors[i] != csc_colors[to])//если лежат в разных компонениах
                    CSC_graph[csc_colors[i]].insert(csc_colors[to]);//можем попасть

        vector<bool> in(num_of_csc, 0);//массив тех компонент, которые доступны из других компонент
        //считаем те, в которые либо приходят 0 стрелок, либо из которых исходят
        int in_zero = 0, out_zero = 0;

        for(int i = 0; i < num_of_csc; ++i)
            for (auto s : CSC_graph[i])
                in[s] = 1;//компоненты, в которые ведут ребра

        for(int i = 0; i < num_of_csc; ++i)
            if (in[i] == 0)
                in_zero++;

        for(int i = 0; i < num_of_csc; ++i)
            if(CSC_graph[i].size() == 0)
                out_zero++;//компоненты, из которых не ведут ребра

        if(CSC_graph.size() == 1)//если это одна компонента
            return 0;
        else//иначе максимум из значений
            return (in_zero > out_zero) ? in_zero : out_zero;
    }
};

int main() {
    int num_vert, num_edge;
    cin >> num_vert >> num_edge;

    Graph G(num_vert);

    int from, to;

    for(int i = 0; i < num_edge; ++i) {
        cin >> from >> to;
        G.add(from - 1, to - 1);
    }

    cout << G.function();
    return 0;
}

