#ifndef __FUZZY_H
#define __FUZZY_H

#include<ostream>
#include<compare>
#include<cmath>
#include<set>

using real_t = double;

class TriFuzzyNum {
	public:
        constexpr real_t lower_value() const { return l; }
        constexpr real_t modal_value() const { return m; }
        constexpr real_t upper_value() const { return u; }

        constexpr TriFuzzyNum(real_t a, real_t b, real_t c) {
            swap3(a, b, c);
            l = a;
            m = b;
            u = c;
        }

        constexpr std::partial_ordering operator<=>(const TriFuzzyNum &other) const {
            if (auto cmp = this->first() <=> other.first(); cmp != 0)
                return cmp;
            if (auto cmp = this->second() <=> other.second(); cmp != 0)
                return cmp;
            auto cmp = this->third() <=> other.third();
            return cmp;
        }
        
        constexpr bool operator== (const TriFuzzyNum& other) const = default;
        constexpr bool operator!= (const TriFuzzyNum& other) const = default;
        		
        constexpr TriFuzzyNum& operator+=(const TriFuzzyNum& other) {
            this->l = this->l + other.l;
            this->m = this->m + other.m;
            this->u = this->u + other.u;
            return *this;
        }

        constexpr TriFuzzyNum& operator-=(const TriFuzzyNum &other) {
            this->l = this->l - other.u;
            this->m = this->m - other.m;
            this->u = this->u - other.l;
            return *this;
        }

        constexpr TriFuzzyNum& operator*=(const TriFuzzyNum& other) {
            real_t a = this->l * other.l;
            real_t b = this->m * other.m;
            real_t c = this->u * other.u;
            swap3(a, b, c);
            this->l = a;
            this->m = b;
            this->u = c;
            return *this;
        }

        

        constexpr const TriFuzzyNum operator+(const TriFuzzyNum& other) const {
            return (TriFuzzyNum(*this) += other);
        }

        constexpr const TriFuzzyNum operator-(const TriFuzzyNum& other) const {
            return (TriFuzzyNum(*this) -= other);
        }

        constexpr const TriFuzzyNum operator*(const TriFuzzyNum& other) const {
            return (TriFuzzyNum(*this) *= other);
        }

        friend std::ostream& operator<< (std::ostream& os, const TriFuzzyNum& num);
    
    private:
		real_t l;
		real_t m;
		real_t u;

        constexpr real_t z () const { return (u - l) + sqrt(1 + (u - m) * (u - m)) + sqrt(1 + (m - l) * (m - l)); }
        constexpr real_t x () const { return ((u - l) * m + sqrt(1 + (u - m) * (u - m)) * l + sqrt(1 + (m - l) * (m - l)) * u) / z(); }
        constexpr real_t y () const { return (u - l) / z();}
        constexpr real_t first () const { return x() - y() / 2; }
        constexpr real_t second () const { return 1 - y(); }
        constexpr real_t third () const { return m; }

        constexpr void  swap3 (real_t& a, real_t& b, real_t& c) {
            if (a > b) { std::swap(a, b); }
            if (b > c) { std::swap(b, c); }
            if (a > b) { std::swap(a, b); }
        }
};

consteval TriFuzzyNum crisp_number(real_t v) {
	return TriFuzzyNum(v, v, v);
}

inline constinit const TriFuzzyNum crisp_zero = crisp_number(0);

class TriFuzzyNumSet {
    public:
        TriFuzzyNumSet();
        TriFuzzyNumSet(std::initializer_list<TriFuzzyNum> args);

        void insert(const TriFuzzyNum& num);
        void insert(TriFuzzyNum&& num);
        void remove(const TriFuzzyNum& num);

        TriFuzzyNum arithmetic_mean() const;

	private:
		std::multiset<TriFuzzyNum> s;
};

#endif