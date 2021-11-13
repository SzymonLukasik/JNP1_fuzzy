#include<utility>
#include<set>
#include<compare>
#include<cmath>

using namespace std;

using real_t = double;

class TriFuzzyNum {
	private:
		real_t l;
		real_t m;
		real_t u;
		real_t first;
    real_t second;
    real_t third;

	public:
		TriFuzzyNum(real_t a, real_t b, real_t c) {
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
		

		// Default copy constructor is fine
		TriFuzzyNum(const TriFuzzyNum& other) : l(other.l), m(other.m), u(other.u) {} 
		
		TriFuzzyNum& operator= (const TriFuzzyNum& other) {
			l = other.l;
			m = other.m;
			u = other.u;
			return *this;
		}

		// Maybe initialization list ?
		TriFuzzyNum (TriFuzzyNum&& other) {
			*this  = other;
		}

		TriFuzzyNum& operator= (TriFuzzyNum&& other) {
			return	*this = other;
		}
	
		real_t lower_value() {
			return l;
		}

		real_t modal_value() {
			return m;
		}

		real_t upper_value() {
			return u;
		}

	auto operator<=>(const TriFuzzyNum& right_operand) const {
			if (auto cmp = this->first <=> right_operand.first; cmp != 0)
					return cmp;
			if (auto cmp = this->second <=> right_operand.second; cmp != 0)
					return cmp;
			auto cmp = this->third <=> right_operand.third;
			return cmp;
		}

	TriFuzzyNum& operator+= (const TriFuzzyNum& right_operand) {
    this->l += right_operand.l;
    this->m += right_operand.m;
    this->u += right_operand.u;
    return *this;
	}
};


class TriFuzzyNumSet {
	public:
		TriFuzzyNumSet() : s() {}

		TriFuzzyNumSet(initializer_list<TriFuzzyNum> args) {
			s = set<TriFuzzyNum>(args);
		}

		// Default copy constructor also fine?
		// TriFuzzyNumSet(const TriFuzzyNumSet& other): s(other.s) {}

		TriFuzzyNumSet& operator= (const TriFuzzyNumSet& other) {
			s.clear(); // Maybe not needed
			s = other.s;
			return *this;
		}

		// Maybe initialization list ?
		TriFuzzyNumSet (TriFuzzyNumSet&& other) {
			*this = other;
		}

		TriFuzzyNumSet& operator= (TriFuzzyNumSet&& other) {
			s.clear(); // Maybe not needed
			s = move(other.s);
			return *this;
		}

		void insert(const TriFuzzyNum& num) {
			s.insert(num);
		}

		void insert(TriFuzzyNum&& num) {
			s.insert(move(num));
		}

		void remove(const TriFuzzyNum& num) {
			s.erase(num);
		}

		void remove(TriFuzzyNum&& num) {
			s.erase(num);
		}

		TriFuzzyNum arithmetic_mean() {
			TriFuzzyNum sum = TriFuzzyNum(0., 0., 0);
			for (const TriFuzzyNum& num : s)
				sum += num;
			size_t n = s.size();

			real_t l = sum.lower_value() / n;
			real_t m = sum.modal_value() / n;
			real_t r = sum.upper_value() / n;
			return TriFuzzyNum(l, m, r);
		}

	private:
		set<TriFuzzyNum> s;
};

TriFuzzyNum crisp_number(real_t v) {
	return TriFuzzyNum(v, v, v);
}

TriFuzzyNum crisp_zero() {
	return TriFuzzyNum(0, 0, 0);
}