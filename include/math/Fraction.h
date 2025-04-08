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
    
    // 添加double构造函数
    explicit Fraction(double value);
      
    // 添加与double相关的运算符
    Fraction& operator=(double value);
    operator double() const; // 转换运算符
    
    Fraction abs() const;
    static Fraction abs(const Fraction& f); // 静态方法
    
    // 添加比较运算符
    bool operator<(const Fraction& other) const;
    bool operator>(const Fraction& other) const;
    bool operator<=(const Fraction& other) const;
    bool operator>=(const Fraction& other) const;
    
    // 添加与double的比较
    bool operator<(double value) const;
    bool operator>(double value) const;
    bool operator<=(double value) const;
    bool operator>=(double value) const;
    
    
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
