#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <queue>
#include <stack>
#include <cassert>
#include <cstdio>

using namespace std;

class Graph {
private:
    vector<set<int>> adj;
    vector<int> colors;
    stack<int> q;

public:
    Graph(int n){
        adj.resize(n);
        colors.resize(n, 0);
    }
    ~Graph() = default;
    void add(int from, int to) {
        adj[from].insert(to);
    }

    void DFS(int s) {
        if (colors[s] != 0)
            return;
        colors[s] = 1;
        for (auto u : adj[s])
            if (colors[u] == 0)
                DFS(u);
        colors[s] = 2;
        q.push(s);
    }
    void prepare(int n){
        assert(q.empty());
    }
    void print(){
        for(int i = 0; i < 10; ++i) {
            cout << char(i + 'A') << ":";
            for (auto u : adj[i])
                cout << char(u + 'A') << " ";
            cout << endl;
        }
    }
    void print_q(){
        while(!q.empty()) {
            cout << char(q.top() + 'A') << " ";
            q.pop();
        }
    }

};

int main() {
    Graph G(10);
    G.prepare(10);

    string s1, s2;
    int count = 0;

    getline(cin, s1);

    while (getline(cin, s2) && s2 != "eof"){
        count++;
        for(int i = 0; i < s2.length(); ++i)
            if (s1[i] != s2[i]) {
                G.add(s1[i] - 'A', s2[i] - 'A');
                break;
            }
        swap(s1, s2);
    }
    //G.print();
    for(int i = 0; i < 10; ++i)
        G.DFS(i);

    G.print_q();
}
/*
BCHD
BCHG
BCGI
BCGF
BDIG
BDIJ
BDJH
BDJA
AFAB
AFAI
AFJA
AFJD
AEEF
AEEE
AECC
AECH
eof*/