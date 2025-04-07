//
//  matrix.cpp
//  com.xm.xlib
//
//  Created by apple on 2023/6/1.
//

#include "math/matrix.h"
#include "math/util.h"
#include "math/Fraction.h"
#include <cstring>
#include <stdexcept>
#include <numeric>
#include <iostream>
#include <sstream>
#include <cmath>

namespace xlib {

    std::vector<double> matrixVectorMultiply(std::vector<std::vector<double>>& A, std::vector<double>& x) {
        int m = A.size();
        int n = A[0].size();
        std::vector<double> y(m);
        for (int i = 0; i < m; i++) {
            y[i] = 0;
            for (int j = 0; j < n; j++) {
                y[i] += A[i][j] * x[j];
            }
        }
        return y;
    }

    std::vector<double> normalGaussianElimination(std::vector<std::vector<double>>& A, std::vector<double>& b) {
        int n = A.size();
        // 构造增广矩阵
        std::vector<std::vector<double>> Ab(n, std::vector<double>(n + 1));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                Ab[i][j] = A[i][j];
            }
            Ab[i][n] = b[i];
        }
        // 进行高斯消元
        for (int i = 0; i < n; i++) {
            // 选取主元
            int pivot = i;
            for (int j = i + 1; j < n; j++) {
                if (abs(Ab[j][i]) > abs(Ab[pivot][i])) {
                    pivot = j;
                }
            }
            // 交换行
            if (pivot != i) {
                std::swap(Ab[i], Ab[pivot]);
            }
            // 消元
            for (int j = i + 1; j <= n; j++) {
                Ab[i][j] /= Ab[i][i];
            }
            for (int j = 0; j < n; j++) {
                if (j != i) {
                    for (int k = i + 1; k <= n; k++) {
                        Ab[j][k] -= Ab[j][i] * Ab[i][k];
                    }
                }
            }
        }
        // 提取解向量
        std::vector<double> x(n);
        for (int i = 0; i < n; i++) {
            x[i] = Ab[i][n];
        }
        return x;
    }

    double vectorNorm(std::vector<double>& x) {
        double norm = 0;
        for (int i = 0; i < x.size(); i++) {
            norm += x[i] * x[i];
        }
        return sqrt(norm);
    }

    double eigenvaluePowerMethod(std::vector<std::vector<double>>& A, int n, double init) {
        // 初始化迭代向量
        std::vector<double> x(n, 1);
        x[0] = init;
        // 进行迭代
        for (int i = 0; i < 1000; i++) {
            // 计算新的迭代向量
            std::vector<double> Ax = matrixVectorMultiply(A, x);
            double norm = vectorNorm(Ax);
            for (int j = 0; j < n; j++) {
                x[j] = Ax[j] / norm;
            }
            // 判断是否收敛
            auto newAx = matrixVectorMultiply(A, x);
            if (vectorNorm(newAx) / norm < 1e-6) {
                break;
            }
        }
        // 计算特征值
        std::vector<double> Ax = matrixVectorMultiply(A, x);
        double lambda = 0;
        for (int i = 0; i < n; i++) {
            lambda += Ax[i] * x[i];
        }
        return lambda;
    }

    // 将矩阵的每个元素乘以一个标量
    Matrix operator*(double scalar, const Matrix& matrix) {
        Matrix result(matrix.m, matrix.n);
        for (int i = 0; i < matrix.m; i++) {
            for (int j = 0; j < matrix.n; j++) {
                result.a[i][j] = scalar * matrix.a[i][j];
            }
        }
        return result;
    }

    Matrix::Matrix() noexcept{
        m = 1;
        n = 1;
        a = std::vector<std::vector<double>>(m, std::vector<double>(n));
    }

    Matrix::Matrix(int rows, int cols) noexcept{
        m = rows;
        n = cols;
        a = std::vector<std::vector<double>>(m, std::vector<double>(n));
    }

    Matrix::Matrix(std::vector<std::vector<double>>& matrix) {
        m = matrix.size();
        n = matrix[0].size();
        a = std::vector<std::vector<double>>(m, std::vector<double>(n));
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                a[i][j] = matrix[i][j];
            }
        }
    }

    Matrix::Matrix(const Matrix& mat) noexcept{
        m = mat.m;
        n = mat.n;
        a = std::vector<std::vector<double>>(m, std::vector<double>(n));
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                a[i][j] = mat.a[i][j];
            }
        }
    }

    Matrix Matrix::operator+(const Matrix& mat) const {
        if(mat.m<m ||mat.n<n){
            throw std::runtime_error("Failed add Matrix , row or col not equal");
        }
        Matrix result = *this;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                result.a[i][j] += mat.a[i][j];
            }
        }
        return result;
    }

    Matrix Matrix::operator-(const Matrix& mat) const {
        if(mat.m<m ||mat.n<n){
            throw std::runtime_error("Failed sub Matrix , row or col not equal");
        }
        Matrix result = *this;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                result.a[i][j] -= mat.a[i][j];
            }
        }
        return result;
    }

    Matrix Matrix::operator*(const Matrix& mat) const {
        if(mat.m!=n){
            throw std::runtime_error("Failed multiply by Matrix , row or col not equal");
        }
        Matrix result = Matrix(m, mat.n);
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < mat.n; j++) {
                int sum = 0;
                for(int k = 0 ; k< n ; k++){
                    sum += a[i][k]*mat.a[k][j];
                }
                result.a[i][j]  = sum;
            }
        }
        return result;
    }

    Matrix Matrix::operator=(std::vector<std::vector<double>>& matrix){
        if(a!=matrix){
            a.swap(matrix);
            m = a.size();
            n = a[0].size();
        }
        return *this;
    }

    Matrix Matrix::operator=(const Matrix& mat){
        return Matrix(mat);
    }

    double& Matrix::operator()(int i,int j) {
        return a[i][j];
    }

    std::string Matrix::toString() const{
        std::stringstream ss;
        for(int r = 0 ; r< m; r++){
            for(int c=0 ; c< n; c++){
                ss << a[r][c] << " ";
            }
            ss << "\n";
        }
        return ss.str();
    }

    double Matrix::det(){
        if(m!=n){
            throw std::runtime_error("det must be row equal col");
        }
        double result = 0;
        std::vector<int> arr;
        for(int i = 0 ; i < n ; i++){
            arr.push_back(i);
        }
        auto perm = permutation(arr, n);
        for(auto& pm:perm){
            int cnt =  reverse_count(pm);
            double mul = 1;
            for(int i = 0 ; i< n ; i++){
                mul = mul*a[i][pm[i]];
            }
            if(cnt%2){
                mul = -mul;
            }
            result += mul;
        }
        return result;
    }

    // FIXME 修改为迭代器，性能可以更好
//    SubmatrixGenerator Matrix::submatrix(int k){
//        int max = m>n?n:m;
//        if(k>max){
//            throw std::runtime_error("The N-th submatrix of a matrix must be smaller than both the row and column dimensions.");
//        }
//        return SubmatrixGenerator(*this,k);
//    }
   
    int Matrix::rank() {
        int rank = 0; // 初始化秩为0
        Matrix temp = *this; // 创建一个临时矩阵，用于进行高斯消元操作
        // 遍历矩阵的行和列
        for (int row = 0, col = 0; row < temp.m && col < temp.n; ++col) {
            int pivotRow = row; // 初始化主元所在行为当前行
            // 寻找当前列中绝对值最大的元素所在行
            for (int i = row + 1; i < temp.m; ++i) {
                if (abs(temp.a[i][col]) > abs(temp.a[pivotRow][col])) {
                    pivotRow = i;
                }
            }
            // 如果找到的主元不为0
            if (temp.a[pivotRow][col] != 0) {
                // 如果主元所在行不等于当前行，交换两行
                if (pivotRow != row) {
                    std::swap(temp.a[pivotRow], temp.a[row]);
                }
                // 使用主元所在行消去其他行的当前列元素
                for (int i = row + 1; i < temp.m; ++i) {
                    double factor = temp.a[i][col] / temp.a[row][col]; // 计算消元因子
                    // 更新其他行的元素
                    for (int j = col; j < temp.n; ++j) {
                        temp.a[i][j] -= factor * temp.a[row][j];
                    }
                }
                ++rank; // 矩阵秩加1
                ++row; // 移动到下一行
            }
        }
        return rank; // 返回矩阵的秩
    }

    Matrix Matrix::transpose(){
        Matrix T(n,m);
        for(int i=0;i<m;i++){
            for (int j = 0 ; j<n; j++) {
                T(j,i) = a[i][j];
            }
        }
        return T;
    }

    std::vector<double> Matrix::gaussianElimination(std::vector<double>& B){
        
        if(m!=n){
            throw std::runtime_error("matrix must be row equal col");
        }
        
        if(B.size()!=m){
            throw std::runtime_error("vector size must be equal matrix col");
        }
        
        if(0==det()){
            //非满秩矩阵，无唯一解
            throw std::runtime_error("not full rank matrix");
            //@TODO 找到一种可行解是否可以？
        }
        
        std::vector<double> result = std::vector<double>(m);
        std::vector<int> canceled = std::vector<int>(m);
        auto tmp = *this;
        for(int j = 0 ; j< m ; j++){
            bool pivot = false;
            for(int i = 0 ; i< m ; i++){
                //找到第i 行 第j 列不为0的元素作为主元
                if(!canceled[i] && !pivot && tmp.a[i][j]!=0){
                    pivot = true;
                    canceled[i] = 1;
                    //查找最小公倍数
                    std::vector<int> v;
                    for(int k = 0 ; k< m ; k++){
                        if(tmp.a[k][j]!=0){
                            v.push_back(tmp.a[k][j]);
                        }
                    }
                    int lc = lcm(v);
                    for(int k = 0 ; k< m ; k++){
                        if(tmp.a[k][j]!=0){
                            int mul = lc/tmp.a[k][j];
                            for(int l = 0 ; l< m; l++){
                                tmp.a[k][l] = tmp.a[k][l]*mul;
                            }
                            B[k] = B[k]*mul;
                        }
                    }
                    //消元
                    for(int k = 0 ; k< m ; k++){
                        if(k!=i && tmp.a[k][j]!=0){
                            //k行减去i行
                            for(int l = 0 ; l< m; l++){
                                tmp.a[k][l] -= tmp.a[i][l];
                            }
                            B[k] -= B[i];
                        }
                    }
                }
            }
            if(!pivot){
                //没有找到主元
                throw std::runtime_error("not pivot in matrix on " + std::to_string(j));
            }
        }
        for(int i = 0 ; i< m ; i++){
            for(int j = 0 ; j< m ; j++){
                //i 行 j 列
                if(tmp.a[i][j]!=0){
                    //第j个解
                    result[j] = B[i]/tmp.a[i][j];
                }
            }
        }
        return result;
    }

    std::vector<int> Matrix::solveLightsOutPuzzle(std::vector<int>& B,int mod){
        if(m!=n){
            throw std::runtime_error("matrix must be row equal col");
        }
        
        if(B.size()!=m){
            throw std::runtime_error("vector size must be equal matrix col");
        }
        
//        if(0==det()){
//            //非满秩矩阵，无唯一解
//            //找到一种可行解
//        }
        std::vector<int> result = std::vector<int>(m);
        std::vector<int> canceled = std::vector<int>(m);
        std::vector<int> freeArg;
        auto tmp = *this;
        for(int j = 0 ; j< m ; j++){
            bool pivot = false;
            for(int i = 0 ; i< m ; i++){
                //找到第i 行 第j 列不为0的元素作为主元
                if(!canceled[i] && !pivot && tmp.a[i][j]!=0){
                    pivot = true;
                    canceled[i] = 1;
                    //查找最小公倍数
                    std::vector<int> v;
                    for(int k = 0 ; k< m ; k++){
                        if(tmp.a[k][j]!=0){
                            v.push_back(tmp.a[k][j]);
                        }
                    }
                    int lc = lcm(v);
                    for(int k = 0 ; k< m ; k++){
                        if(tmp.a[k][j]!=0){
                            int mul = lc/tmp.a[k][j];
                            for(int l = 0 ; l< m; l++){
                                tmp.a[k][l] = ((int)tmp.a[k][l]*mul)%mod;
                            }
                            B[k] = (B[k]*mul)%mod;
                        }
                    }
                    //消元
                    for(int k = 0 ; k< m ; k++){
                        if(k!=i && tmp.a[k][j]!=0){
                            //k行减去i行
                            for(int l = 0 ; l< m; l++){
                                tmp.a[k][l] = int(tmp.a[k][l]-tmp.a[i][l])%mod;
                            }
                            B[k] = (B[k]-B[i])%mod;
                        }
                    }
                }
            }
            if(!pivot){
                //没有找到主元
//                throw std::runtime_error("not pivot in matrix on " + std::to_string(j));
                //自由变量
                freeArg.push_back(j);
            }
        }
        for(int i = 0 ; i< m ; i++){
            for(int j = 0 ; j< m ; j++){
                //i 行 j 列
                if(tmp.a[i][j]!=0){
                    //第j个解
                    result[j] = abs(int(B[i]/tmp.a[i][j])%mod);
                }
            }
        }
        for(auto& idx : freeArg){
            result[idx] = 0;
        }
        return result;
    }

    Matrix Matrix::solveLightsOutPuzzle(int lightSize){
        if(lightSize<1 || lightSize > 65535){
            throw std::overflow_error("lightSize too large");
        }
        int r = lightSize*lightSize;
        Matrix matrixLight(r,r);
        for(int i = 0 ; i< lightSize ; i++){
            for(int j = 0 ; j< lightSize; j++){
                //点亮 i 行j 列的灯
                int c = i*lightSize + j;
                // 上
                if(i>0){
                    int r = (i-1)*lightSize+j;
                    matrixLight(c,r) = 1;
                }
                // 下
                if(i<lightSize-1){
                    int r = (i+1)*lightSize+j;
                    matrixLight(c,r) = 1;
                }
                // 中
                {
                    int r = i*lightSize+j;
                    matrixLight(c,r) = 1;
                }
                // 左
                if(j>0){
                    int r = i*lightSize+j-1;
                    matrixLight(c,r) = 1;
                }
                // 右
                if(j<lightSize-1){
                    int r = i*lightSize+j+1;
                    matrixLight(c,r) = 1;
                }
            }
        }
        
        std::vector<int> status=std::vector<int>(lightSize*lightSize,1);

        auto result = matrixLight.solveLightsOutPuzzle(status,2);
        
        Matrix newMatrix(lightSize, lightSize);
        for (int i = 0; i < lightSize; i++) {
            for (int j = 0; j < lightSize; j++) {
                newMatrix(i, j) = result[i * lightSize + j];
            }
        }
        return newMatrix;
    }

    Matrix Matrix::augmented(const Matrix& mat){
        Matrix AM(m,n+mat.n);
        for(int i = 0 ; i< m ; i++){
            for(int j = 0 ; j < n+mat.n ; j++){
                AM(i,j) = j<n ? a[i][j] : mat.a[i][j-n];
            }
        }
        return AM;
    }
    
    Matrix Matrix::identity(int n){
        Matrix E(n,n);
        for(int i = 0 ; i < n ; i++){
            E(i,i) = 1;
        }
        return E;
    }
    
    Matrix Matrix::inverse(){
        if(m!=n){
            throw std::runtime_error("matrix must be row equal col");
        }
        auto tmp = *this;
        Matrix E = Matrix::identity(n);
        for(int i = 0 ; i< n ; i++){
            if(tmp.a[i][i]!=0){
                //查找最小公倍数
                std::vector<int> v;
                for(int k = 0 ; k< n ; k++){
                    if(tmp.a[k][i]!=0){
                        v.push_back(tmp.a[k][i]);
                    }
                }
                int lc = lcm(v);
                for(int k = 0 ; k< n ; k++){
                    if(tmp.a[k][i]!=0){
                        int mul = lc/tmp.a[k][i];
                        for(int l = 0 ; l< n; l++){
                            tmp.a[k][l] = tmp.a[k][l]*mul;
                            E.a[k][l] = E.a[k][l]*mul;
                        }
                    }
                }
                //消元
                for(int k = 0 ; k< n ; k++){
                    if(k!=i && tmp.a[k][i]!=0){
                        //k行减去i行
                        for(int l = 0 ; l< n; l++){
                            tmp.a[k][l] -= tmp.a[i][l];
                            E.a[k][l] -= E.a[i][l];
                        }
                    }
                }
            }else{
                //为0，得让它变成1
                bool set = true;
                for(int p = 0 ; p< n ; p++){
                    if(!set){
                        break;
                    }
                    if(p!=i){
                        if(tmp.a[p][i]!=0){
                            set = false;
                            //加上p行
                            for(int k = 0 ; k< n ; k++){
                                tmp.a[i][k] += tmp.a[p][k];
                                E.a[i][k] += E.a[p][k];
                            }
                        }
                    }
                }
                if(set){
                    throw std::runtime_error("matrix have not inverse");
                }
                i-=1;
            }
        }
        for(int i = 0 ; i< n ; i++){
            if(tmp.a[i][i]!=1){
                int factor = tmp.a[i][i];
                for(int j = 0 ; j< n; j++){
                    tmp.a[i][j] /= factor;
                    E.a[i][j] /= factor;
                }
            }
        }
        return E;
    }

    // 计算特征多项式
    Matrix Matrix::charPoly(double lambda) {
        // 初始化单位矩阵
        Matrix I = Matrix::identity(n);
        // 计算 A-λI
        Matrix A_minus_lambda_I = (*this) - lambda * I;
        // 返回行列式
        return A_minus_lambda_I;
    }

    std::vector<double> Matrix::eigenvalues() {
        // 求解特征值
        std::vector<double> eigenvalues;
        for (int i = 0; i < n; i++) {
            double lambda = eigenvaluePowerMethod(a, n, a[i][i]);
            eigenvalues.push_back(lambda);
        }
        // 求解特征向量
        std::vector<std::vector<double>> eigenvectors;
        for (int i = 0; i < n; i++) {
            std::vector<std::vector<double>> A_minus_lambdaI(n, std::vector<double>(n));
            for (int j = 0; j < n; j++) {
                for (int k = 0; k < n; k++) {
                    A_minus_lambdaI[j][k] = a[j][k] - eigenvalues[i] * (j == k ? 1 : 0);
                }
            }
            std::vector<double> b(n, 0);
            b[i] = 1;
            std::vector<double> eigenvector = normalGaussianElimination(A_minus_lambdaI, b);
            eigenvectors.push_back(eigenvector);
        }
        // 将特征向量组合成矩阵
        Matrix V(eigenvectors);
        // 返回特征值和特征向量的乘积
        
        Matrix result = V.inverse() * (*this) * V;
        std::vector<double> r;
        for (int i = 0; i < result.m; i++) {
            r.push_back(result.a[i][i]);
        }
        return r;
    }

    // 使用牛顿迭代法求解特征值
    std::vector<double> Matrix::eigenvaluesNewton(double epsilon, int max_iterations) {
        // 初始化结果向量
        std::vector<double> result;
        // 遍历每个元素
        for (int i = 0; i < n; i++) {
            // 初始化迭代起点
            double x = a[i][i];
            // 进行牛顿迭代
            for (int j = 0; j < max_iterations; j++) {
                double f = charPoly(x).det();
                double f_prime = ((*this) - x * Matrix::identity(n)).trace();
                double x_new = x - f / f_prime;
                if (abs(x_new - x) < epsilon) {
                    // 收敛，将特征值加入结果向量
                    result.push_back(x_new);
                    break;
                }
                x = x_new;
            }
        }
        return result;
    }

    // 使用二分法求解特征值
    std::vector<double> Matrix::eigenvaluesBinarySearch(double left, double right, double epsilon) {
        // 初始化结果向量
        std::vector<double> result;
        // 遍历每个元素
        for (int i = 0; i < n; i++) {
            double l = left;
            double r = right;
            // 进行二分法
            while (r - l > epsilon) {
                double mid = (l + r) / 2;
                if (charPoly(mid).det() > 0) {
                    r = mid;
                } else {
                    l = mid;
                }
            }
            // 将特征值加入结果向量
            result.push_back((l + r) / 2);
        }
        return result;
    }

    bool Matrix::operator==(const Matrix& mat) const{
        if (m != mat.m || n != mat.n) {
            return false;
        }
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (a[i][j] != mat.a[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }

    double Matrix::trace() const {
        double result = 0;
        for (int i = 0; i < n; i++) {
            result += a[i][i];
        }
        return result;
    }

    // 重载输出运算符，使其能够输出 Matrix 类型的对象
    std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
        for (int i = 0; i < matrix.m; i++) {
            for (int j = 0; j < matrix.n; j++) {
                os << matrix.a[i][j] << " ";
            }
            os << std::endl;
        }
        return os;
    }

    // LU分解函数
    void luDecomposition(const std::vector<std::vector<double>>& A, std::vector<std::vector<double>>& L, std::vector<std::vector<double>>& U) {
        int n = A.size();
        L.resize(n, std::vector<double>(n, 0.0));
        U.resize(n, std::vector<double>(n, 0.0));
        for (int i = 0; i < n; i++) {
            U[0][i] = A[0][i];
            L[i][0] = A[i][0] / U[0][0];
        }
       //计算U的第r行，L的第r列元素
        for (int r = 1; r < n; r++) {
            for (int i = r; i < n; i++) {
                double sLU = 0.0;
                for (int k = 0; k < r; k++) {
                    sLU += L[r][k] * U[k][i];
                }
                U[r][i] = A[r][i] - sLU;
                if(i==r){
                    L[r][r]=1;
                }
                else if(r==n){
                    L[n][n]=1;  
                } 
                else{
                    double sLU = 0.0;
                    for (int k = 0; k < r; k++) {
                        sLU += L[i][k] * U[k][r];
                    }
                    L[i][r] = (A[i][r] - sLU) / U[r][r];
                } 
            }
        }
    }

}
