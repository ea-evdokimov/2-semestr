#include <iostream>
#include <vector>
#include <stack>

using namespace std;

class Graph {
private:
    vector<vector<int>> neib;
    vector<int> colors;
    stack<int> q;
    bool flag;

public:
    Graph(int n){
        neib.resize(n);
        colors.resize(n, 0);
        flag = false;
    }

    ~Graph() = default;

    void add(int from, int to) {
        neib[from].push_back(to);
    }

    void prepare(){
        for(int i = 0; i < colors.size(); ++i)
            colors[i] = 0;
        while(!q.empty())
            q.pop();
    }

    void function(){
        for(int i = 0; i < colors.size(); ++i)
            if(colors[i] == 0)
                DFS(i);
        if(flag)
            cout << "NO";
        else {
            cout << "YES" << endl;
            print_q();
        }
    }

    void DFS(int s) {
        colors[s] = 1;

        for (auto u : neib[s]) {
            if(colors[u] == 1)
                flag = true;
            if (colors[u] == 0)
                DFS(u);
        }

        colors[s] = 2;
        q.push(s);
    }

    void print(){
        for(int i = 0; i < colors.size(); ++i) {
            cout << i << ":";
            for (auto u : neib[i])
                cout << u << " ";
            cout << endl;
        }
    }

    void print_q(){
        while(!q.empty()) {
            cout << q.top() << " ";
            q.pop();
        }
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

    G.function();
}
