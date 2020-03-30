/*B. Секретные материалы
Если такого порядка не существует, то выведите «NO» (без кавычек).
В противном случае, выведите «YES» в первую из строк. Во вторую строку выведите перестановку индексов вершин*/
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

class Graph {
private:
    vector<vector<int>> graph;
public:
    Graph(int n){
        graph.resize(n);
    }

    ~Graph() = default;

    void add(int from, int to) {
        graph[from].push_back(to);
    }

    struct Help{
        vector<int> colors;
        stack<int> res;
        bool flag;

        Help(int n){
            colors.resize(n, 0);
            flag = 0;
        }
    };


    void function(){
        Help H(graph.size());
        //сортируем все достижимые вершины из данной, для прохода по порядку
        for(int i = 0; i < graph.size(); ++i)
            sort(graph[i].begin(), graph[i].end());

        for(int i = 0; i < H.colors.size(); ++i)
            if(!H.flag && H.colors[i] == 0)
                DFS(i, H);

        print_res(H);
    }

    void DFS(int from, Help & H) {
        stack<int> s;
        s.push(from);

        while(!s.empty()){
            int cur = s.top();

            if (H.colors[cur] == 0) {
                H.colors[cur] = 1;//обрабатываем вершину
                for (int i = graph[cur].size()- 1; i >= 0; --i) {
                    int to = graph[cur][i];

                    if (H.colors[to] == 1) {//если нашли цикл
                        H.flag = true;
                        break;
                    }
                    if (H.colors[to] == 0) {//непосещенная
                        s.push(to);
                    }
                }

            } else if (H.colors[cur] == 1) {
                H.colors[cur] = 2;
                H.res.push(cur);

            } else
                s.pop();
        }
    }

    void print(){
        for(int i = 0; i < graph.size(); ++i) {
            cout << i << ":";
            for (auto u : graph[i])
                cout << u << " ";
            cout << endl;
        }
    }

    void print_res(Help &H){
        if (!H.flag) {
            cout << "YES" << endl;
            while(!H.res.empty()) {
                cout << H.res.top() << " ";
                H.res.pop();
            }
        } else
            cout << "NO";
    }
};

int main() {
    int num_vert, num_edge;
    cin >> num_vert >> num_edge;

    Graph G(num_vert);

    int from, to;

    for(int i = 0; i < num_edge; ++i) {
        cin >> from >> to;
        G.add(from, to);
    }

    G.function();
}
