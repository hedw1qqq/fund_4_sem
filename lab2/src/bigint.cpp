#include "../include/bigint.hpp"

bool BigInt::validateString(const std::string& str) {
	if (str.empty()) {
		return false;
	}
	size_t start_pos = 0;
	if (str[0] == '-' || str[0] == '+') {
		if (str.size() == 1) {
			return false;
		}
		start_pos = 1;
	}

	for (size_t i = start_pos; i < str.size(); ++i) {
		if (!isdigit(str[i])) {
			return false;
		}
	}
	return true;
}
bool BigInt::isNull() const {
	if (digits.size() == 1 && digits[0] == 0) {
		return true;
	}
	return false;
}

void BigInt::removeLeadingZeros() {
	while (digits.size() > 1 && digits.back() == 0) {
		digits.pop_back();
	}
	if (isNull()) {
		isNegative = false;
	}
}

void BigInt::subtractValue(const BigInt& smaller) {
	long long borrow = 0;
	size_t n = digits.size();
	size_t m = smaller.digits.size();

	for (size_t i = 0; i < n; ++i) {
		long long current_this_digit = digits[i];
		long long current_smaller_digit = (i < m) ? smaller.digits[i] : 0;
		long long diff = current_this_digit - current_smaller_digit - borrow;

		if (diff < 0) {
			diff += BASE;
			borrow = 1;
		} else {
			borrow = 0;
		}
		digits[i] = static_cast<unsigned long long>(diff);
	}
	removeLeadingZeros();
}
void BigInt::addValue(const BigInt& other) {
	unsigned long long carry = 0;
	size_t n = digits.size();
	size_t m = other.digits.size();
	size_t max_len = std::max(n, m);
	digits.resize(max_len, 0);

	for (size_t i = 0; i < max_len || carry != 0; ++i) {
		if (i == digits.size()) {
			digits.push_back(0);
		}
		unsigned long long other_digit = (i < m) ? other.digits[i] : 0;
		unsigned long long currentSum = digits[i] + other_digit + carry;
		digits[i] = currentSum % BASE;
		carry = currentSum / BASE;
	}
}

std::strong_ordering BigInt::compareValue(const BigInt& other) const {
	size_t n = digits.size();
	size_t m = other.digits.size();

	if (n > m) {
		return std::strong_ordering::greater;
	}
	if (n < m) {
		return std::strong_ordering::less;
	}

	for (int i = n - 1; i >= 0; --i) {
		if (digits[i] > other.digits[i]) {
			return std::strong_ordering::greater;
		}
		if (digits[i] < other.digits[i]) {
			return std::strong_ordering::less;
		}
	}
	return std::strong_ordering::equal;
}

BigInt::BigInt() : isNegative(false) { digits.push_back(0); }

BigInt::BigInt(long long value) : isNegative(false) {
	if (value == 0) {
		digits.push_back(0);
	} else {
		if (value < 0) {
			isNegative = true;
			value = -value;
		}
	}
	while (value > 0) {
		digits.push_back(static_cast<unsigned long long>(value % BASE));
		value /= BASE;
	}
}

BigInt::BigInt(const std::string& str) : isNegative(false) {
	if (!validateString(str)) {
		throw std::invalid_argument("Invalid string format for BigInt construction");
	}

	size_t start_pos = 0;
	if (str[0] == '-') {
		isNegative = true;
		start_pos = 1;
	} else if (str[0] == '+') {
		isNegative = false;
		start_pos = 1;
	} else {
		isNegative = false;
	}

	size_t first_digit_pos = start_pos;
	while (first_digit_pos < str.length() && str[first_digit_pos] == '0') {
		first_digit_pos++;
	}

	if (first_digit_pos == str.length()) {
		digits.push_back(0);
		isNegative = false;
		return;
	}

	digits.clear();

	for (long long i = static_cast<long long>(str.length() - 1); i >= static_cast<long long>(first_digit_pos);
	     i -= BASE_DIGITS) {
		int block_start = std::max(static_cast<long long>(first_digit_pos), i - BASE_DIGITS + 1);
		std::string current_part_str = str.substr(block_start, i - block_start + 1);
		try {
			digits.push_back(std::stoull(current_part_str));
		} catch (const std::out_of_range& oor) {
			throw std::out_of_range("String segment is out of range for ull: " + current_part_str);
		}
	}

	removeLeadingZeros();
}
BigInt::BigInt(const BigInt& other) : digits(other.digits), isNegative(other.isNegative) {};

BigInt::BigInt(BigInt&& other) noexcept : digits(std::move(other.digits)), isNegative(other.isNegative) {
	other.isNegative = false;
	other.digits = {0};
}

BigInt& BigInt::operator=(const BigInt& other) {
	if (*this != other) {
		isNegative = other.isNegative;
		digits = other.digits;
	}
	return *this;
}

BigInt& BigInt::operator=(BigInt&& other) noexcept {
	if (*this != other) {
		isNegative = other.isNegative;
		digits = std::move(other.digits);
		other.isNegative = false;
		other.digits = {0};
	}
	return *this;
}

BigInt& BigInt::operator+=(const BigInt& other) {
	if (isNegative == other.isNegative) {
		addValue(other);
	} else {
		std::strong_ordering value_comparison = compareValue(other);

		if (value_comparison == std::strong_ordering::equal) {
			digits = {0};
			isNegative = false;
		} else if (value_comparison == std::strong_ordering::greater) {
			subtractValue(other);
		} else {
			BigInt temp = other;
			temp.subtractValue(*this);
			digits = std::move(temp.digits);
			isNegative = temp.isNegative;
		}
	}
	removeLeadingZeros();
	return *this;
}

BigInt& BigInt::operator-=(const BigInt& other) {
	BigInt temp = other;
	if (!(temp.isNull())) {
		temp.isNegative = !temp.isNegative;
	}
	return *this += temp;
}

BigInt BigInt::operator+(const BigInt& other) const { return BigInt(*this) += other; }

BigInt BigInt::operator-(const BigInt& other) const { return BigInt(*this) -= other; }

bool BigInt::operator==(const BigInt& other) const {
	if (isNegative != other.isNegative) {
		return false;
	}
	return compareValue(other) == 0;
}

bool BigInt::operator!=(const BigInt& other) const { return !(*this == other); }

BigInt& BigInt::operator++() { return *this += BigInt(1); }

BigInt BigInt::operator++(int) {
	BigInt temp = *this;
	*this += BigInt(1);
	return temp;
}

BigInt& BigInt::operator--() { return *this -= BigInt(1); }

BigInt BigInt::operator--(int) {
	BigInt temp = *this;
	*this -= BigInt(1);
	return temp;
}

BigInt& BigInt::operator*=(const BigInt& other) {
	bool this_is_zero = isNull();
	bool other_is_zero = other.isNull();
	if (this_is_zero || other_is_zero) {
		*this = BigInt(0);
		return *this;
	}

	bool result_is_negative = (isNegative != other.isNegative);

	size_t n = digits.size();
	size_t m = other.digits.size();
	std::vector<unsigned long long> result_digits(n + m, 0);

	for (size_t i = 0; i < n; ++i) {
		unsigned long long carry = 0;
		for (size_t j = 0; j < m || carry != 0; ++j) {
			unsigned long long current_other_digit = (j < m) ? other.digits[j] : 0;
			unsigned long long current_product = result_digits[i + j] + digits[i] * current_other_digit + carry;
			result_digits[i + j] = current_product % BASE;
			carry = current_product / BASE;
		}
	}

	isNegative = result_is_negative;
	digits = std::move(result_digits);

	removeLeadingZeros();

	return *this;
}

BigInt& BigInt::operator/=(const BigInt& other) {
	if (other.isNull()) {
		throw std::runtime_error("Division by zero");
	}

	bool this_is_zero = isNull();
	if (this_is_zero) {
		return *this;
	}

	std::strong_ordering value_cmp = compareValue(other);

	if (value_cmp == std::strong_ordering::less) {
		*this = BigInt(0);
		return *this;
	}
	if (value_cmp == std::strong_ordering::equal) {
		*this = BigInt(1);
		isNegative = (isNegative != other.isNegative);
		return *this;
	}

	bool result_is_negative = (isNegative != other.isNegative);

	BigInt abs_dividend = *this;
	abs_dividend.isNegative = false;
	BigInt abs_divisor = other;
	abs_divisor.isNegative = false;

	BigInt quotient(0);
	BigInt current_partial_dividend(0);

	for (long long i = static_cast<long long>(abs_dividend.digits.size()) - 1; i >= 0; --i) {
		current_partial_dividend *= BigInt(BASE);
		current_partial_dividend += BigInt(abs_dividend.digits[i]);

		unsigned long long low = 0, high = BigInt::BASE;
		unsigned long long current_quotient_digit = 0;

		while (low < high) {
			unsigned long long mid = low + (high - low) / 2;

			BigInt temp_product = abs_divisor;
			temp_product *= BigInt(mid);

			if (temp_product <= current_partial_dividend) {
				current_quotient_digit = mid;
				low = mid + 1;
			} else {
				high = mid;
			}
		}

		quotient *= BigInt(BASE);
		quotient += BigInt(current_quotient_digit);

		if (current_quotient_digit > 0) {
			BigInt to_subtract = abs_divisor;
			to_subtract *= BigInt(current_quotient_digit);
			current_partial_dividend -= to_subtract;
		}
	}

	digits = std::move(quotient.digits);
	isNegative = result_is_negative;

	removeLeadingZeros();

	return *this;
}
BigInt& BigInt::operator%=(const BigInt& other) {
	if (other.isNull()) {
		throw std::runtime_error("Modulo by zero");
	}

	BigInt quotient = *this / other;
	*this -= (quotient * other);

	if (isNull()) {
		isNegative = false;
	}

	return *this;
}

BigInt BigInt::operator%(const BigInt& other) const { return BigInt(*this) %= other; }

BigInt BigInt::operator/(const BigInt& other) const { return BigInt(*this) /= other; }
BigInt BigInt::operator*(const BigInt& other) const { return BigInt(*this) *= other; }

bool BigInt::operator>(const BigInt& other) const { return other < *this; }

bool BigInt::operator<(const BigInt& other) const {
	if (isNegative && !other.isNegative) {
		return true;
	}
	if (!isNegative && other.isNegative) {
		return false;
	}

	std::strong_ordering value_comparison = compareValue(other);

	if (!isNegative) {
		return value_comparison == std::strong_ordering::less;
	} else {
		return value_comparison == std::strong_ordering::greater;
	}
}

bool BigInt::operator<=(const BigInt& other) const { return !(*this > other); }
bool BigInt::operator>=(const BigInt& other) const { return !(*this < other); }

std::ostream& operator<<(std::ostream& os, const BigInt& num) {
	if (num.isNull()) {
		os << '0';
		return os;
	}

	if (num.isNegative) {
		os << '-';
	}
	os << num.digits.back();
	for (long long i = static_cast<long long>(num.digits.size()) - 2; i >= 0; --i) {
		os << std::setw(BigInt::BASE_DIGITS) << std::setfill('0') << num.digits[i];
	}

	return os;
}

std::istream& operator>>(std::istream& is, BigInt& num) {
	std::string input_str;
	if (is >> input_str) {
		try {
			BigInt temp(input_str);
			num = std::move(temp);
		} catch (const std::invalid_argument& e) {
			is.setstate(std::ios_base::failbit);
		} catch (const std::out_of_range& e) {
			is.setstate(std::ios_base::failbit);
		}
	}

	return is;
}

BigInt BigInt::mod_exp(const BigInt& base, const BigInt& exp, const BigInt& mod) {
	if (mod.isNull() || mod == BigInt(1)) {
		if (mod.isNull()) {
			throw std::runtime_error("Modulo by zero");
		}
		return BigInt(0);
	}
	if (exp.isNegative) {
		throw std::invalid_argument("Negative exp");
	}
	if (exp.isNull()) {
		return BigInt(1);
	}
	if (base.isNull()) {
		return BigInt(0);
	}
	BigInt normalized_base = base % mod;
	if (normalized_base.isNegative) {
		normalized_base += mod;
	}
	BigInt half_exp_result = mod_exp(normalized_base, exp / BigInt(2), mod);
	BigInt result_squared = (half_exp_result * half_exp_result) % mod;
	if ((exp % BigInt(2)).isNull()) {
		return result_squared;
	} else {
		BigInt final_result = (normalized_base * result_squared) % mod;
		return final_result;
	}
}

void BigInt::shiftLeft(const int k) {
	if (k > 0) {
		digits.insert(digits.begin(), k, 0);
	} else if (k < 0) {
		for (int i = 0; i < abs(k); ++i) {
			digits.erase(digits.begin());
		}
	}
}

BigInt BigInt::karatsubaRecursive(BigInt num1, BigInt num2) {
	if (num1.isNull() || num2.isNull()) {
		return BigInt(0);
	}

	size_t n = 1;
	size_t maxDigitsSize = std::max(num1.digits.size(), num2.digits.size());

	while (n < maxDigitsSize) {
		n <<= 1;
	}

	num1.digits.resize(n, 0);
	num2.digits.resize(n, 0);

	if (n == 1) {
		return num1 * num2;
	}

	BigInt xLow, xHigh, yLow, yHigh;
	size_t halfN = n / 2;

	xLow.digits.assign(num1.digits.begin(), num1.digits.begin() + halfN);
	xLow.removeLeadingZeros();

	xHigh.digits.assign(num1.digits.begin() + halfN, num1.digits.end());
	xHigh.removeLeadingZeros();

	yLow.digits.assign(num2.digits.begin(), num2.digits.begin() + halfN);
	yLow.removeLeadingZeros();

	yHigh.digits.assign(num2.digits.begin() + halfN, num2.digits.end());
	yHigh.removeLeadingZeros();

	BigInt prodHighHigh = karatsubaRecursive(xHigh, yHigh);
	BigInt prodLowLow = karatsubaRecursive(xLow, yLow);

	BigInt sumXHighXLow = xHigh + xLow;
	BigInt sumYHighYLow = yHigh + yLow;
	BigInt prodOfSums = karatsubaRecursive(sumXHighXLow, sumYHighYLow);

	BigInt middleTermProduct = prodOfSums;
	middleTermProduct -= prodHighHigh;
	middleTermProduct -= prodLowLow;

	prodHighHigh.shiftLeft(static_cast<int>(n));
	middleTermProduct.shiftLeft(static_cast<int>(halfN));

	BigInt finalProductValue = prodHighHigh;
	finalProductValue += middleTermProduct;
	finalProductValue += prodLowLow;

	finalProductValue.removeLeadingZeros();

	return finalProductValue;
}

BigInt BigInt::karatsuba(const BigInt& num1, const BigInt& num2) {
	bool sign = false;
	if (num1.isNegative != num2.isNegative) {
		sign = true;
	}
	BigInt ans = karatsubaRecursive(num1, num2);
	ans.isNegative = sign;
	if (ans.isNull()) {
		ans.isNegative = false;
	}
	return ans;
}

void BigInt::fftAlgorithm(std::vector<cd>& a, bool invert) {
	int n = a.size();
	if (n == 1) {
		return;
	}

	std::vector<cd> a0(n / 2), a1(n / 2);
	for (int i = 0; 2 * i < n; i++) {
		a0[i] = a[2 * i];
		a1[i] = a[2 * i + 1];
	}
	fftAlgorithm(a0, invert);
	fftAlgorithm(a1, invert);

	double ang = 2 * PI / n * (invert ? -1.0 : 1.0);
	cd w(1.0, 0.0);
	cd wn(cos(ang), sin(ang));

	for (int i = 0; 2 * i < n; i++) {
		a[i] = a0[i] + w * a1[i];
		a[i + n / 2] = a0[i] - w * a1[i];
		if (invert) {
			a[i] /= 2.0;
			a[i + n / 2] /= 2.0;
		}
		w *= wn;
	}
}

void BigInt::fft(BigInt& num, bool invert) {
	size_t n_orig = num.digits.size();

	if (n_orig == 1) {
			std::vector<cd> temp_coeffs(1);
			temp_coeffs[0] = cd(static_cast<long double>(num.digits[0]), 0.0);
			fftAlgorithm(temp_coeffs, invert);
			if (invert) {
				num.digits[0] = static_cast<unsigned long long>(std::round(temp_coeffs[0].real()));
		}
		return;
	}

	size_t n = 1;
	while (n < n_orig) {
		n <<= 1;
	}

	std::vector<cd> coeffs(n, cd(0, 0));
	for (size_t i = 0; i < n_orig; ++i) {
		coeffs[i] = cd(static_cast<long double>(num.digits[i]), 0.0);
	}

	fftAlgorithm(coeffs, invert);

	if (invert) {
		num.digits.resize(n);
		for (size_t i = 0; i < n; ++i) {
			num.digits[i] = static_cast<unsigned long long>(std::round(coeffs[i].real()));
		}

		num.removeLeadingZeros();
	}
}

BigInt BigInt::fftMultiply(const BigInt& num1, const BigInt& num2) {
	if (num1.isNull() || num2.isNull()) {
		return BigInt(0);
	}

	bool resultIsNegative = (num1.isNegative != num2.isNegative);

	std::vector<cd> fa(num1.digits.size());
	for (size_t i = 0; i < num1.digits.size(); ++i) {
		fa[i] = cd(static_cast<long double>(num1.digits[i]), 0.0);
	}

	std::vector<cd> fb(num2.digits.size());
	for (size_t i = 0; i < num2.digits.size(); ++i) {
		fb[i] = cd(static_cast<long double>(num2.digits[i]), 0.0);
	}

	size_t n = 1;
	while (n < fa.size() + fb.size()) {
		n <<= 1;
	}

	fa.resize(n, cd(0, 0));
	fb.resize(n, cd(0, 0));

	fftAlgorithm(fa, false);
	fftAlgorithm(fb, false);

	for (size_t i = 0; i < n; i++) {
		fa[i] *= fb[i];
	}

	fftAlgorithm(fa, true);

	BigInt result;
	result.digits.assign(n + 1, 0);

	unsigned long long carry = 0;
	for (size_t i = 0; i < n; i++) {
		double realVal = fa[i].real();
		unsigned long long termVal = static_cast<unsigned long long>(std::round(realVal));

		unsigned long long currentSum = termVal + carry;
		result.digits[i] = currentSum % BASE;
		carry = currentSum / BASE;
	}

	size_t currentIdx = n;
	while (carry > 0) {
		if (currentIdx >= result.digits.size()) {
			result.digits.push_back(0);
		}
		result.digits[currentIdx] = carry % BASE;
		carry /= BASE;
		currentIdx++;
	}

	result.isNegative = resultIsNegative;
	result.removeLeadingZeros();

	if (result.isNull()) {
		result.isNegative = false;
	}

	return result;
}
