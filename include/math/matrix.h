//
//  matrix.h
//  com.xm.xlib
//
//  Created by xiaominghe2014@gmail.com on 2023/6/1.
//

#ifndef matrix_h
#define matrix_h

#include <vector>
#include <string>

namespace xlib {
    // 定义矩阵类
    class Matrix {
        public:
            int m, n; // 矩阵行数和列数
            std::vector<std::vector<double>> a; // 二维数组表示的矩阵
        public:
            
            Matrix() noexcept;
            
            Matrix(int rows, int cols) noexcept;
            
            Matrix(std::vector<std::vector<double>>& matrix);

            // 构造函数，用于初始化矩阵
            Matrix(const Matrix& mat) noexcept;
            // 重载运算符+,用于矩阵加法运算
            Matrix operator+(const Matrix& mat) const;
            // 重载运算符-,用于矩阵减法运算
            Matrix operator-(const Matrix& mat) const;
            // 重载运算符*,用于矩阵乘法运算
            Matrix operator*(const Matrix& mat) const;
            
            Matrix operator=(std::vector<std::vector<double>>& matrix);
        
            Matrix operator=(const Matrix& mat);
            
            double& operator()(int i,int j);
            
            std::string toString() const;
                
            //行列式的值
            double det();
            
            //k阶子式
            std::vector<Matrix> submatrix(int k);
        
            //矩阵的秩
            int rank();
            
            //转置矩阵 transpose Matrix
            Matrix transpose();
            
            //是否为方阵
            inline bool isSquare(){ return m==n;};
            
            //高斯消元解方程组---唯一解
            std::vector<double> gaussianElimination(std::vector<double>& B);
                
            static Matrix solveLightsOutPuzzle(int lightSize);
            std::vector<int> solveLightsOutPuzzle(std::vector<int>& B,int mod);
        
            //增广矩阵
            Matrix augmented(const Matrix& mat);
            
            //n 阶单位矩阵
            static Matrix identity(int n);
        
            //逆矩阵 inverse Matrix
            Matrix inverse();
            
            // 计算特征多项式矩阵
            Matrix charPoly(double lambda);
            
            //TODO there are some bugs, please don't use this
            std::vector<double> eigenvalues();
            
            // 使用牛顿迭代法求解特征值--单特征值
            std::vector<double> eigenvaluesNewton(double epsilon, int max_iterations);
            
            // 使用二分法求解特征值
            std::vector<double> eigenvaluesBinarySearch(double left, double right, double epsilon);
        
            // 计算矩阵的迹
            double trace() const;
            
            bool operator==(const Matrix& mat) const;
    };

    std::vector<double> normalGaussianElimination(std::vector<std::vector<double>>& A, std::vector<double>& b);

    double eigenvaluePowerMethod(std::vector<std::vector<double>>& A, int n, double init);
    
    Matrix operator*(double scalar, const Matrix& matrix);

    std::ostream& operator<<(std::ostream& os, const Matrix& matrix);
    
    std::string toString(const std::vector<std::vector<double>>& matrix);

    //矩阵的LU 分解
    void luDecomposition(const std::vector<std::vector<double>>& A, std::vector<std::vector<double>>& L, std::vector<std::vector<double>>& U);
}

#endif /* matrix_h */
