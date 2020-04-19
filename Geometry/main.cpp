#include "main.h"

int main() {
    Point a(8, 11), b(15, 19), c(19, 15);
    Triangle T(a, b, c);
    std::cout << T.ninePointsCircle().radius() << std::endl;
    std::cout << T.ninePointsCircle().center().x << std::endl;
    std::cout << T.ninePointsCircle().center().y << std::endl;
}
