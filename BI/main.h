#include <iostream>
#include <vector>
#include <string>

typedef long long ll;

class BigInteger;
bool operator <= (const BigInteger& a, const BigInteger& b);
bool operator < (const BigInteger& a, const BigInteger& b);
bool operator == (const BigInteger& a, const BigInteger& b);
BigInteger operator * (const BigInteger& a, const BigInteger& b);

class BigInteger{
private:
    size_t base = 1e6;
    bool sign;  //bigint is negative if sign = 1
    std::vector<size_t> digits;

public:
    int num_base() const {
        int n_base = base, n = 0;
        while (n_base){
            n_base /= 10;
            n++;
        }
        return n - 1;
    }//подсчет количества цифр в базу

    int size_vec (size_t x) const {
        int n = 0;
        while (x){
            n++;
            x /= base;
        }
        return n;
    }//подсчет размера вектора

    //показывает буфер
    const std::vector<size_t>& GetDigits() const {
        return digits;
    }
    //показывает знак
    const bool& GetSign() const {
        return sign;
    }

    //конструктор копирования
    BigInteger(const BigInteger& a){
        sign = a.sign;

        for(int digit : a.digits)
            digits.push_back(digit);
    }

    //дефолтный конструктор
    BigInteger(){
        sign = false;
        digits.push_back(0);
    }

    //конструктор от int
    BigInteger(const int& inp){
        sign = false;
        ll input = inp;

        if (input < 0) {
            sign = true;
            input = -input;
        }

        if (input == 0) {
            digits.push_back(0);
            return;
        }

        digits.resize(size_vec(input), -1);

        int num = 0;
        while (input){
            digits[num] = input % base;
            num++;
            input /= base;
        }
    }

    //конструктор от строки
    explicit BigInteger(const std::string &s){
        if(s.empty())
            return;

        std::string str;
        for(char i : s)
            str += i;

        sign = (str[0] == '-' ? 1 : 0);

        if (sign)//if negative
            str.erase(0, 1);

        size_t str_size = str.size();

        size_t size_vec = ((str_size % num_base()) == 0 ? str_size / num_base() : str_size / num_base() + 1);//size of vector
        digits.resize(size_vec, -1);

        //по методу Горнера бежим с начала строки, считывая по числу, помещающемуся в одну базу
        size_t last_size = str_size % num_base();//самый больщой разряд имеет произвольный размер от 0 до размера базы
        //на i позиции хранится число base ^ i
        size_t cur = 0;
        size_t count = 0;

        for(size_t i = 0; i < str_size; ++i){
            cur *= 10;
            cur += str[i] - '0';

            if (i + 1 >= last_size && (i + 1 - last_size) % num_base() == 0) {
                digits[size_vec - count - 1] = cur;
                cur = 0;
                ++count;
            }
        }
    }

    //унарный минус
    BigInteger operator - () const {
        BigInteger a = *this;
        a.sign = 1 - a.sign;
        return a;
    }

    //постфиксный инкремент/декремент
    BigInteger operator ++ (int) {
        BigInteger a = (*this);
        ++*this;
        return a;
    }

    BigInteger operator -- (int) {
        BigInteger a = (*this);
        --*this;
        return a;
    }

    //префиксный..
    BigInteger& operator++() {
        if(!sign) {
            digits[0] += 1;
            if (digits[0] >= base)//только в этом случае нормируем
                NormRight();
        } else {//если отрицательное число
            if (digits[0] != 0)
                digits[0] -= 1;
            else
                *this -= BigInteger(1);
        }

        return *this;
    }

    BigInteger& operator--() {
        if(!sign) {
            if(digits[0] != 0)
                digits[0] -= 1;
            else
                *this -= BigInteger(1);
        } else {//если отрицательное число
            digits[0] += 1;
            if (digits[0] >= base)
                NormRight();
        }
        return *this;
    }

    //нормализация bigint(если больше или равно, чем в базе, "перекидываем" на следующий разряд
    void NormRight(){
        for(size_t i = 0; i < digits.size(); ++i){
            if (digits[i] >= base){
                if (i == digits.size() - 1) {
                    digits.push_back(digits[i] / base);
                    digits[i] = digits[i] % base;
                }
                else {
                    digits[i + 1] += digits[i] / base;
                    digits[i] = digits[i] % base;
                }
            }
        }
        if (digits[digits.size() - 1] == 0)
            sign = false;
    }

    BigInteger& operator += (const BigInteger& a) {
        int min = a.digits.size();

        if (a.digits.size() > digits.size()) {
            digits.resize(a.digits.size());
            min = digits.size();
        }

        if (sign == a.sign) {
            for (int i = 0; i < min; ++i) {
                digits[i] += a.digits[i];
            }
            NormRight();
            return *this;
        }
        else {
            BigInteger b = a;
            b.sign = 1 - b.sign;
            return *this -= b;
        }
    }

    void Normalize(){//избавление от лишних нулей в начале digits
        size_t i = digits.size() - 1;
        while (i > 0 && digits[i] == 0)
            --i;
        ++i;

        digits.resize(i);
        if (digits[digits.size() - 1] == 0)
            sign = false;
    }

    BigInteger& operator -= (const BigInteger& a) {
        if (!sign && !a.sign && a <= *this){//случай когда из положительного вычитаем меньшее положительное(столбиком)
            int min = a.digits.size();
            for(ll i = 0; i < min; ++i) {
                if(digits[i] >= a.digits[i])
                    digits[i] -= a.digits[i];
                else {
                    digits[i] = digits[i] + base - a.digits[i];
                    if (digits[i + 1] != 0) {
                        digits[i + 1] -= 1;
                    }
                    else{
                        ll j = i + 1;
                        while(digits[j] == 0){
                            digits[j] = base - 1;
                            ++j;
                        }
                        digits[j] -= 1;
                    }
                }
            }
            Normalize();
            return *this;
        }
        //остальные случае сводятся к предыдущему и +=
        else if (!sign && !a.sign && *this <= a) {
            BigInteger b = a;
            b -= *this;
            *this = b;
            sign = true;
            Normalize();
            return *this;
        }

        else if (!sign && a.sign) {
            BigInteger b = a;
            b.sign = false;
            return *this += b;
        }

        else if (sign && !a.sign){
            BigInteger b = a;
            b.sign = true;
            return *this += b;
        }

        else if (sign && a.sign && a <= *this) {
            BigInteger b = a;
            BigInteger c = *this;
            b.sign = false;
            c.sign = false;
            b -= c;
            *this = b;
            Normalize();
            return *this;
        }

        else {
            BigInteger b = a;
            BigInteger c = *this;
            b.sign = false;
            c.sign = false;
            c -= b;
            c.sign = true;
            *this = c;
            Normalize();
            return *this;
        }
    }

    int num_digit(const int &y) const {
        int n = 0, x = y;
        if(x == 0)
            return 1;
        while (x){
            n++;
            x /= 10;
        }
        return n;
    }

    //вывод в строку
    std::string toString() const {
        std::string res;

        if (sign && digits[digits.size() - 1] != 0)//чтобы не -0
            res += "-";

        for (size_t i = 0; i < digits.size(); ++i) {
            if (i != 0 && num_digit(digits[digits.size() - 1 - i]) < num_base()) {
                int j = 0;
                while (j < num_base() - num_digit(digits[digits.size() - 1 - i])) {
                    res += "0";
                    ++j;
                }
            }
            res += std::to_string(digits[digits.size() - 1 - i]);
        }
        return res;
    }

    explicit operator bool() {
        return !(digits[0] == 0 && digits.size() == 1);
    }

////////////////////////////////////////////multiplying////////////////////////

    void vec_norm(std::vector<size_t> &a){//аналогичное нормаирование вектора
        size_t j = a.size() - 1;

        while (a[j] == 0 && j > 0)//избавляемся от последних нулей
            --j;
        ++j;

        a.resize(j);

        for(size_t i = 0; i < a.size(); ++i){//нормализация(перкидывание лишнего)
            if (a[i] >= base){
                if (i == a.size() - 1) {
                    a.push_back(a[i] / base);
                    a[i] = a[i] % base;
                    break;
                }
                else {
                    a[i + 1] += a[i] / base;
                    a[i] = a[i] % base;
                }
            }
        }
    }

    //наивное умножение
    static std::vector<size_t> Naive(const std::vector<size_t>& a, const std::vector<size_t>& b){
        std::vector<size_t> res(2 * a.size());

        for (size_t i = 0; i < a.size(); ++i) {
            for (size_t j = 0; j < a.size(); ++j) {
                res[i + j] += a[i] * b[j];
            }
        }
        return res;
    }

    //приведение векторов к равной длине(степени двойки) дполонением нулями в начало
    void EqSize (std::vector<size_t> &a_vec){
        size_t max = digits.size();//приведение к нужному размеру
        if (a_vec.size() > max) {
            max = a_vec.size();
        }

        while (max & (max - 1))//получение степени двойки
            ++max;

        digits.resize(max, 0);
        a_vec.resize(max, 0);
    }

    //алгоритм Каракубы
    std::vector<size_t>Karatsuba(const std::vector<size_t>& a, const std::vector<size_t>& b) {
        //A= a1*x + a2, B = b1x + 2 --> A*B = a1b1x^2 + a2b2 + ((a1+a2)(b1+b2) - a1b1 - a2b2) * x
        //уже имеем одинаковые размеры, степени 2
        size_t max = a.size();

        std::vector<size_t> res(max * 2);

        //результат не превзойдет этого размера
        size_t len = a.size() / 2;

        if (max <= 2) {
            return Naive(a, b);
        }

        std::vector<size_t> a_1{a.begin(), a.begin() + len};
        std::vector<size_t> a_2{a.begin() + len, a.end()};
        std::vector<size_t> b_1{b.begin(), b.begin() + len};
        std::vector<size_t> b_2{b.begin() + len, b.end()};

        //a1 * b1 =: d1
        std::vector<size_t> d1 = Karatsuba(a_1, b_1);
        //a2 * b2 =: d2
        std::vector<size_t> d2 = Karatsuba(a_2, b_2);

        //(a1+a2)(b1+b2) =: h1 * h2
        std::vector<size_t> h1(len);
        std::vector<size_t> h2(len);

        for (size_t i = 0; i < len; ++i) {
            h1[i] = a_1[i] + a_2[i];
            h2[i] = b_1[i] + b_2[i];
        }

        //d3 :=(a1+a2)(b1+b2) - a1b1 - a2b2
        std::vector<size_t> d3 = Karatsuba(h1, h2);

        substr(d3, d2, d1);

        //запись в результат
        for (size_t i = 0; i < max; ++i){
            res[i] = d1[i];
            res[i + max] = d2[i];
        }

        for (size_t i = len; i < len + max; ++i)
            res[i] += d3[i - len];

        return res;
    }

    BigInteger& operator *= (const BigInteger& a) {
        if ((digits[0] == 0 && digits.size() == 1) || (a.digits[0] == 0 && a.digits.size() == 1)) {
            digits.resize(1);
            digits[0] = 0;
            return *this;
        } else {
            sign = sign != a.sign;//определили знак

            std::vector<size_t> a_vec;
            for (unsigned long digit : a.digits)
                a_vec.push_back(digit);//копия буфера a

            EqSize(a_vec);//привели к нужному размеру

            std::vector<size_t> res = Karatsuba(digits, a_vec);

            vec_norm(res);//только сейчас нормируем

            digits.resize(res.size());

            for (size_t i = 0; i < res.size(); ++i) {
                digits[i] = res[i];
            }

            return *this;
        }
    }

    void substr(std::vector<size_t> &d3, std::vector<size_t> &d2, std::vector<size_t> &d1){
        if (d1.size() < d3.size()){
            d1.resize(d3.size(), 0);
            d2.resize(d3.size(), 0);
        }

        for(size_t i = 0; i < d3.size(); ++i) {
            if(d3[i] >= (d2[i] + d1[i]))
                d3[i] -= (d2[i] + d1[i]);
            else {
                while (d3[i] < (d2[i] + d1[i])){
                    if (d3[i + 1] != 0) {
                        d3[i + 1] -= 1;
                        d3[i] += base;
                    }
                    else{
                        ll j = i + 1;
                        while(d3[j] == 0){
                            d3[j] = base - 1;
                            ++j;
                        }
                        d3[j] -= 1;
                    }
                }
                d3[i] -= (d2[i] + d1[i]);
            }
        }
    }

/////////////////////////////dividing///////////////////////////////////////
    BigInteger& operator /= (const BigInteger& a) {

        if (mod_dif(a)) {//если делитель по модулю больше делителя
            digits.resize(1);
            digits[0] = 0;
        } else if (digits.size() == 1 && a.digits.size() == 1){
            digits[0] /= a.digits[0];

        } else {//бинарныым поиском ответа от 1 до *this
            BigInteger copy = *this;
            copy.sign = false;

            BigInteger cur = div_by_2(*this);//текущий
            cur.sign = false;

            BigInteger div = a;//делитель
            div.sign = false;

            BigInteger left(0);
            BigInteger right = *this;

            while (true){
                if (copy < cur * div){
                    right = --cur;
                    cur = bin_s(left, right);
                } else {
                    if (copy < ++cur * div){//если следующий уже больше, чем нужно
                        bool flag = (sign != a.sign);
                        *this = --cur;
                        sign = flag;
                        break;
                    } else {
                        left = cur;
                        cur = bin_s(left, right);
                    }
                }
            }
        }
        return *this;
    }

    //возврщает bigint "посередине"
    BigInteger bin_s (BigInteger &a, BigInteger &b){
        BigInteger copy = a;
        copy += b;
        return div_by_2(copy);
    }

    //деление на 2
    BigInteger div_by_2 (BigInteger a) {
        for (ll i = a.digits.size() - 1; i >= 0 ; --i){
            if (a.digits[i] % 2 && i != 0)
                a.digits[i - 1] += a.digits[i] % 2 * base;
            a.digits[i] = a.digits[i] / 2;
        }
        a.Normalize();
        return a;
    }

    //сравнение по модулю
    bool mod_dif(const BigInteger& a) const {
        BigInteger b = *this;
        BigInteger c = a;
        b.sign = false;
        c.sign = false;
        return b < c;
    }

    BigInteger& operator %= (const BigInteger& a) {
        BigInteger b = *this;
        b /= a;
        b *= a;
        *this -= b;
        return *this;
    }
};

////////////////////////// end of the class///////////////////////////////////////*/

//поток вывода
std::ostream& operator << (std::ostream& stream, const BigInteger &a){
    stream << a.toString();
    return stream;
}

bool IsDigit(char &x){
    return x >= '0' && x <= '9';
}

bool IsMinus(char &x){
    return x == '-';
}

//поток ввода
std::istream& operator >> (std::istream& stream, BigInteger &a){
    char cur;
    cur = stream.get();
    std::string inp_str;

    //все ненужные символы пропускаем
    while(!(IsDigit(cur) || IsMinus(cur))){
        cur = stream.get();
    }

    if (IsDigit(cur) || IsMinus(cur)){
        if(IsMinus(cur)){
            inp_str += '-';
            cur = stream.get();
        }

        while(IsDigit(cur)){
            inp_str += cur;
            cur = stream.get();
        }
    }
    a = BigInteger(inp_str);
    return stream;
}

bool operator < (const BigInteger& a, const BigInteger& b) {
    if (a.GetSign() > b.GetSign())
        return true;
    if (a.GetSign() < b.GetSign())
        return false;
    if (a.GetSign() + b.GetSign() == 0){//случай оба положительных числа
        if (a.GetDigits().size() < b.GetDigits().size())
            return true;
        else if (a.GetDigits().size() > b.GetDigits().size())
            return false;
        else { //случай, когда их размеры равны и нужно бежать с наибольшего разряда
            bool flag = false;
            for(ll i = a.GetDigits().size() - 1; i >= 0; --i){
                if (a.GetDigits()[i] < b.GetDigits()[i]) {
                    flag = true;
                    break;
                } else if(a.GetDigits()[i] > b.GetDigits()[i]){
                    flag = false;
                    break;
                } else//только когда разряды совпали
                    continue;
            }
            return flag;
        }
    } else {//оба отрицательные
        if (a.GetDigits().size() > b.GetDigits().size())
            return true;
        else if (a.GetDigits().size() < b.GetDigits().size())
            return false;
        else { //случай, когда их размеры равны и нужно бежать с наибольшего разряда
            bool flag = false;
            for(ll i = a.GetDigits().size() - 1; i >= 0; --i){
                if (a.GetDigits()[i] > b.GetDigits()[i]) {
                    flag = true;
                    break;
                }
                else if(a.GetDigits()[i] < b.GetDigits()[i]) {
                    flag = false;
                    break;
                } else
                    continue;
            }
            return flag;
        }
    }
}

bool operator == (const BigInteger& a, const BigInteger& b) {
    if (a.GetDigits().size() != b.GetDigits().size() || a.GetSign() != b.GetSign())
        return false;
    else {
        bool flag = true;
        for(ll i = a.GetDigits().size() - 1; i >= 0; --i){
            if (a.GetDigits()[i] != b.GetDigits()[i]) {
                flag = false;
                break;
            }
        }
        return flag;
    }
}

bool operator > (const BigInteger& a, const BigInteger& b) {
    return !(a < b) && !(a == b);
}

bool operator >= (const BigInteger& a, const BigInteger& b) {
    return !(a < b);
}

bool operator <= (const BigInteger& a, const BigInteger& b) {
    return a < b || a == b;
}

bool operator != (const BigInteger& a, const BigInteger& b) {
    return !(a == b);
}
/////////////////////////////////////////////////////////////////////////////

BigInteger operator + (const BigInteger& a, const BigInteger& b) {
    BigInteger c = a;
    c += b;
    return c;
}

BigInteger operator + (const int& a, const BigInteger& b) {
    BigInteger c(a);
    c += b;
    return c;
}

BigInteger operator + (const BigInteger& a, const int& b) {
    BigInteger c(b);
    c += a;
    return c;
}

BigInteger operator - (const BigInteger& a, const BigInteger& b) {
    BigInteger c = a;
    c -= b;
    return c;
}

BigInteger operator - (const int& a, const BigInteger& b) {
    BigInteger c(a);
    c -= b;
    return c;
}

BigInteger operator - (const BigInteger& a, const int& b) {
    BigInteger c(b);
    c = a - c;
    return c;
}

BigInteger operator * (const BigInteger& a, const BigInteger& b) {
    BigInteger c = a;
    c *= b;
    return c;
}

BigInteger operator * (const int& a, const BigInteger& b) {
    BigInteger c(a);
    c *= b;
    return c;
}

BigInteger operator * (const BigInteger& a, const int& b) {
    BigInteger c(b);
    c *= a;
    return c;
}

BigInteger operator / (const BigInteger& a, const BigInteger& b) {
    BigInteger c = a;
    c /= b;
    return c;
}

BigInteger operator / (const int& a, const BigInteger& b) {
    BigInteger c(a);
    c /= b;
    return c;
}

BigInteger operator / (const BigInteger& a, const int& b) {
    BigInteger c(b);
    c = a / c;
    return c;
}

BigInteger operator % (const BigInteger& a, const BigInteger& b) {
    BigInteger c = a;
    c %= b;
    return c;
}

BigInteger operator % (const int& a, const BigInteger& b) {
    BigInteger c(a);
    c %= b;
    return c;
}

BigInteger operator % (const BigInteger& a, const int& b) {
    BigInteger c(b);
    c = a % c;
    return c;
}