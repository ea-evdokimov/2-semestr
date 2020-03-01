#include <iostream>
#include <vector>
#include <set>

using namespace std;

class Graph {
private:
    vector<set<int>> neib;
    vector<int> colors;
    vector<int> l;
    vector<int> d;
    vector<int> res;
    int time;
public:
    Graph(int n){
        neib.resize(n);
        colors.resize(n, 0);
        l.resize(n, 0);
        d.resize(n, 0);
        res.resize(n, 0);
        time = 0;
    }

    ~Graph() = default;

    void add(int from, int to) {
        neib[from].insert(to);
        neib[to].insert(from);
    }

    void DFS_spec(int s, int par = -1) {
        time++;
        l[s] = time;
        d[s] = time;
        colors[s] = 1;
        //int child = 0;

        for (auto v : neib[s]) {
            if (v != par){
                if(colors[v] == 0){
                    DFS_spec(v, s);
                    //child++;
                    if (l[s] > l[v])
                        l[s] = l[v];
                    //if(par != -1 && l[v] >= d[s])
                        //res[s] = 1;
                    if(l[v] > d[s])
                        cout << v << " " << s << endl;
                }
                else {
                    if (l[s] > d[v])
                        l[s] = d[v];
                }
            }
        }

        //if (par < 0 && child > 1)
            //res[s] = 1;
        //colors[s] = 2;
        //time++;

        /*if(s != par) {
            for (auto v : neib[s]) {
                if (colors[v] == 0) {
                    DFS(v, s);
                    if (l[s] > l[v])
                        l[s] = l[v];
                }
            }
            colors[s] = 2;
        }*/
    }

    void print(){
        for(int i = 0; i < colors.size(); ++i) {
            cout << i << ":";
            for (auto u : neib[i])
                cout << u << " ";
            cout << endl;
        }
    }

    void result(){
        for(int i = 0; i < colors.size(); ++i)
            if(res[i])
                cout << i << " ";
    }
};

int main() {
    int n, m;
    cin >> n >> m;

    Graph G(n);

    int from, to;

    for(int i = 0; i < m; ++i) {
        cin >> from >> to;
        G.add(from, to);
    }

    G.DFS_spec(0);
    //G.result();
}

/*
5 6
0 3
3 4
4 0
0 2
2 1
1 0
  */
/*
9 11
0 1
1 2
2 0
2 3
3 4
4 6
6 5
5 3
2 7
7 8
1 7
 */