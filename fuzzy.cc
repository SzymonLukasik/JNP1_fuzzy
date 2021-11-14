#include<utility>
#include<set>
#include<compare>
#include<cmath>
#include<cassert>

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

int main() {
	TriFuzzyNum num1(1, 2, 3);
	TriFuzzyNum num2(0.5, 0.25, 0.75);
	//constexpr TriFuzzyNum num3(1, 1, 1);

	// cout << num1 << endl;  // (1, 2, 3)
	// cout << num2 << endl;  // (0.25, 0.5, 0.75)
	// cout << num3 << endl;  // (1, 1, 1)

	//assert(num1 + num2 == TriFuzzyNum(1.25, 2.5, 3.75));
	//assert(num1 - num2 == TriFuzzyNum(0.25, 1.5, 2.75));
	//assert(num1 * num2 == TriFuzzyNum(0.25, 1, 2.25));

	assert(num1 > num2);
	assert(num1 >= num2);
	// assert(num1 != num2);
	assert(num2 < num1);
	assert(num2 <= num1);
	//assert(num3 == crisp_number(1));
	// static_assert(num3 == crisp_number(1));

	assert(num1 <=> num2 > 0);
	assert(num2 <=> num1 < 0);

	//assert((num1 += num3) == TriFuzzyNum(2, 3, 4));
	//assert((num1 -= num3) == TriFuzzyNum(1, 2, 3));
	//assert((num1 *= num3) == num1);
	//assert((num1 += crisp_zero) == num1);

	//static_assert(num3.lower_value() == 1);
	//static_assert(num3.modal_value() == 1);
	//static_assert(num3.upper_value() == 1);

	TriFuzzyNumSet fn_set1({TriFuzzyNum(1, 2, 4), TriFuzzyNum(1, 3, 6)});
	//assert(fn_set1.arithmetic_mean() == TriFuzzyNum(1, 2.5, 5));

	fn_set1.insert(TriFuzzyNum(1, 4, 5));
	//assert(fn_set1.arithmetic_mean() == TriFuzzyNum(1, 3, 5));

	fn_set1.remove(TriFuzzyNum(1, 3, 6));
	//assert(fn_set1.arithmetic_mean() == TriFuzzyNum(1, 3, 4.5));

	TriFuzzyNumSet fn_set2({TriFuzzyNum(1, 2, 3),
							TriFuzzyNum(1, 2, 3),
							TriFuzzyNum(1, 5, 6)});
	//assert(fn_set2.arithmetic_mean() == TriFuzzyNum(1, 3, 4));

	//constinit static TriFuzzyNum num4(1.25, 2.25, 3.25);
	//num4 += crisp_number(0.25);
	//assert(num4 == TriFuzzyNum(1.5, 2.5, 3.5));
}