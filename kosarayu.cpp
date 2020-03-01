#include <iostream>
#include <vector>
#include <set>

using namespace std;

class Graph {
private:
    vector< set<int> > graph, invert_graph;
    vector<int> colors, used, out, component;
public:
    Graph(int n){
        graph.resize(n);
        invert_graph.resize(n);
        colors.resize(n);
        used.resize(n, 0);
    }
    ~Graph() = default;

    void add(int from, int to) {

        if(graph.size() < from || graph.size() < to) {
            if (from >= to) {
                graph.resize(from);
                invert_graph.resize(from);
                colors.resize(from, 0);
                used.resize(from, 0);
            } else {
                graph.resize(to);
                invert_graph.resize(to);
                colors.resize(to, 0);
                used.resize(to, 0);
            }
        }

        graph[from].insert(to);
        invert_graph[to].insert(from);
    }

    void dfs(int s) {
        used[s] = 1;

        for (auto u : graph[s])
            if (used[u] == 0)
                dfs(u);
        out.push_back(s);
    }

    void dfs_invert(int s){
        used[s] = 1;
        component.push_back(s);

        for (auto u : invert_graph[s])
            if (used[u] == 0)
                dfs_invert(u);
    }

    void function() {
        for (int i = 0; i < graph.size(); ++i) {
            if (!used[i])
                dfs(i);
        }
        for (int i = 0; i < used.size(); ++i)
            used[i] = 0;

        for (int i = 0; i < invert_graph.size(); ++i) {
            int v = out[invert_graph.size() -i - 1];

            if (!used[v]) {
                dfs_invert(v);
                for (int i = 0; i < component.size(); ++i)
                    cout << char(component[i] + 'A');
                cout << endl;
                component.clear();
            }
        }
    }
};

int main() {
    int n, m; cin >> n >> m;
    Graph G(n);

    for (int i = 0; i < m; ++i) {
        int n1, n2;
        cin >> n1 >> n2;
        G.add(n1, n2);
    }

    G.function();
    return 0;
}

/* 9 10
0 1
1 2
2 0
3 4
4 5
5 6
6 3
8 7
7 8
8 1
*/

