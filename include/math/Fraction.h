//
//  fraction.h
//  com.xm.xlib
//
//  Created by apple on 2025/4/7.
//

#ifndef Fraction_h
#define Fraction_h

#include "macro/XBase.h"
#include <string>

XLIB_BEGAIN

class Fraction {
public:
    using IntType = int64;
    
    // 构造函数
    Fraction(IntType n = 0, IntType d = 1);
    
    // 运算符重载
    Fraction operator+(const Fraction& other) const;
    Fraction operator-(const Fraction& other) const;
    Fraction operator*(const Fraction& other) const;
    Fraction operator/(const Fraction& other) const;
    bool operator==(const Fraction& other) const;
    
    // 转换函数
    double toDouble() const;
    std::string toString() const;
    
    // 获取分子分母
    IntType getNumerator() const { return numerator; }
    IntType getDenominator() const { return denominator; }
    
private:
    IntType numerator;   // 分子
    IntType denominator; // 分母
    
    // 辅助函数
    void normalize();    // 规范化分数
    void reduce();       // 约分
};

// 全局运算符重载
std::ostream& operator<<(std::ostream& os, const Fraction& f);

XLIB_END

#endif /* Fraction_h */
