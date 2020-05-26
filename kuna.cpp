#include <iostream>
#include <vector>
/*5 5 10
 0 0
 0 1
 0 2
 1 0
 2 1
 2 3
 3 2
 3 3
 3 4
 4 2
*/
struct Graph{
private:
    std::vector<std::vector<int>> graph;
    std::vector<int> pairs;
public:

    Graph(int n1, int m){
        graph.resize(n1);
        pairs.resize(m, -1);
    }

    void add(int x, int y){
        graph[x].push_back(y);
    }

    void kuna(int m){
        for(int i = 0; i < graph.size(); ++i) {
            std::vector<bool> used(graph.size(), false);
            try_kuna(i, used);
        }
    }

    bool try_kuna(int from, std::vector<bool> &used){
        if(used[from])
            return false;

        used[from] = true;

        for (auto to : graph[from]) {
            if (pairs[to] == -1 || try_kuna(pairs[to], used)) {
                pairs[to] = from;
                return true;
            }
        }
        return false;
    }
};

int main() {
    int n1, n2, m; std::cin >> n1 >> n2 >> m;
    int x, y;

    Graph G(n1, m);

    for(int i = 0; i < m; ++i){
        std::cin >> x >> y;
        G.add(x, y);
    }

    G.kuna(m);

    return 0;
}
