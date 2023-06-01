//
//  matrix.h
//  com.xm.xlib
//
//  Created by xiaominghe2014@gmail.com on 2023/6/1.
//

#ifndef matrix_h
#define matrix_h

#include <vector>

namespace xlib {
    // 定义矩阵类
    class Matrix {
    public:
        int m, n; // 矩阵行数和列数
        std::vector<std::vector<int>> a; // 二维数组表示的矩阵
    public:
        
        Matrix() noexcept;
        
        Matrix(int rows, int cols) noexcept;
        
        Matrix(std::vector<std::vector<int>>& matrix);

        // 构造函数，用于初始化矩阵
        Matrix(const Matrix& mat) noexcept;
        // 重载运算符+,用于矩阵加法运算
        Matrix operator+(const Matrix& mat) const;
        // 重载运算符-,用于矩阵减法运算
        Matrix operator-(const Matrix& mat) const;
        // 重载运算符*,用于矩阵乘法运算
        Matrix operator*(const Matrix& mat) const;
        
        Matrix operator=(std::vector<std::vector<int>>& matrix);
        
        int& operator()(int i,int j);
        
        std::string toString() const;
        
        int rank();
    };
}

#endif /* matrix_h */
