//
//  Fraction.cpp
//  com.xm.xlib
//
//  Created by apple on 2025/4/7.
//

#include "math/Fraction.h"
#include "math/util.h"
#include <stdexcept>
#include <cmath>
#include <sstream>

XLIB_BEGAIN

Fraction::Fraction(IntType n, IntType d) : numerator(n), denominator(d) {
    if (d == 0) {
        throw std::runtime_error("Denominator cannot be zero");
    }
    normalize();
    reduce();
}

void Fraction::normalize() {
    if (denominator < 0) {
        numerator = -numerator;
        denominator = -denominator;
    }
}

void Fraction::reduce() {
    IntType g = gcd(std::abs(numerator), std::abs(denominator));
    numerator /= g;
    denominator /= g;
}

Fraction Fraction::operator+(const Fraction& other) const {
    IntType n = numerator * other.denominator + other.numerator * denominator;
    IntType d = denominator * other.denominator;
    return Fraction(n, d);
}

Fraction Fraction::operator-(const Fraction& other) const {
    IntType n = numerator * other.denominator - other.numerator * denominator;
    IntType d = denominator * other.denominator;
    return Fraction(n, d);
}

Fraction Fraction::operator*(const Fraction& other) const {
    IntType n = numerator * other.numerator;
    IntType d = denominator * other.denominator;
    return Fraction(n, d);
}

Fraction Fraction::operator/(const Fraction& other) const {
    if (other.numerator == 0) {
        throw std::runtime_error("Division by zero");
    }
    IntType n = numerator * other.denominator;
    IntType d = denominator * other.numerator;
    return Fraction(n, d);
}

bool Fraction::operator==(const Fraction& other) const {
    return numerator * other.denominator == other.numerator * denominator;
}

double Fraction::toDouble() const {
    return static_cast<double>(numerator) / denominator;
}

std::string Fraction::toString() const {
    std::stringstream ss;
    ss << numerator;
    if (denominator != 1) {
        ss << "/" << denominator;
    }
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Fraction& f) {
    return os << f.toString();
}

XLIB_END
