#include <iostream>
#include <vector>

//дерево с поддержкой минимума экспозиции на отрезке
struct SegmentTree{
private:
    std::vector<int> array;
    int size;
    //храним в массиве пару из текущей экспозиции на отрезке и изменения(перекрашивание)
    //может быть как положительной, так и отрицательной
    std::vector<std::pair<int, int>> tree;
public:
    SegmentTree(std::vector<int> &start_expo) {
        array = start_expo;
        size = array.size();
        tree.resize(2 * pow_2(size), {1e3, -1});
        //по дефолту если не было изменений, то -1

        build_tree(0, 0, size - 1);
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
            tree[cur].first = min(tree[cur * 2 + 1].first, tree[cur * 2 + 2].first);
        }
    }

    int get_min_on(int from, int to){
        return get_min_segment(0, 0, size - 1, from, to);
    }

    int get_min_segment(int cur, int cur_l, int cur_r, int left, int right){
        //если есть изменения, проталкиваем
        down(cur, cur_l, cur_r);

        //текущий подотрезок полностью вкладывается в искомый отрезок
        if (cur_l >= left && cur_r <= right) {
            return tree[cur].first;
        }

        //если в неправильном месте ищем
        if (cur_r < left || cur_l > right) {
            return 1e3;
        }

        int median = (cur_l + cur_r) / 2;

        //вызываем рекурсивно в потомках
        int left_min = get_min_segment(cur * 2 + 1, cur_l, median, left, right);

        int right_min = get_min_segment(cur * 2 + 2, median + 1, cur_r, left, right);

        return min(left_min, right_min);
    }

    //проталкивание
    void down(int cur, int cur_l, int cur_r){
        //если это узел(а не лист), отвечающий за какой-то отрезок перкрашен, проталкиваем это изменение на его детей
        if (tree[cur].second != -1) {
            if (cur_l != cur_r) {
                tree[2 * cur + 1].second = tree[cur].second;
                tree[2 * cur + 2].second = tree[cur].second;
            }
            // само значение в узле -
            tree[cur].first = tree[cur].second;
            tree[cur].second = -1;
        }
    }

    void change_on(int from, int to, int incr){
        change_segment(0, 0, size - 1, from, to, incr);
    }

    void change_segment(int cur, int cur_l, int cur_r, int left, int right, int incr) {
        down(cur, cur_l, cur_r);

        //если попали
        if (left <= cur_l && cur_r <= right) {
            tree[cur].second = incr;
            down(cur, cur_l, cur_r);
            return;
        }

        // если не там ищем
        if (right < cur_l || cur_r < left)
            return;

        int median = (cur_l + cur_r) / 2;
        //вызываем рекурсивно в детях
        change_segment(2 * cur + 1, cur_l, median, left, right, incr);
        change_segment(2 * cur + 2, median + 1, cur_r, left, right, incr);

        tree[cur].first = min(tree[2 * cur + 1].first ,tree[2 * cur + 2].first);
    }

    int min(int a, int b){
        return a < b ? a : b;
    }

    //вывод дерева отрезков
    void print(){
        for(auto i : tree)
            std::cout << i.first << " ";
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
    int function_request(int from_draw, int to_draw, int expo, int from_photo, int to_photo){
        change_on(from_draw, to_draw, expo);
        return get_min_on(from_photo, to_photo);
    }
};

int main() {
    int N; std::cin >> N;
    int R, G, B;
    std::vector<int> start_expo(N);

    for(int i = 0; i < N; ++i) {
        std::cin >> R >> G >> B;
        start_expo[i] = R + G + B;
        //std::cin >> start_expo[i];
    }

    SegmentTree T(start_expo);

    int num_requests; std::cin >> num_requests;
    int from_draw, to_draw, from_photo, to_photo;

    for(int i = 0; i < num_requests; ++i){
        std::cin >> from_draw >> to_draw;
        std::cin >> R >> G >> B;
        std::cin >> from_photo >> to_photo;

        std::cout << T.function_request(from_draw, to_draw, R + G + B, from_photo, to_photo) << " ";
    }
//    int x, y, z; char a;
//
//    while(true){
//        std::cin >> a;
//        if(a == 'g') {
//            std::cin >> x >> y;
//            std::cout << T.get_min_on(x, y) << std::endl;;
//        }
//        else if (a == 'c'){
//            std::cin >> x >> y >> z;
//            T.change_on(x, y, z);
//            std::cout << std::endl;
//        } else if (a == 'p') {
//            T.print();
//            std::cout << std::endl;
//        } else
//            break;
//    }

    return 0;
}

/*
 5
7 40 3
54 90 255
44 230 8
33 57 132
17 8 5
2
0 3 100 40 41 2 4
2 4 0 200 57 1 3
 */

/*
 5
7 40 3
54 90 255
44 230 8
33 57 132
17 8 5
0 3 181
 2 4
2 4 257
 1 3
 */