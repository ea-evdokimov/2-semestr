/*D. Look What You Made Rick Do
 * Чтобы Морти не потерялся, Рику необходимо узнать,
 * между какими вселенными существуют пути, а между какими нет.
 * Выведите матрицу, где единица в i-й строке j-м столбце обозначает существование пути
 * из i-й вселенной в j-ю и ноль — его отсутствие.*/
#include <iostream>
#include <vector>
#include <string>

void transitive_closure(std::vector<std::vector<int>>& matrix, int M, int base, int last) {
    //пробегаемся по всем таким k, которые могут быть промежуточными в пути между i и j
    //если есть путь из i в k и из k в j, то есть и из i в j
    int s;
    for (int k = 0; k < matrix.size(); ++k) {
        for (int i = 0; i < matrix.size(); ++i) {
            //смотрим на бит под номером b - то есть логическое "и" с 1 << (base - 1 - b)
            //если последний
            if (k / base == (M - 1) && last != 0) {
                s = (1 << (last - 1 - k % base)) & (matrix[i][k / base]);
                if (s != 0)
                    for (int j = 0; j < M; ++j)
                        matrix[i][j] = matrix[i][j] | matrix[k][j];
            }
            //если это не последний
            else {
                s = 1 << (base - 1 - k % base) & (matrix[i][k / base]);
                if (s != 0)
                    for (int j = 0; j < M; ++j)
                        matrix[i][j] = matrix[i][j] | matrix[k][j];
            }
        }
    }
}

int to_dec(std::string &s){
    int res = 0;
    for(int i = 0; i < s.size() - 1; ++i){
        res += (s[i] == '1');
        res <<= 1;
    }
    res += (s[s.size() - 1] == '1');

    return res;
}

//перевод в строку из 0 и 1
std::string to_bin(int key, int base, int last, int flag){
    std::string res;

    while(key != 0){
        if(key % 2 == 1)
            res += "1";
        else
            res += "0";
        key >>= 1;
    }

    if (flag)
        while(res.size() < last)
            res += "0";
    else
        while(res.size() < base)
            res += "0";

    int len = res.size();

    for(int i = 0; i < len / 2; i++)
    {
        char t = res[i];
        res[i] = res[len - i - 1];
        res[len - i - 1] = t;
    }
    return res;
}

void input(std::vector<std::vector<int>> &matrix, int base){
    //для считывания строки
    std::string line;
    int N = matrix.size();
    //сколько будет чисел после разбиения по базе
    int M = N % base == 0 ? N / base : N / base + 1;
    int last = N % base;

    //разбиваем каждую строку на подсроки по base символов
    for(int i = 0; i < N; ++i) {
        std::cin >> line;

        std::string li;
        int cur_pos = 0;

        for(int j = 0; j < M - 1; ++j) {
            li = line.substr(cur_pos, base);
            matrix[i].push_back(to_dec(li));
            cur_pos += base;
        }
        if (last)
            li = line.substr(cur_pos, last);
        else
            li = line.substr(cur_pos, base);

        matrix[i].push_back(to_dec(li));
    }
}

int main() {
    int N; std::cin >> N;
    std::vector<std::vector<int>> matrix(N);

    int base = 8;
    //сколько будет чисел после разбиения по базе
    int M = N % base == 0 ? N / base : N / base + 1;
    //сколько в последнем
    int last = N % base;
    //считывание
    input(matrix, base);

    transitive_closure(matrix, M, base, last);

    //вывод
    for(int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            if (last != 0 && j == M - 1)
                std::cout << to_bin(matrix[i][j], base, last, 1);
            else
                std::cout << to_bin(matrix[i][j], base, last, 0);
        }
        std::cout << std::endl;
    }
    return 0;
}

/*12
010010001000
000000000001
000110001000
011011000000
000000000000
000000100000
010110000001
100000000100
100000000010
010000000000
110000000100
000000000000*/
/*
 8
00000000
00000000
00000000
00000000
00000000
00000000
00100000
01100000
 */