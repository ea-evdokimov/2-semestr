/*A. Вторая статистика
Дано число N и последовательность из N целых чисел. Найти вторую порядковую статистику на заданных диапазонах.
Для решения задачи используйте структуру данных Sparse Table.
Требуемое время обработки каждого диапазона O(1). Время подготовки структуры данных O(nlogn)*/
#include <iostream>
#include <vector>

struct SparceTable{
private:
    //input sequence
    std::vector<int> array;
    //sparce table
    std::vector<std::vector<std::pair<int, int>>> table;
    //array of logs
    std::vector<int> logs;
public:
    SparceTable(int N){
        logs.resize(N + 1);

        //get array of logarithms
        logs[1] = 0;
        for (int i = 2; i <= N; i++)
            logs[i] = logs[i / 2] + 1;

        //table has sizes N * logN
        table.resize(N);

        for(int i = 0; i < N; ++i)
            table[i].resize(logs[N] + 1);
    }

    void add_sequence(int num){
        array.push_back(num);
    }

    void precompute() {
        int N = array.size(), K = table[0].size();
        //there are pairs in table with 1 statistics and 2 statistics on [i;j]
        //1st row of table
        int big = 1e6;
        for (int j = 0; j < N; ++j) {
            if(j == N - 1)
                table[j][0] = {array[j], big};
            else
                table[j][0] = get_right_pair(array[j], array[j + 1]);
        }

        //last rows dynamically
        for (int i = 1; i < K; ++i) {
            //так как прибавляя степень двойки мы выйдем за пределы таблицы
            int last = 1 << i;

            for (int j = 0; j <= N - last; ++j) {
                //сдвиг клетки, с которой мы будем сравнивать предыдущую
                int shift = 1 << (i - 1);
                table[j][i] = get_min_pair(table[j][i - 1], table[j + shift][i - 1]);
            }
        }
    }

    std::pair<int, int> get_right_pair(int a, int b){
        std::pair<int, int> res;
        if (a < b)
            res = {a, b};
        else
            res = {b, a};
        return res;
    }

    std::pair<int, int> get_min_pair(std::pair<int, int> a, std::pair<int, int> b){
        std::pair<int, int> res;
        if (a.first < b.first){
            res.first = a.first;
            if(a.second < b.first)
                res.second = a.second;
            else
                res.second = b.first;
        }
        else {
            res.first = b.first;
            if(b.second < a.first)
                res.second = b.second;
            else
                res.second = a.first;
        }

        if (a.first == b.first){
            res.first = a.first;
            res.second = a.second < b.second ? a.second : b.second;
        }

        return res;
    }

    int get_2_stat(int s, int f){
        int res = 0, w = is_2_pow(f - s);
        if (w != -1)
            res = table[s][w].second;
        else {
            int k = logs[f - s];
            //считаем как 2 статичстика на отрезкач [s, s + 2^k], [f - 2^k, f]
            res = get_min_pair(table[s][k], table[f - (1 << k)][k]).second;
        }
        return res;
    }

    int is_2_pow(int n){
        int s = 0;

        while(n){
            if (n == 1)
                return s;
            if (n % 2 != 0)
                return -1;
            n /= 2;
            ++s;
        }
    }
};

int main() {
    int N, M, num = 0; std::cin >> N >> M;
    //std::cin >> N;
    SparceTable S(N);

    //input sequence
    for(int i = 0; i < N; ++i){
        std::cin >> num;
        S.add_sequence(num);
    }

    S.precompute();

    //answer for requests
    int s, f;
//    while(true){
//        std::cin >> s >> f;
//        std::cout << S.get_2_stat(s - 1, f - 1) <<  std::endl;
//    }
    for(int i = 0; i < M; ++i){
        std::cin >> s >> f;
        std::cout << S.get_2_stat(s - 1, f - 1) << std::endl;
    }
    return 0;
}
/*
 6
 3 7 8 5 4 1
*/

/*
 10
 1 2 3 4 5 6 7 8 9 10
 */

/*
 10
 3 7 8 5 4 7 2 6 1 3
*/

