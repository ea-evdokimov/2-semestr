#include <iostream>
#include <cmath>
#include <vector>
#include <cassert>

//с какой точностью будем сравнивать разность double
bool equal_double(double a, double b){
    return ((a - b) < 1e-10) && ((a - b) > -1e-10);
}

struct Point{
    double x, y;
    //дефолтный
    Point() : x(0), y(0) {}

    Point(double a, double b) : x(a), y(b) {}

    //конструктор копирования
    Point(const Point& A) : x(A.x), y(A.y) {}
};

bool operator == (const Point &A, const Point &B){
    return (equal_double(A.x, B.x) && equal_double(A.y, B.y));
}

bool operator != (const Point &A, const Point &B){
    return !(equal_double(A.x, B.x) && equal_double(A.y, B.y));
}

///////////////Vector////////////
struct Vector{
    double x, y;
    //дефолтный
    Vector() : x(0), y(0) {}

    //от координат
    Vector(double a, double b): x(a), y(b) {}

    //конструктор копирования
    Vector(const Vector &A) : x(A.x), y(A.y) {}

    //конструктор от двух точек
    Vector(const Point &A, const Point &B): x(B.x - A.x), y(B.y - A.y) {}

    //скалярное умножение
    double skal_mult(const Vector &another) const {
        return x * another.x + y * another.y;
    }

    //векторное умножение
    double mod_vec_mult(const Vector &another) const {
        double res = x * another.y - y * another.x;
        return res;
    }

    //нормализация вектора под нужную длину
    void normaling(double k){
        double old_len = len();
        x = x / old_len * k, y = y / old_len * k;
    }

    //длина
    double len() const{
        assert((x != 0) || (y != 0));
        return sqrt(x * x + y * y);
    }
};

bool operator == (const Vector &a, const Vector &b){
    return equal_double(a.y / a.x, b.y / b.x);
}

bool operator != (const Vector &a, const Vector &b){
    return !(a == b);
}
/////////////////Line////////////////
class Line{
private:
    //прямая будет задаваться точкой и направляющим ветором
    Point T;
    Vector t;
public:
    //конструктор от двух точек
    Line(const Point &A, const Point &B) : T(A), t(B.x - A.x, B.y - A.y) {}

    //конструктор от точки и угл коэффициента
    Line(const Point &A, double k) : T(A), t(1, k) {}

    //конструктор вида y = kx+b
    Line(double k, double b) : T(0, b), t(1, k) {}

    //конструктор от направляющего вектора и точки
    Line(const Point &N, const Vector &n) : T(N), t(n) {}

    double get_angle() const{
        return t.y / t.x;
    }

    //показывает точку
    const Point& GetPoint() const {
        return T;
    }

    //нужно получить нормаль до прямой
    Vector normal_to_line() const {
        Vector norm(t.y, -t.x);
        Vector to_T(T.x, T.y);

        //хочу нормаль, ведущую к прямой
        if (norm.skal_mult(to_T) < 0)
            norm.x *= -1.0, norm.y *= -1.0;

        //делаю ее размера как расстояние до прямой
        double dist = to_T.mod_vec_mult(t);
        if(dist < 0)
            dist = -dist;

        dist /= t.len();
        //подгоняем длину
        norm.normaling(dist);

        return norm;
    }
};

bool operator == (const Line &a, const Line &b){
    if (equal_double(a.get_angle(), b.get_angle())){
        //если направляющие вектора равны, остается проверить
        //что вектор, заданный двумя точками тоже равен им
        //отдельный случай, когда точки совпадают
        if (equal_double((b.GetPoint().y - a.GetPoint().y) /(b.GetPoint().x - a.GetPoint().x), a.get_angle()))
            return true;
        if (equal_double(a.GetPoint().x, b.GetPoint().x) && equal_double(a.GetPoint().y, b.GetPoint().y))
            return true;
    }
    return false;
}

bool operator != (const Line &a, const Line &b) {
    return !(a == b);
}
//////////////Shape & Polygon ///////////////////
class Shape{
public:
    virtual ~Shape() = default;
    virtual double perimeter() const = 0;
    virtual double area() const = 0;
    virtual bool isCongruentTo(const Shape& another) const = 0;
    virtual bool isSimilarTo(const Shape& another) const = 0;
    virtual bool containsPoint(Point point) const = 0;
    virtual bool operator == (const Shape& another) const = 0;

    virtual void rotate(Point center, double angle) = 0;
    virtual void reflex(Point center) = 0;
    virtual void reflex(Line axis) = 0;
    virtual void scale(Point center, double coefficient) = 0;
};

class Polygon : public Shape{
protected:
    std::vector<Point> verts;

    //для конструктора прямоугольника
    void add_vert(const Point &point){
        verts.push_back(point);
    }
public:
    //дефолтный конструктор
    Polygon() = default;

    //конструктор от вектора точек
    explicit Polygon(const std::vector<Point> &input) {
        for(auto i: input)
            verts.push_back(i);
    }

    //конструктор от непонятного количества точек
    template<typename Head, typename... Tail>
    Polygon(const Head& head, const Tail&... tail){
        verts.push_back(head);
        get_verts(tail...);
    }

    void get_verts(){}

    template<typename Head, typename... Tail>
    void get_verts(const Head& head, const Tail&... tail){
        verts.push_back(head);
        get_verts(tail...);
    }

    int verticesCount() const{
        return verts.size();
    }

    const std::vector<Point> &getVertices() const{
        return verts;
    }

    //проверка на выпуклость
    bool isConvex() const{
        //идея - посмотреть, в одну ли сторону "поворачивают" стороны многоугольника
        std::vector<bool> angles(verts.size(), false);
        //вектор для заполнения знаками углов

        for(size_t i = 0; i < verts.size(); ++i){
            //стороны a1 и a2
            //проверяем a1.x * a2.y - a1.y * a2.x
            angles[(i + 1) % verts.size()] = (verts[(i + 1) % verts.size()].x - verts[i].x) * (verts[(i + 2) % verts.size()].y - verts[(i + 1) % verts.size()].y) -
                                             (verts[(i + 1) % verts.size()].y - verts[i].y) * (verts[(i + 2) % verts.size()].x - verts[(i + 1) % verts.size()].x) < 0;
        }
        size_t sum = 0;
        //у нас заполнен массив знаков углов
        for(auto i :angles)
            sum += i;
        //если все отрицательные или все положительные, то многоугольник выпуклый
        return (sum == verts.size() || sum == 0);
    }

    //периметр
    double perimeter() const override{
        double res = 0;
        for(size_t i = 0; i < verts.size(); ++i){
            res += sqrt((verts[(i + 1) % verts.size()].x - verts[i].x) * (verts[(i + 1) % verts.size()].x - verts[i].x) +
                        (verts[(i + 1) % verts.size()].y - verts[i].y) * (verts[(i + 1) % verts.size()].y - verts[i].y));
        }
        return res;
    }

    //площадь находится методом гаусса
    double area() const override{
        double res = 0;

        for(size_t i = 0; i < verts.size(); ++i){
            res += (verts[i].x * verts[(i + 1) % verts.size()].y - verts[i].y * verts[(i + 1) % verts.size()].x);
        }
        //если отрицательная, меняем знак
        res = res / 2.0;
        res = res < 0 ? -res : res;
        return res;
    }

    bool containsPoint(Point point) const override{
        double min_x = verts[0].x, max_x = verts[0].x;
        double min_y = verts[0].y, max_y = verts[0].y;
        bool flag = false;//флаг,в котором будет итоговый ответ
        int size = verticesCount();

        //ограничим наш многоугольник прямоугольников, найдя наименьгие и наибольшие координаты
        for(int i = 0; i < size; ++i) {
            min_x = min_x > verts[i].x ? verts[i].x : min_x;
            max_x = max_x < verts[i].x ? verts[i].x : max_x;
            min_y = min_y > verts[i].y ? verts[i].y : min_y;
            max_y = max_y < verts[i].y ? verts[i].y : max_y;
        }
        //сразу же, если точка не попадает в этот прямоугольник, уж точно не попадает в наш многуогольник
        if(point.x < min_x || point.y < min_y || point.y > max_y || point.x > max_x)
            return false;
        int j = 0;
        //теперь бежим по сторонам многоугольника, i - начало, j - конец
        //воспользуемся подобием
        for(int i = 0; i < size; ++i){
            j = (i + size - 1) % size;
            if ((verts[i].y < point.y) != (verts[j].y < point.y)){
                if((point.y - verts[i].y) / (verts[j].y - verts[i].y) * (verts[j].x - verts[i].x) + verts[i].x > point.x)
                    flag = 1 - flag;
            }
        }
        return flag;
    }

    //проверка на подобие многоугольников
    bool isSimilarTo(const Shape &another) const override{
        const Polygon &poly = dynamic_cast<const Polygon&>(another);
        int size = verts.size();
        if(poly.verticesCount() != size)
            return false;
        else {
            //массивы углов
            std::vector<double> an_angles(size, 0);
            std::vector<double> angles(size, 0);
            get_angles(angles);
            poly.get_angles(an_angles);
            return is_cyclic_shift(angles, an_angles);
        }
    }

    //равенство в геометрическом смысле фигур
    bool isCongruentTo(const Shape& another) const override{
        if (isSimilarTo(another)) {
            const Polygon &poly = dynamic_cast<const Polygon&>(another);
            if (equal_double(perimeter(), poly.perimeter()))
                return true;
        }
        return false;
    }

    //проверка на то, что векторы состоят из одинаковых значений с точностью до сдвига или поворота
    bool is_cyclic_shift(const std::vector<double> &first, const std::vector<double> &second) const {
        if (first.size() != second.size())
            return false;

        int size = first.size();
        int start = -1;

        for (int i = 0; i < size; ++i){
            if (equal_double(second[i], first[0])) {
                start = i;
                //если нашли одинаковые элементы
                bool flag1 = true, flag2 = true;
                //начинаем бежать в 2 стороны
                for (int j = 0; j < size; ++j) {
                    if (!equal_double(first[j], second[(j + start) % size]))
                        flag1 = false;
                    if (!equal_double(first[j], second[(size - j + start) % size]))
                        flag2 = false;
                }
                if (flag1 || flag2) {
                    return true;
                }
            }
        }
        //если не нашли совпадение
        return false;
    }

    //заполнение массива углов многоугольника
    void get_angles(std::vector<double> &angles) const {
        int size = verts.size();
        //считаем квадрат косинуса угла
        for (int i = 0; i < size; ++i) {
            double skal = 0;
            skal = (verts[(i + 2) % size].x - verts[(i + 1) % size].x) * (verts[(i + 1) % size].x - verts[i].x) +
                   (verts[(i + 2) % size].y - verts[(i + 1) % size].y) * (verts[(i + 1) % size].y - verts[i].y);
            skal *= skal;
            double a =  (verts[(i + 1) % verts.size()].x - verts[i].x) *
                        (verts[(i + 1) % verts.size()].x - verts[i].x) +
                        (verts[(i + 1) % verts.size()].y - verts[i].y) *
                        (verts[(i + 1) % verts.size()].y - verts[i].y);
            double b = (verts[(i + 2) % verts.size()].x - verts[(i + 1) % verts.size()].x) *
                       (verts[(i + 2) % verts.size()].x - verts[(i + 1) % verts.size()].x) +
                       (verts[(i + 2) % verts.size()].y - verts[(i + 1) % verts.size()].y) *
                       (verts[(i + 2) % verts.size()].y - verts[(i + 1) % verts.size()].y);
            skal /= a;
            skal /= b;
            angles[(i + 1) % verts.size()] = skal;
        }
    }

    //проверка на то, что это одинаковые фигуры
    bool operator == (const Shape &another) const override{
        const Polygon *poly = dynamic_cast<const Polygon*>(&another);
        if(poly) {
            if (verticesCount() != poly->verticesCount())
                return false;
            else {
                std::vector<double> x_coords, y_coords, an_x_coords, an_y_coords;
                const std::vector<Point> &an_verts = poly->getVertices();
                for (int i = 0; i < verticesCount(); ++i) {
                    x_coords.push_back(verts[i].x);
                    y_coords.push_back(verts[i].y);
                    an_x_coords.push_back(an_verts[i].x);
                    an_y_coords.push_back(an_verts[i].y);
                }
                return is_cyclic_shift(x_coords, an_x_coords) && is_cyclic_shift(y_coords, an_y_coords);
            }
        } else
            return false;
    }

    bool operator != (const Shape &another) const {
        const Polygon *poly = dynamic_cast<const Polygon*>(&another);

        if (poly) {
            if (verticesCount() != poly->verticesCount())
                return true;
            else {
                std::vector<double> x_coords, y_coords, an_x_coords, an_y_coords;
                const std::vector<Point> &an_verts = poly->getVertices();
                for (int i = 0; i < verticesCount(); ++i) {
                    x_coords.push_back(verts[i].x);
                    y_coords.push_back(verts[i].y);
                    an_x_coords.push_back(an_verts[i].x);
                    an_y_coords.push_back(an_verts[i].y);
                }
                return !(is_cyclic_shift(x_coords, an_x_coords) && is_cyclic_shift(y_coords, an_y_coords));
            }
        }
        else
            return true;
    }

    //поворот
    void rotate(Point center, double angle) override{
        Point origin(-center.x, -center.y);
        //сдвигаем начало координат в center
        shift_transformation(verts, origin);
        //с помощью матрицы поворота, поворачиваем
        rot_transformation(verts, angle);
        //возвращаем к начальным координатам
        shift_transformation(verts, center);
    }

    void shift_transformation(std::vector<Point> &_verts, Point shift) {
        for(size_t i = 0; i < _verts.size(); ++i) {
            _verts[i].x += shift.x;
            _verts[i].y += shift.y;
        }
    }

    void rot_transformation(std::vector<Point> &_verts, double angle){
        double pi = 3.141592653589793;
        double x, y, rad = angle * pi / 180;
        for(size_t i = 0; i < _verts.size(); ++i) {
            x = _verts[i].x, y = _verts[i].y;
            _verts[i].x = x * cos(rad) - y * sin(rad);
            _verts[i].y = x * sin(rad) + y * cos(rad);
        }
    }

    //отражение относительно точки
    void reflex(Point center) override{
        double x, y;
        for(int i = 0; i < verticesCount(); ++i){
            x = verts[i].x, y = verts[i].y;
            verts[i].x = x + 2.0 * (center.x - x);
            verts[i].y = y + 2.0 * (center.y - y);
        }
    }

    //отражение относительно прямой
    void reflex(Line axis) override {
        Vector n = axis.normal_to_line();
        double n_len_2 = n.len() * n.len();
        double res;

        for(int i = 0; i < verticesCount(); ++i){
            Vector p(verts[i].x, verts[i].y);
            res = p.skal_mult(n);
            res /= n_len_2;
            res = 1 - res;
            res *= 2;
            verts[i].x += (n.x * res);
            verts[i].y += (n.y * res);
        }
    }

    //растяжение
    void scale(Point center, double coefficient) override{
        Point origin(-center.x, -center.y);
        //сдвигаем начало координат в center
        shift_transformation(verts, origin);
        for(size_t i = 0; i < verts.size(); ++i) {
            verts[i].x *= coefficient;
            verts[i].y *= coefficient;
        }
        shift_transformation(verts, center);
    }
};
///////////////Ellipse/////////////////////
class Ellipse : public Shape{
protected:
    Point F1, F2;
    double a, c;
public:
    //конструктор от 2 фокусов и r1+r2
    Ellipse(const Point& f1, const Point& f2, double sum_dist){
        //sum_dist r2 + r1 = 2a, где a - большая полуось
        F1 = f1, F2 = f2;
        a = sum_dist / 2;
        c = sqrt((F1.x - F2.x) * (F1.x - F2.x) + (F1.y - F2.y) * (F1.y - F2.y)) / 2;
    }

    std::pair<Point, Point> focuses() const {
        return std::make_pair(F1, F2);
    }

    std::pair<Line, Line> directrices() const {
        //расстояние от центра до директрис
        assert(!equal_double(c, 0.0));
        double dist = a * a / c;
        //направляющий нормированный вектор фокальной плоскости
        Vector t(F1, F2);
        t.x *= dist / (2 * c), t.y *= dist / (2 * c);
        //направляющий вектор директрис
        Vector n(t.y, -t.x);
        //осталось найти какую-то точку
        Point O = center();
        Line L1(Point(O.x + n.x, O.y + n.y), n);
        Line L2(Point(O.x - n.x, O.y - n.y), n);

        return std::make_pair(L1, L2);
    }

    //эксцентриситет
    double eccentricity() const {
        return c / a;
    }

    //центр
    Point center() const {
        return Point((F1.x + F2.x) / 2, (F1.y + F2.y) / 2);
    }

    //показать a и c
    std::pair<double, double> get_pair() const{
        return std::make_pair(a, c);
    }

    //отражение относительно точки
    void reflex(Point center) override {
        double x_1 = F1.x, x_2 = F2.x, y_1 = F1.y, y_2 = F2.y;
        F1.x = x_1 + 2.0 * (center.x - x_1), F2.x = x_2 + 2.0 * (center.x - x_2);
        F1.y = y_1 + 2.0 * (center.x - y_1), F2.y = y_2 + 2.0 * (center.x - y_2);
    }

    //поворот
    void rotate(Point center, double angle) override{
        Point origin(-center.x, -center.y);
        //сдвигаем начало координат в center
        shift_transformation(origin);
        //с помощью матрицы поворота, поворачиваем
        rot_transformation(angle);
        //возвращаем к начальным координатам
        shift_transformation(center);
    }

    void shift_transformation(const Point& shift){
        F1.x += shift.x, F2.x += shift.x;
        F1.y += shift.y, F2.y += shift.y;
    }

    void rot_transformation(double angle){
        double pi = 3.141592653589793, rad = angle * pi / 180;
        double x1 = F1.x, y1 = F1.y, x2 = F2.x, y2 = F2.y;

        F1.x = x1 * cos(rad) - y1 * sin(rad), F2.x = x2 * cos(rad) - y2 * sin(rad);
        F1.y = x1 * sin(rad) + y1 * cos(rad), F2.y = x2 * sin(rad) + y2 * cos(rad);
    }

    //оторажение относительно прямой
    void reflex(Line axis) override {
        Vector n = axis.normal_to_line();

        double n_len_2 = n.len() * n.len();
        double res1, res2;

        Vector p1(F1.x, F1.y), p2(F2.x, F2.y);
        res1 = p1.skal_mult(n), res2 = p2.skal_mult(n);
        res1 /= n_len_2, res2 /= n_len_2;
        res1 = 1 - res1, res2 = 1 - res2;
        res1 *= 2, res2 *= 2;
        F1.x += (n.x * res1), F1.y += (n.y * res1);
        F2.x += (n.x * res2), F2.y += (n.y * res2);
    }

    //растяжение
    void scale(Point center, double coefficient) override{
        //остается постоянным при растяжении
        double e = eccentricity();
        Point origin(-center.x, -center.y);
        //сдвигаем начало координат в center
        shift_transformation(origin);
        F1.x *= coefficient, F1.y *= coefficient;
        F2.x *= coefficient, F2.y *= coefficient;

        shift_transformation(center);
        //пересчитываем коэффициенты
        if(!equal_double(e, 0)) {
            c = sqrt((F1.x - F2.x) * (F1.x - F2.x) + (F1.y - F2.y) * (F1.y - F2.y)) / 2;
            a = c / e;
        }
        else{
            a *= coefficient;
        }
    }

    //периметр
    double perimeter() const override {
        double b = sqrt(a * a - c * c), pi = 3.141592653589793;
        return pi * ( 3 * (a + b) - sqrt((3 * a + b) * (a + 3 * b)));
    }

    //площадь
    double area() const override {
        double b = sqrt(a * a - c * c), pi = 3.141592653589793;;
        return pi * a * b;
    }

    bool operator == (const Shape& another) const override{
        const Ellipse *elly = dynamic_cast<const Ellipse*>(&another);
        if(elly) {
            std::pair<Point, Point> focuses = elly->focuses();
            bool flag = (focuses.first == F1 && focuses.second == F2) || (focuses.first == F2 && focuses.second == F1);
            return (equal_double(eccentricity(), elly->eccentricity()) && flag);
        } else
            return false;
    }

    bool isCongruentTo(const Shape& another) const override{
        if(isSimilarTo(another)){
            const Ellipse &elly = dynamic_cast<const Ellipse&>(another);
            if (equal_double(elly.get_pair().first, a) && equal_double(elly.get_pair().second, c))
                return true;
        }
        return false;
    }

    bool isSimilarTo(const Shape& another) const override{
        const Ellipse *elly = dynamic_cast<const Ellipse*>(&another);
        if(!elly)
            return false;
        std::pair<double, double> get = elly->get_pair();
        if (equal_double(c, 0))
            return equal_double(elly->focuses().first.x, elly->focuses().second.x) && equal_double(elly->focuses().first.y, elly->focuses().second.y);
        else
            return (equal_double(get.first / a, get.second / c));
    }

    bool containsPoint(Point point) const override{
        //проверка на то, что сумма расстояний до фокусов меньше 2а
        return (sqrt((point.x - F1.x) * (point.x - F1.x) + (point.y - F1.y) * (point.y - F1.y)) +
                sqrt((point.x - F2.x) * (point.x - F2.x) + (point.y - F2.y) * (point.y - F2.y)) < 2 * a);
    }
};

class Circle : public Ellipse{
public:
    Circle(const Point& center, double radius) : Ellipse(center, center, 2 * radius) {}

    double radius() const {
        return a;
    }
};

class Rectangle : public Polygon{
public:
    //конструктор от вектора точек
    explicit Rectangle(const std::vector<Point> &input) : Polygon(input){}

    //конструктор от 2 точек и отношения сторон
    Rectangle(const Point& a, const Point& b, double k){
        //коэффициент будет больше 1
        k = k < 1 ? 1 / k : k;
        double angle = atan(k);
        double gipotenuse_2 = (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
        double small_katet = cos(angle) * sqrt(gipotenuse_2);
        double height = small_katet * sin(angle);
        double small_height = small_katet * cos(angle);

        Vector gip(a, b);
        gip.normaling(1.0);

        Vector h(-gip.y, gip.x);
        h.normaling(height);
        gip.normaling(small_height);

        Point c(a.x + gip.x + h.x, a.y + gip.y + h.y);
        Point d(b.x - gip.x - h.x, b.y - gip.y - h.y);

        add_vert(a), add_vert(c), add_vert(b), add_vert(d);
    }

    //центр прямоугольника
    Point center() const {
        assert(verticesCount() == 4);
        return Point((verts[0].x + verts[2].x) / 2, (verts[0].y + verts[2].y) / 2);
    }

    //диагонали
    std::pair<Line, Line> diagonals() const {
        assert(verticesCount() == 4);
        return std::make_pair(Line(verts[0], verts[2]), Line(verts[1], verts[3]));
    }
};

class Square : public Rectangle{
public:
    //конструктор от вектора точек
    explicit Square(const std::vector<Point> &input) : Rectangle(input){}

    Square(Point a, Point b) : Rectangle(a, b, 1.0){}

    //описанная окружность
    Circle circumscribedCircle() const {
        assert(verts.size() == 4);

        Point cent = Rectangle::center();
        double diagonal = sqrt((cent.x - verts[0].x) * (cent.x - verts[0].x) + (cent.y - verts[0].y) * (cent.y - verts[0].y)) / 2;
        return Circle(cent, diagonal);
    }

    //вписанная лкоужность
    Circle inscribedCircle() const {
        assert(verts.size() == 4);

        Point cent = Rectangle::center();
        double rad = sqrt((verts[1].x - verts[0].x) * (verts[1].x - verts[0].x) + (verts[1].y - verts[0].y) * (verts[1].y - verts[0].y)) / 2;
        return Circle(cent, rad);
    }
};

/////////////////////////////////////Triangle///////////////////////////////////
class Triangle : public Polygon{
public:
    //конструктор от вектора точек
    explicit Triangle(const std::vector<Point> &input) : Polygon(input){}

    template<typename Head, typename... Tail>
    explicit Triangle(const Head& head, const Tail&... tail) : Polygon(head, tail...){}

    //центр масс
    Point centroid() const {
        assert(verticesCount() == 3);
        return Point((verts[0].x + verts[1].x + verts[2].x) / 3, (verts[0].y + verts[1].y + verts[2].y) / 3);
    }

    //вписанная окружность
    Circle inscribedCircle() const {
        //сначала найдем точку пересечения биссектрис
        //а совсем для начала найдем длины сторон
        assert(verticesCount() == 3);
        double a = sqrt((verts[0].x - verts[1].x) * (verts[0].x - verts[1].x) + (verts[0].y - verts[1].y) * (verts[0].y - verts[1].y));
        double b = sqrt((verts[1].x - verts[2].x) * (verts[1].x - verts[2].x) + (verts[1].y - verts[2].y) * (verts[1].y - verts[2].y));
        double c = sqrt((verts[0].x - verts[2].x) * (verts[0].x - verts[2].x) + (verts[0].y - verts[2].y) * (verts[0].y - verts[2].y));
        Point O((a * verts[2].x + b * verts[0].x + c * verts[1].x) / (a + b + c), (a * verts[2].y + b * verts[0].y + c * verts[1].y) / (a + b + c));

        //центр окружности найден, остался радиус
        double r = area() / (perimeter() / 2);

        return Circle(O, r);
    }

    //описанная окружность
    Circle circumscribedCircle() const {
        //найдем точку пересечения серединных перепендикуляров
        double x1 = verts[0].x, x2 = verts[1].x, x3 = verts[2].x, y1 = verts[0].y, y2 = verts[1].y, y3 = verts[2].y;

        double bot = 2 * (x1 * (y2 - y3) - y1 * (x2 - x3) + (x2 * y3 - x3 * y2));
        double top_x = (x1 * x1 + y1 * y1) * (y2 - y3) - y1 * (x2 * x2 + y2 * y2 - x3 * x3 - y3 * y3) +
                       (x2 * x2 + y2 * y2) * y3 - (x3 * x3 + y3 * y3) * y2;
        double top_y = x1 * (x2 * x2 + y2 * y2 - x3 * x3 - y3 * y3) - (x1 * x1 + y1 * y1) * (x2 - x3) +
                       x2 * (x3 * x3 + y3 * y3) - x3 * (x2 * x2 + y2 * y2);

        double x0 = top_x / bot, y0 = top_y / bot;

        double radius = sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));

        return Circle(Point(x0, y0), radius);
    }

    Point orthocenter() const {
        double x1 = verts[0].x, x2 = verts[1].x, x3 = verts[2].x, y1 = verts[0].y, y2 = verts[1].y, y3 = verts[2].y;
        double bot = (x1 * (y2 - y3) - y1 * (x2 - x3) + (x2 * y3 - x3 * y2));

        double top_x = y1 * (x3 * x1 + y2 * y2 - x1 * x2 - y3 * y3) - (x2 * x3 + y1 * y1) * (y2 - y3) +
                       y2 * (x1 * x2 + y3 * y3) - y3 * (x1 * x3 + y2 * y2);
        double top_y = (x1 * x1 + y2 * y3) * (x2 - x3) - x1 * (x2 * x2 + y3 * y1 - x3 * x3 - y1 * y2) +
                       x3 * (x2 * x2 + y3 * y1) - x2 * (x3 * x3 + y1 * y2);

        return Point(top_x / bot, top_y / bot);
    }

    Line EulerLine() const {
        Point A = centroid();
        Point B = orthocenter();

        return Line(A, B);
    }

    Circle ninePointsCircle() const {
        Point H = orthocenter();
        Point A((H.x + verts[0].x) / 2,  (H.y + verts[0].y) / 2);
        Point B((H.x + verts[1].x) / 2,  (H.y + verts[1].y) / 2);
        Point C((H.x + verts[2].x) / 2,  (H.y + verts[2].y) / 2);

        Triangle T(A, B, C);
        return T.circumscribedCircle();
    }
};