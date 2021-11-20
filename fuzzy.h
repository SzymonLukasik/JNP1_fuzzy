#ifndef __FUZZY_H
#define __FUZZY_H

#include<ostream>
#include<compare>
#include<cmath>
#include<set>

using real_t = double;

class TriFuzzyNum {
	public:
        constexpr TriFuzzyNum(real_t a, real_t b, real_t c) {
            if (a > b) {std::swap (a, b);}
            if (b > c) {std::swap (b, c);}
            if (a > b) {std::swap (a, b);}
            l = a;
            m = b;
            u = c;

            // We want to get these via functions with same signatures as lower_value
            real_t z = (u - l) + sqrt(1 + (u - m) * (u - m)) + sqrt(1 + (m - l) * (m - l));
            real_t x = ((u - l) * m + sqrt(1 + (u - m) * (u - m)) * l + sqrt(1 + (m - l) * (m - l)) * u) / z;
            real_t y = (u - l) / z;
            first = x - y / 2;
            second = 1 - y;
            third = m;
        }

        constexpr real_t lower_value() const {
            return l;
        }

        constexpr real_t modal_value() const {
            return m;
        }

        constexpr real_t upper_value() const {
            return u;
        }
                
        constexpr std::partial_ordering operator<=>(const TriFuzzyNum& other) const {
            if (auto cmp = this->first <=> other.first; cmp != 0)
                return cmp;
            if (auto cmp = this->second <=> other.second; cmp != 0)
                return cmp;
            auto cmp = this->third <=> other.third;
            return cmp;
        }
        
        constexpr bool operator== (const TriFuzzyNum& other) const = default;
        		
        constexpr TriFuzzyNum& operator+= (const TriFuzzyNum& other) {
            this->l += other.l;
            this->m += other.m;
            this->u += other.u;
            return *this;
        }

        constexpr TriFuzzyNum& operator-= (const TriFuzzyNum& other) {
            this->l -= other.u;
            this->m -= other.m;
            this->u -= other.l;
            return *this;
        }

        constexpr TriFuzzyNum& operator*= (const TriFuzzyNum& other) {
            this->l *= other.l;
            this->m *= other.m;
            this->u *= other.u;
            return *this;
        }

        constexpr const TriFuzzyNum operator+ (const TriFuzzyNum& other) const {
            return TriFuzzyNum(*this) += other;
        }

        constexpr const TriFuzzyNum operator- (const TriFuzzyNum& other) const {
            TriFuzzyNum result = *this;
            result -= other;
            return result;
        }

        constexpr const TriFuzzyNum operator* (const TriFuzzyNum& other) const {
            TriFuzzyNum result = *this;
            result *= other;
            return result;
        }

        friend std::ostream& operator<< (std::ostream& os, const TriFuzzyNum& num);
    
    private:
		real_t l;
		real_t m;
		real_t u;
		real_t first;
		real_t second;
		real_t third;
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