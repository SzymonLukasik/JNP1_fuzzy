#include"fuzzy.h"

using namespace std;

ostream &operator<<(ostream &os, const TriFuzzyNum &num) {
    os << "(" << num.l << ", " << num.m << ", " << num.u << ")";
    return os;
}

TriFuzzyNumSet::TriFuzzyNumSet() : s() {}

TriFuzzyNumSet::TriFuzzyNumSet(initializer_list<TriFuzzyNum> args) : s(args) {}

void TriFuzzyNumSet::insert(const TriFuzzyNum &num) { s.insert(num); }

void TriFuzzyNumSet::insert(TriFuzzyNum &&num) { s.insert(move(num)); }

void TriFuzzyNumSet::remove(const TriFuzzyNum &num) { s.erase(num); }

TriFuzzyNum TriFuzzyNumSet::arithmetic_mean() const {
    if (s.empty()) {
        throw length_error("TriFuzzyNumSet::arithmetic_mean"
                           " - the set is empty.");
	} else {
		TriFuzzyNum sum = TriFuzzyNum(0., 0., 0);
		for (const TriFuzzyNum &num : s)
			sum += num;
		size_t n = s.size();
		real_t l = sum.lower_value() / n;
		real_t m = sum.modal_value() / n;
		real_t r = sum.upper_value() / n;
		return TriFuzzyNum(l, m, r);
	}
}