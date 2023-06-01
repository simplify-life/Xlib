//
//  matrix.cpp
//  com.xm.xlib
//
//  Created by apple on 2023/6/1.
//

#include "math/matrix.h"
#include <cstring>
#include <stdexcept>

namespace xlib {

    Matrix::Matrix() noexcept{
        m = 1;
        n = 1;
        a = std::vector<std::vector<int>>(m, std::vector<int>(n));
    }

    Matrix::Matrix(int rows, int cols) noexcept{
        m = rows;
        n = cols;
        a = std::vector<std::vector<int>>(m, std::vector<int>(n));
    }

    Matrix::Matrix(std::vector<std::vector<int>>& matrix) {
        m = matrix.size();
        n = matrix[0].size();
        a = std::vector<std::vector<int>>(m, std::vector<int>(n));
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                a[i][j] = matrix[i][j];
            }
        }
    }

    Matrix::Matrix(const Matrix& mat) noexcept{
        m = mat.m;
        n = mat.n;
        a = std::vector<std::vector<int>>(m, std::vector<int>(n));
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

    Matrix Matrix::operator=(std::vector<std::vector<int>>& matrix){
        if(a!=matrix){
            a.swap(matrix);
            m = a.size();
            n = a[0].size();
        }
        return *this;
    }

    int& Matrix::operator()(int i,int j) {
        return a[i][j];
    }

    std::string Matrix::toString() const{
        std::string str = "";
        for(int r = 0 ; r< m; r++){
            for(int c=0 ; c< n; c++){
                str += std::to_string(a[r][c]) + " ";
            }
            str += "\n";
        }
        return str;
    }

    // TODO
    int Matrix::rank(){
        int r = m;
//        if(m==n){
//            //方阵
//            for(int i = 0 ; i< m; i++){ //遍历所有行
//                for(int j = i+1 ; j< m ; j++){ //遍历除当前行之外的所有行
//                    bool is_row_independent = true; // 假设当前行是独立的
//                    for(int k=0; k<m ; k++){ // 遍历当前行的所有元素
//                        if(a[i][k]!=0 && a[j][k] % a[i][k]==0){ // 如果当前行与当前列存在倍数关系，则认为它们相关联
//                            is_row_independent = false;
//                            break;
//                        }
//                    }
//                    if(is_row_independent){
//                        r--;
//                        break;
//                    }
//                }
//            }
//        }else{
//            //不是方阵，直接计算秩
//            int pivot = a[0][0]; // 从第一行的第一个元素开始寻找主元
//            for (int i = 1; i < m; i++) { // 从第二行开始遍历所有元素
//                if (a[i][0] != pivot) { // 如果找到不同的主元，则跳出循环
//                    pivot = a[i][0];
//                    break;
//                }
//            }
//            r = m - pivot; // 根据主元的位置计算秩
//        }
        return r;
    }
}
