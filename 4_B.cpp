/*B. Система бронирования
В систему бронирования последовательно приходят запросы от пассажиров
с указанием номера начальной и конечной станции,
а также количество билетов, которые пассажир хочет приобрести.
Требуемая скорость обработки каждого запроса - O(logN) */
#include <iostream>
#include <vector>

struct SegmentTree{
private:
    int max_capacity, stations;
    std::vector<int> array;
    //храним в массиве пару из текущей загрузки на перегоне и увеличения(проданные по пути)
    std::vector<std::pair<int, int>> tree;
public:
    SegmentTree(std::vector<int> &sold_tickets, int capacity) {
        max_capacity = capacity;
        array = sold_tickets;
        stations = array.size();

        tree.resize(2 * pow_2(stations), {0, 0});
        build_tree(0, 0, stations - 1);
    }

    void build_tree(int cur, int cur_l, int cur_r){
        //построение дерева сверху вниз
        if (cur_l == cur_r)
            tree[cur].first = array[cur_l];
        else {
            int median = (cur_l + cur_r) / 2;
            //левое поддерево
            build_tree(cur * 2 + 1, cur_l, median);
            //правое поддерево
            build_tree(cur * 2 + 2, median + 1, cur_r);
            //в вершине максимум из поддеревьев
            tree[cur].first = max(tree[cur * 2 + 1].first, tree[cur * 2 + 2].first);
        }
    }

    int get_max_on(int from, int to){
        return get_max_segment(0, 0, stations - 1, from, to - 1);
    }

    //[left, right] - отрезок, на котором ищем максимум
    int get_max_segment(int cur, int cur_l, int cur_r, int left, int right){
        //текущий подотрезок полностью вкладывается в искомый отрезок
        if (cur_l >= left && cur_r <= right) {
            return tree[cur].first + tree[cur].second;
        }

        //если в неправильном месте ищем
        if (cur_r < left || cur_l > right) {
            return 0;
        }

        int median = (cur_l + cur_r) / 2;
        //вызываем рекурсивно в потомках
        int left_max = get_max_segment(cur * 2 + 1, cur_l, median, left, right);

        int right_max = get_max_segment(cur * 2 + 2, median + 1, cur_r, left, right);
	//учитываем добавку
        return max(left_max, right_max) + tree[cur].second;
    }

    void increase_on(int from, int to, int incr){
        //так как считаем по сути перегоны, а не станции, f - 1
        increase_segment(0, 0, stations - 1, from, to - 1, incr);
    }

    void increase_segment(int cur, int cur_l, int cur_r, int left, int right, int incr) {
        //если попали, увеличиваем добавку
        if (left <= cur_l && cur_r <= right) {
            tree[cur].second += incr;
            return;
        }

        // если не там ищем
        if (right < cur_l || cur_r < left)
            return;

        int median = (cur_l + cur_r) / 2;
        //вызываем рекурсивно в детях
        increase_segment(2 * cur + 1, cur_l, median, left, right, incr);
        increase_segment(2 * cur + 2, median + 1, cur_r, left, right, incr);
	//обновляем cur
        tree[cur].first = max(tree[2 * cur + 1].first + tree[2 * cur + 1].second,
                tree[2 * cur + 2].first + tree[2 * cur + 2].second);
    }

    int max(int a, int b){
        return a > b ? a : b;
    }

    //вывод дерева отрезков
    void print(){
        for(auto i : tree)
            std::cout << i.first + i.second << " ";
    }

    //поисх наименьшей степени двойки >= n
    int pow_2(int n){
        int s = 0;
        bool flag = true;

        while(n){
            if (n == 1) {
                if(!flag)
                    ++s;

                return 1 << s;
            }
            if (n % 2 != 0)
                flag = false;
            n /= 2;
            ++s;
        }
    }

    //основная функция, отвечающая на запросы
    bool function_request(int from, int to, int num){
        int free_tickets = max_capacity - get_max_on(from, to);

        if (free_tickets >= num){
            increase_on(from, to, num);
            return true;
        } else
            return false;
    }
};

int main() {
    int N, capacity, num_requests; std::cin >> N;
    std::vector<int> sold_tickets(N - 1);

    for(int i = 0; i < N - 1; ++i) {
        std::cin >> sold_tickets[i];
    }

    std::cin >> capacity >> num_requests;
    SegmentTree T(sold_tickets, capacity);

    int from, to, num;

    for(int i = 0; i < num_requests; ++i){
        std::cin >> from >> to >> num;

        if(!T.function_request(from, to, num))
            std::cout << i  << " ";
    }
    return 0;
}
/*
 5 5 2 8 4
 1
 */

/*
6
1 0 0 0 1
5
8
0 2 1
1 4 2
2 5 1
1 3 3
3 5 2
0 4 1
0 1 3
1 3 2
 */
