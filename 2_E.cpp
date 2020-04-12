/*E. Ricked out
 Рик обладает некоторой суммой в валюте S. Он задаётся вопросом, можно ли,
 после нескольких операций обмена увеличить свой капитал.
 Конечно, он хочет, чтобы в конце его деньги вновь были в валюте S.
 Если Рик может увеличить свой капитал, выведите YES, иначе выведите NO.*/
#include <iostream>
#include <vector>

//заведем струтуру для хранения ребер
struct Edge{
    int start;
    int finish;
    double rate;
    double comission;
    //важно то, что комиссия в валюте, а не в процентах
    Edge(int s, int f, double r, double c) : start(s), finish(f), rate(r), comission(c) {}
};

class Graph{
private:
    std::vector<Edge> edges;
public:
    void add_edges(int a, int b, double x, double y){
        edges.push_back(Edge(a, b, x, y));
    }

    bool FordBellman(int N, int S, double V){
        //если на последней итерации произошла релаксация, значит нашли"отрицательный цикл"
        //ходя по которому, сможем увеличивать состояние Рика
        int small = -1e9;
        std::vector<double> wealth(N, 0.0);
        //начальная вершина
        wealth[S] = V;

        int flag;
        for(int i = 0; i < N; ++i){
            //при каждой итерации пытаемся прорелаксировать значения по всем ребрам
            flag = 0;
            for (int j = 0; j < edges.size(); ++j){
                //пытаемся произвести релаксацию, если нам это выгодно
                double relax = (wealth[edges[j].start] - edges[j].comission) * edges[j].rate;
                if (wealth[edges[j].finish] < relax) {
                    wealth[edges[j].finish] = relax;
                    flag = 1;
                }
            }
        }
        //если flag, значит нашли "отрицательный цикл"
        return (wealth[S] > V || flag);
    }
};

int main() {
    int N, M, S, A, B;
    double V, r_ab, c_ab, r_ba, c_ba;
    std::cin >> N >> M >> S >> V;

    Graph G;
    //считывание ребер
    for(int i = 0; i < M; ++i){
        std::cin >> A >> B >> r_ab >> c_ab >> r_ba >> c_ba;
        G.add_edges(A - 1, B - 1, r_ab, c_ab);
        G.add_edges(B - 1, A - 1, r_ba, c_ba);
    }

    if (G.FordBellman(N, S - 1, V))
        std::cout << "YES";
    else
        std::cout << "NO";
    return 0;
}
/*
3 2 1 10.0
1 2 1.0 1.0 1.0 1.0
2 3 1.1 1.0 1.1 1.0
 */
/*
2 1 1 17.8
1 2 0.8 0.39 2 5
*/