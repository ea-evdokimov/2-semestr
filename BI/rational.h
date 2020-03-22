#include "main.h"

class Rational{
private:
    BigInteger top;//числитель
    BigInteger bot;//заменатель
public:
    //конструкторы
    Rational (const int &x, const int &y = 1) : top(x), bot(y) {
        sign_fix();
    }
    Rational (const BigInteger &a, const BigInteger &b = 1) : top(a), bot(b) {
        sign_fix();
    }
    //дефолтный
    Rational () : top(0), bot(1) {}

    //конструктор копирования
    Rational(const Rational& a){
        top = a.top;
        bot = a.bot;
    }

    void sign_fix() {//оставляет знак только в числителе
        bool sign_cur = ((top < 0) != (bot < 0));

        if ((top < 0) != sign_cur)
            top = -top;
        if (bot < 0)
            bot = -bot;
    }

    void num_fix(){//к правильному виду
        BigInteger nod = NOD(top, bot);

        if (nod > 1) {
            top /= nod;
            bot /= nod;
        }
    }

    //НОД и НОК
    BigInteger NOD(BigInteger a, BigInteger b) const {
        a = a < 0 ? -a : a;
        b = b < 0 ? -b : b;

        while (a != 0 and b != 0){
            if (a > b)
                a %= b;
            else
                b %= a;
        }
        a += b;
        return a;
    }

    BigInteger NOK (const BigInteger &a, const BigInteger &b) const {
        BigInteger nod = NOD(a, b);

        BigInteger res = a;
        res *= b;
        res /= nod;

        return res;
    }

    std::string toString() {
        std::string res;
        res += top.toString();

        if (!(top % bot == BigInteger(0))) {
            res += "/";
            res += bot.toString();
        }
        return res;
    }

    Rational& operator += (const Rational &a){
        BigInteger nok = NOK(bot, a.bot);
        BigInteger a_n = nok / a.bot;
        BigInteger n = nok / bot;

        a_n *= a.top;
        n *= top;
        n += a_n;

        *this = Rational(n, nok);

        sign_fix();
        num_fix();

        return *this;
    }

    Rational& operator -= (const Rational &a) {
        BigInteger nok = NOK(bot, a.bot);
        BigInteger a_n = nok / a.bot;
        BigInteger n = nok / bot;

        a_n *= a.top;
        n *= top;
        n -= a_n;

        *this = Rational(n, nok);

        sign_fix();
        num_fix();

        return *this;
    }

    Rational& operator *= (const Rational &a) {
        top *= a.top;
        bot *= a.bot;

        sign_fix();
        num_fix();

        return *this;
    }

    Rational& operator /= (const Rational &a) {
        top *= a.bot;
        bot *= a.top;

        sign_fix();
        num_fix();

        return *this;
    }

    //унарный
    Rational operator -() const {
        Rational a = *this;
        a.top = -a.top;
        return a;
    }

    bool GetSign() const {
        return (top < 0);
    }

    bool IsZero() const {
        return (top == 0);
    }

    std::string asDecimal(size_t precision=0){
        std::string res;

        bool flag = (top < 0);
        if (flag) {
            res += "-";
            top = -top;
        }

        BigInteger main_part = top / bot;
        Rational p(main_part * bot, bot);
        *this -= p;

        res += main_part.toString();
        res += ".";

        BigInteger sub_part;

        //умножаем на 10 и вычитаем
        for (size_t i = 0; i < precision - 1; ++i){
            sub_part = top * 10 / bot;
            top = top * 10 - sub_part * bot;
            res += sub_part.toString();
        }

        //последнее цифру нужно округлить в зависимости от следующей
        sub_part = top * 10 / bot;
        top = top * 10 - sub_part * bot;
        if (sub_part.GetSign())
            sub_part = -sub_part;

        BigInteger last_part = top * 10 / bot;
        if (last_part.GetSign())
            last_part = -last_part;
        if (last_part >= BigInteger(5))
            sub_part = (sub_part + 1) % 10;

        res += sub_part.toString();

        return res;
    }

    explicit operator double() {
        std::string res_str = asDecimal(9);
        //сначала asDecimal, потом Горнер, потом в double
        double res;
        bool sign = (res_str[0] == '-' ? 1 : 0);
        size_t i = (sign) ? 1 : 0;

        //целая часть
        size_t cur = 0;
        while(res_str[i] != '.'){
            cur *= 10;
            cur += res_str[i] - '0';
            ++i;
        }
        ++i;
        //считывание части после запятой
        size_t sub_cur = 0;
        while(i < res_str.size()){
            sub_cur *= 10;
            sub_cur += res_str[i] - '0';
            ++i;
        }

        res = sign ? -(cur + sub_cur / 1e9) : cur + sub_cur / 1e9;

        return res;
    }

};

Rational operator + (const Rational& a, const Rational& b) {
    Rational c = a;
    c += b;
    return c;
}

Rational operator - (const Rational& a, const Rational& b) {
    Rational c = a;
    c -= b;
    return c;
}

Rational operator * (const Rational& a, const Rational& b) {
    Rational c = a;
    c *= b;
    return c;
}

Rational operator / (const Rational& a, const Rational& b) {
    Rational c = a;
    c /= b;
    return c;
}
////////////////////////////////////////////////////////////

bool operator < (const Rational& a, const Rational& b) {
    Rational c = b;
    c -= a;
    return (!c.GetSign() && !c.IsZero());
}

bool operator == (const Rational& a, const Rational& b) {
    Rational c = b;
    c -= a;
    return c.IsZero();
}

bool operator != (const Rational& a, const Rational& b) {
    return !(a == b);
}

bool operator > (const Rational& a, const Rational& b) {
    return !(a < b) && !(a == b);
}

bool operator <= (const Rational& a, const Rational& b) {
    return (a < b) || (a == b);
}

bool operator >= (const Rational& a, const Rational& b) {
    return (a > b) || (a == b);
}





