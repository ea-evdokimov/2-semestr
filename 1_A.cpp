/*A. За молоком
Леон и Матильда собрались пойти в магазин за молоком.
Каково минимальное количество улиц, по которым пройдёт хотя бы один из ребят?*/
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

    int min_sum(int leon, int mat, int milk){
        vector<int> leon_dist(neib.size(), 0);
        vector<int> mat_dist(neib.size(), 0);
        vector<int> milk_dist(neib.size(), 0);
        //подсчет с помощью bfs расстояний до всех вершин от заданной
        BFS(mat - 1, mat_dist);
        BFS(leon - 1, leon_dist);
        BFS(milk - 1, milk_dist);

        //выбор минимальной суммы
    	int min = leon_dist[0] + mat_dist[0] + milk_dist[0];

        for(size_t i = 1; i < neib.size(); ++i)
            if(leon_dist[i] + mat_dist[i] + milk_dist[i] < min)
                min = leon_dist[i] + mat_dist[i] + milk_dist[i];

        return min;
    }

    void BFS(int s, vector<int>& dist){
        vector<bool> used(neib.size(), 0);
        queue<int> q;
        used[s] = true;
        q.push(s);

        while(!q.empty()){
            int cur = q.front();
            q.pop();

            for(auto next : neib[cur])
                if(!used[next]){
                    used[next] = true;
                    q.push(next);
                    dist[next] = dist[cur] + 1;
                }
        }
    }

    void print(){
        for(size_t i = 0; i < neib.size(); ++i) {
            cout << i << ":";
            for (auto u : neib[i])
                cout << u << " ";
            cout << endl;
        }
    }
};

int main() {
    int num_vert, num_edge;
    cin >> num_vert >> num_edge;

    int leon, mat, milk;
    cin >> leon >> mat >> milk;

    Graph G(num_vert);

    int from, to;

    for(int i = 0; i < num_edge; ++i) {
        cin >> from >> to;
        G.add(from - 1, to - 1);
    }

    cout << G.min_sum(leon, mat, milk);
}
