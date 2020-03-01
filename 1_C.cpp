#include <iostream>
#include <vector>
#include <set>

using namespace std;

class Graph {
private:
    vector< vector<int> > graph, invert_graph;
    vector<int> out, component;// or using stack
    vector<bool> colors;
    vector<int> css_colors;
public:
    Graph(int n){
        graph.resize(n);
        invert_graph.resize(n);
        colors.resize(n);
        css_colors.resize(n);
    }

    ~Graph() = default;

    void add(int from, int to) {
        graph[from].push_back(to);
        invert_graph[to].push_back(from);
    }

    void dfs(int s) {
        colors[s] = 1;

        for (auto u : graph[s])
            if (colors[u] == 0)
                dfs(u);
        out.push_back(s);
    }

    void dfs_invert(int s){
        colors[s] = 1;
        component.push_back(s);

        for (auto u : invert_graph[s])
            if (colors[u] == 0)
                dfs_invert(u);
    }

    void function() {
        for (int i = 0; i < graph.size(); ++i) {
            if (!colors[i])
                dfs(i);
        }

        for (int i = 0; i < colors.size(); ++i)
            colors[i] = 0;


        int num_of_ccs = 0;
        for (int i = 0; i < invert_graph.size(); ++i) {
            int v = out[invert_graph.size() -i - 1];

            if (!colors[v]) {
                dfs_invert(v);

                for (int i = 0; i < component.size(); ++i)
                    css_colors[component[i]] = num_of_ccs;
                    //cout << component[i];
                num_of_ccs++;
                component.clear();
            }
        }
        CCS(num_of_ccs);
    }
    void CCS(int num /*struct*/){
        vector<set<int>> CCS_graph(num);

        for(int i = 0; i < graph.size(); ++i)
            for(auto s : graph[i])
                if(css_colors[i] != css_colors[s])
                    CCS_graph[css_colors[i]].insert(css_colors[s]);

        /*for(int i = 0; i < num; ++i) {
            cout << i << ": ";
            for (auto s : CCS_graph[i])
                cout << s << " ";
            cout << endl;
        }*/
        bool in[num];
        int x = 0, y = 0;
        for(int i = 0; i < num; ++i)
            for (auto s : CCS_graph[i])
                in[s] = 1;
        for(int i = 0; i < num; ++i)
            if(in[i] == 0)
                x++;
        for(int i = 0; i < num; ++i)
            if(CCS_graph[i].size() == 0)
                y++;

        if(CCS_graph.size() == 1)
            cout << "0";
        else
            cout << x ? y : x >= y;
    }
};

int main() {
    int n, m; cin >> n >> m;
    Graph G(n);

    for (int i = 0; i < m; ++i) {
        int n1, n2;
        cin >> n1 >> n2;
        G.add(n1 - 1, n2 - 1);
    }

    G.function();
    return 0;
}

/*9 10
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

/*9 17
1 2
2 1
1 9
9 1
9 8
2 8
8 7
7 8
3 2
3 7
6 7
3 6
6 4
4 3
5 6
5 4
5 5
*/
/*
9 11
1 2
2 3
3 1
3 4
4 6
6 5
5 4
6 7
7 8
8 9
9 7*/

