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

// 从double构造分数
Fraction::Fraction(double value) {
    // 将double转换为分数
    // 使用一个合理的精度，例如1e-10
    const double epsilon = 1e-10;
    const int maxIterations = 100;
    
    // 处理整数部分
    IntType integerPart = static_cast<IntType>(std::floor(std::abs(value)));
    double fractionalPart = std::abs(value) - integerPart;
    
    // 如果小数部分接近0，直接返回整数
    if (fractionalPart < epsilon) {
        numerator = value < 0 ? -integerPart : integerPart;
        denominator = 1;
        return;
    }
    
    // 使用连分数展开来找到最接近的分数
    IntType n0 = 0, n1 = 1, n2 = 0;
    IntType d0 = 1, d1 = 0, d2 = 0;
    
    double x = fractionalPart;
    int iteration = 0;
    
    while (iteration < maxIterations) {
        IntType a = static_cast<IntType>(std::floor(1.0 / x));
        double remainder = 1.0 / x - a;
        
        n2 = a * n1 + n0;
        d2 = a * d1 + d0;
        
        // 检查是否已经足够精确
        if (std::abs(static_cast<double>(n2) / d2 - fractionalPart) < epsilon) {
            break;
        }
        
        n0 = n1;
        n1 = n2;
        d0 = d1;
        d1 = d2;
        
        x = remainder;
        iteration++;
    }
    
    // 设置分子和分母
    numerator = n2 + integerPart * d2;
    if (value < 0) {
        numerator = -numerator;
    }
    denominator = d2;
    
    normalize();
    reduce();
}

// 赋值运算符
Fraction& Fraction::operator=(double value) {
    *this = Fraction(value);
    return *this;
}

// 转换运算符
Fraction::operator double() const {
    return toDouble();
}

// 绝对值方法
Fraction Fraction::abs() const {
    return Fraction(std::abs(numerator), denominator);
}

// 静态绝对值方法
Fraction Fraction::abs(const Fraction& f) {
    return f.abs();
}

// 比较运算符
bool Fraction::operator<(const Fraction& other) const {
    return numerator * other.denominator < other.numerator * denominator;
}

bool Fraction::operator>(const Fraction& other) const {
    return other < *this;
}

bool Fraction::operator<=(const Fraction& other) const {
    return !(other < *this);
}

bool Fraction::operator>=(const Fraction& other) const {
    return !(*this < other);
}

// 与double的比较
bool Fraction::operator<(double value) const {
    return toDouble() < value;
}

bool Fraction::operator>(double value) const {
    return toDouble() > value;
}

bool Fraction::operator<=(double value) const {
    return toDouble() <= value;
}

bool Fraction::operator>=(double value) const {
    return toDouble() >= value;
}

XLIB_END
