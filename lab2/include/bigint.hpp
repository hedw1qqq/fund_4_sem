#pragma once
#include <math.h>

#include <compare>
#include <complex>
#include <iomanip>
#include <iostream>
#include <vector>

class BigInt {
   public:
	BigInt();
	BigInt(long long value);
	explicit BigInt(const std::string& str);
	BigInt(const BigInt& other);
	BigInt(BigInt&& other) noexcept;
	~BigInt() = default;

	BigInt& operator=(const BigInt& other);
	BigInt& operator=(BigInt&& other) noexcept;

	BigInt operator+(const BigInt& other) const;
	BigInt operator-(const BigInt& other) const;
	BigInt operator*(const BigInt& other) const;
	BigInt operator/(const BigInt& other) const;

	BigInt& operator+=(const BigInt& other);
	BigInt& operator-=(const BigInt& other);
	BigInt& operator*=(const BigInt& other);
	BigInt& operator/=(const BigInt& other);
	BigInt operator%(const BigInt& other) const;
	BigInt& operator%=(const BigInt& other);
	BigInt& operator++();
	BigInt& operator--();
	BigInt operator--(int);
	BigInt operator++(int);
	bool operator==(const BigInt& other) const;
	bool operator!=(const BigInt& other) const;
	bool operator<(const BigInt& other) const;
	bool operator>(const BigInt& other) const;
	bool operator<=(const BigInt& other) const;
	bool operator>=(const BigInt& other) const;

	friend std::istream& operator>>(std::istream& is, BigInt& num);
	friend std::ostream& operator<<(std::ostream& os, const BigInt& num);

	static BigInt mod_exp(const BigInt& base, const BigInt& exp, const BigInt& mod);
	void shiftLeft(int k);
	static BigInt karatsuba(const BigInt& num1, const BigInt& num2);

	using cd = std::complex<long double>;
	inline static const double PI = acos(-1.0L);
    static void fft(BigInt& a, bool invert);
    static BigInt fftMultiply(const BigInt& num1, const BigInt& num2);

   private:
    static void fftAlgorithm(std::vector<cd>& a, bool invert);
	static BigInt karatsubaRecursive(BigInt num1, BigInt num2);
	std::vector<unsigned long long> digits;
	bool isNegative;
	inline static const unsigned long long BASE = 1000000000;
	inline static const int BASE_DIGITS = log10(BASE);
	void removeLeadingZeros();
	static bool validateString(const std::string& str);
	void subtractValue(const BigInt& smaller);
	void addValue(const BigInt& other);
	std::strong_ordering compareValue(const BigInt& other) const;
	bool isNull() const;
};
