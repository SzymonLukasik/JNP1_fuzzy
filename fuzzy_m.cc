#include <iostream>
using namespace std;

using real_t = double;

class TriFuzzyNum {
private:
    real_t l;
    real_t m;
    real_t u;
    //Ranga:
    real_t first;
    real_t second;
    real_t third;
public:
    TriFuzzyNum(real_t a, real_t b, real_t c);
    real_t lower_value() {return l;}
    real_t modal_value() {return m;}
    real_t upper_value() {return u;}
    TriFuzzyNum operator += (const TriFuzzyNum&) const;
    std::weak_ordering operator<=>(const TriFuzzyNum& right_operand) const {
        if (auto cmp = this->first <=> right_operand.first; cmp != 0)
            return cmp;
        if (auto cmp = this->second <=> right_operand.second; cmp != 0)
            return cmp;
        auto cmp = this->third <=> right_operand.third;
        return cmp;
    }
};

TriFuzzyNum::TriFuzzyNum(real_t a, real_t b, real_t c) const {
    if (a > b) {swap (a, b);}
    if (b > c) {swap (b, c);}
    if (a > b) {swap (a, b);}
    l = a;
    m = b;
    u = c;
    real_t z = (u - l) + sqrt(1 + (u - m) * (u - m)) + sqrt(1 + (m - l) * (m - l));
    real_t x = ((u - l) * m + sqrt(1 + (u - m) * (u - m)) * l + sqrt(1 + (m - l) * (m - l)) * u) / z;
    real_t y = (u - l) / z;
    first = x - y / 2;
    second = 1 - y;
    third = m;
}

TriFuzzyNum TriFuzzyNum::operator+= (const TriFuzzyNum& right_operand) const{
    this->l += right_operand.l;
    this->m += right_operand.m;
    this->u += right_operand.u;
    return *this;
}



int main () {
    cout << "x";
    return 0;
}