/*D. LCA
Задано дерево с корнем, содержащее (1 ≤ n ≤ 100 000) вершин, пронумерованных от 0 до n-1.
Требуется ответить на m (1 ≤ m ≤ 10 000 000) запросов о наименьшем общем предке для пары вершин.*/
#include <iostream>
#include <vector>
#include <cmath>

//метод двочиного подъема
struct LCAtree{
private:
    std::vector<std::vector<int>> tree;

    //массивы времен входа в и выхода из вершины i
    std::vector<int> time_in, time_out;
    std::vector<std::vector<int>> table;
    //в таблице на [i, j] стоит 2^j предок вершины i
public:
    LCAtree(std::vector<std::vector<int>> &input) : tree(input) {}

    void prepare(){
        int size = tree.size(), l = get_log(size);
        time_in.resize(size);
        time_out.resize(size);
        table.resize(size);

        for(int i = 0; i < size; ++i)
            table[i].resize(l, 0);

        int cur_time = 0;

        dfs(0, 0, cur_time);
    }

    int get_log(int n){
        return int(log2(n) + 1);
    }

    void dfs(int cur, int par, int &cur_time){
        time_in[cur] = cur_time;
        ++cur_time;

        table[cur][0] = par;

        //динамически заполняем таблицу двочиных подъемов
        for (int i = 1; i <= table[cur].size(); ++i) {
            table[cur][i] = table[table[cur][i - 1]][i - 1];
            //когда встречаем корень, дальше пойдут все нули
            if (table[cur][i] == 0)
                break;
        }

        for (auto next : tree[cur]) {
            if (next != par)
                dfs (next, cur, cur_time);
        }

        time_out[cur] = cur_time;
        ++cur_time;
    }

    //если вершина являетяс предком
    bool is_ancestor(int a, int b){
        return time_in[a] <= time_in[b] && time_out[b] <= time_out[a];
    }

    //ответ на сам запрос
    int lca (int a, int b) {
        //если одна вершина является предком другой, ничего искать не нужно
        if (is_ancestor(a, b))
            return a;
        if (is_ancestor(b, a))
            return b;

        for (int i = table[a].size() - 1; i >= 0; --i)
            if (!is_ancestor(table[a][i], b))
                a = table[a][i];
        return table[a][0];
    }

};

struct Request{
    size_t a1, a2, n;
    size_t x, y, z;

    Request(size_t a1_, size_t a2_, size_t n_, size_t x_, size_t y_, size_t z_) : a1(a1_), a2(a2_), n(n_), x(x_), y(y_), z(z_) {}

    void next_request(int answ, size_t &req1, size_t &req2){
        a1 = (x * a1 + y * a2 + z) % n;
        a2 = (x * a2 + y * a1 + z) % n;

        req1 = (a1 + answ) % n;
        req2 = a2;
    }
};

int main() {
    int n; size_t m; std::cin >> n >> m;
    std::vector<std::vector<int>> tree(n);
    int parent;

    for(int i = 1; i < n; ++i){
        std::cin >> parent;
        tree[parent].push_back(i);
    }

    LCAtree L(tree);
    L.prepare();

    size_t a1, a2; std::cin >> a1 >> a2;
    size_t x, y, z; std::cin >> x >> y >> z;
    size_t result = 0; int answ = 0;

    Request R(a1, a2, n, x, y, z);

    for(size_t i = 0; i < m; ++i) {
        answ =  L.lca(a1, a2);
        result += answ;
        R.next_request(answ, a1, a2);
    }

    std::cout << result;
//    int n; std::cin >> n;
//    std::vector<std::vector<int>> tree(n);
//    int parent;
//
//    for(int i = 1; i < n; ++i){
//        std::cin >> parent;
//        tree[parent].push_back(i);
//    }
//
//    LCAtree L(tree);
//    L.prepare();
//    int a1, a2;
//
//    while(true){
//        std::cin >> a1 >> a2;
//        std::cout << L.lca(a1, a2);
//    }
    return 0;
}

/*
8
0 0 1 1 4 5 4
*/

/*
3 2
0 1
2 1
1 1 0
 */

