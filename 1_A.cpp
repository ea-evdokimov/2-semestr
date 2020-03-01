#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class Graph {
private:
    vector<vector<int>> neib;
public:
    Graph(int n){
        neib.resize(n);
    }

    ~Graph() = default;

    void add(int from, int to) {
        neib[from].push_back(to);
        neib[to].push_back(from);
    }

    void function(int leon, int mat, int milk){
        vector<int> leon_dist(neib.size(), 0);
        vector<int> mat_dist(neib.size(), 0);
        vector<int> milk_dist(neib.size(), 0);
        BFS(mat - 1, mat_dist);
        BFS(leon - 1, leon_dist);
        BFS(milk - 1, milk_dist);

        int min = neib.size();

        for(int i = 0; i < neib.size(); ++i)
            if(leon_dist[i] + mat_dist[i] + milk_dist[i] < min)
                min = leon_dist[i] + mat_dist[i] + milk_dist[i];
        cout << min;
    }


    void BFS(int s, vector<int>& dist){
        vector<bool> used(neib.size(), 0);
        queue<int> q;
        used[s] = true;
        q.push(s);

        while(!q.empty()){
            int cur = q.front();
            q.pop();
            for(auto i : neib[cur])
                if(!used[i]){
                    used[i] = true;
                    q.push(i);
                    dist[i] = dist[cur] + 1;
                }
        }
    }

    void print(){
        for(int i = 0; i < neib.size(); ++i) {
            cout << i << ":";
            for (auto u : neib[i])
                cout << u << " ";
            cout << endl;
        }
    }
};

int main() {
    int n, m;
    cin >> n >> m;

    int leon, mat, milk;
    cin >> leon >> mat >> milk;

    Graph G(n);

    int from, to;

    for(int i = 0; i < m; ++i) {
        cin >> from >> to;
        G.add(from - 1, to - 1);
    }

    G.function(leon, mat, milk);
}
/*
 6 5
1 5 2
1 3
3 4
4 2
4 5
4 6
 */
/*
 3 2 1 2 3
1 3
2 3
 */