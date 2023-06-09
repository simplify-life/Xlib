//
//  matrix.cpp
//  com.xm.xlib
//
//  Created by apple on 2023/6/1.
//

#include "math/matrix.h"
#include "math/util.h"
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

    int Matrix::det(){
        if(m!=n){
            throw std::runtime_error("det must be row equal col");
        }
        int result = 0;
        std::vector<int> arr;
        for(int i = 0 ; i < n ; i++){
            arr.push_back(i);
        }
        auto perm = permutation(arr, n);
        for(auto& pm:perm){
            int cnt =  reverse_count(pm);
            int mul = 1;
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

    // TODO 修改为迭代器，性能可以更好
    std::vector<Matrix> Matrix::submatrix(int k){
        int max = m>n?n:m;
        if(k>max){
            throw std::runtime_error("The N-th submatrix of a matrix must be smaller than both the row and column dimensions.");
        }
        std::vector<Matrix> result;
        std::vector<int> arrM;
        for(int i = 0 ; i < m ; i++){
            arrM.push_back(i);
        }
        //m行任取k行
        auto combM = combination(arrM, k);
        std::vector<int> arrN;
        for(int i = 0 ; i < n ; i++){
            arrN.push_back(i);
        }
        //n列任取k列
        auto combN = combination(arrN, k);
        
        for(int i = 0; i< combM.size() ; i++){
            for(int j = 0 ; j < combN.size() ; j++){
                auto cm = combM[i];
                auto cn = combN[j];
                // 行、列
                auto vm = std::vector<std::vector<int>>(k, std::vector<int>(k));
                for(int r = 0 ; r< k ; r++){
                    for(int c = 0 ; c< k; c++){
                        vm[r][c] = a[cm[r]][cn[c]];
                    }
                }
                Matrix ma = vm;
                result.push_back(ma);
            }
        }
        return result;
    }
    // FIXME 计算k阶子式的方法，性能低下---可以线性变换 消元，计算线性独立行的数量
    int Matrix::rank(){
        int r = m<n? m:n;
        while(r>0){
            auto sub = submatrix(r);
            for(auto& matrix:sub){
                if(matrix.det()!=0){
                    return r;
                }
            }
            r--;
        }
        return r;
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
        for(int j = 0 ; j< m ; j++){
            bool pivot = false;
            for(int i = 0 ; i< m ; i++){
                //找到第i 行 第j 列不为0的元素作为主元
                if(!canceled[i] && !pivot && a[i][j]!=0){
                    pivot = true;
                    canceled[i] = 1;
                    //查找最小公倍数
                    std::vector<int> v;
                    for(int k = 0 ; k< m ; k++){
                        if(a[k][j]!=0){
                            v.push_back(a[k][j]);
                        }
                    }
                    int lc = lcm(v);
                    for(int k = 0 ; k< m ; k++){
                        if(a[k][j]!=0){
                            int mul = lc/a[k][j];
                            for(int l = 0 ; l< m; l++){
                                a[k][l] = a[k][l]*mul;
                            }
                            B[k] = B[k]*mul;
                        }
                    }
                    //消元
                    for(int k = 0 ; k< m ; k++){
                        if(k!=i && a[k][j]!=0){
                            //k行减去i行
                            for(int l = 0 ; l< m; l++){
                                a[k][l] -= a[i][l];
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
                if(a[i][j]!=0){
                    //第j个解
                    result[j] = B[i]/a[i][j];
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
        for(int j = 0 ; j< m ; j++){
            bool pivot = false;
            for(int i = 0 ; i< m ; i++){
                //找到第i 行 第j 列不为0的元素作为主元
                if(!canceled[i] && !pivot && a[i][j]!=0){
                    pivot = true;
                    canceled[i] = 1;
                    //查找最小公倍数
                    std::vector<int> v;
                    for(int k = 0 ; k< m ; k++){
                        if(a[k][j]!=0){
                            v.push_back(a[k][j]);
                        }
                    }
                    int lc = lcm(v);
                    for(int k = 0 ; k< m ; k++){
                        if(a[k][j]!=0){
                            int mul = lc/a[k][j];
                            for(int l = 0 ; l< m; l++){
                                a[k][l] = (a[k][l]*mul)%mod;
                            }
                            B[k] = (B[k]*mul)%mod;
                        }
                    }
                    //消元
                    for(int k = 0 ; k< m ; k++){
                        if(k!=i && a[k][j]!=0){
                            //k行减去i行
                            for(int l = 0 ; l< m; l++){
                                a[k][l] = (a[k][l]-a[i][l])%mod;
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
                if(a[i][j]!=0){
                    //第j个解
                    result[j] = (B[i]/a[i][j])%mod;
                }
            }
        }
        for(auto& idx : freeArg){
            result[idx] = 0;
        }
        return result;
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
        Matrix E = Matrix::identity(n);
        for(int i = 0 ; i< n ; i++){
            if(a[i][i]!=0){
                //查找最小公倍数
                std::vector<int> v;
                for(int k = 0 ; k< n ; k++){
                    if(a[k][i]!=0){
                        v.push_back(a[k][i]);
                    }
                }
                int lc = lcm(v);
                for(int k = 0 ; k< n ; k++){
                    if(a[k][i]!=0){
                        int mul = lc/a[k][i];
                        for(int l = 0 ; l< n; l++){
                            a[k][l] = a[k][l]*mul;
                            E.a[k][l] = E.a[k][l]*mul;
                        }
                    }
                }
                //消元
                for(int k = 0 ; k< n ; k++){
                    if(k!=i && a[k][i]!=0){
                        //k行减去i行
                        for(int l = 0 ; l< n; l++){
                            a[k][l] -= a[i][l];
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
                        if(a[p][i]!=0){
                            set = false;
                            //加上p行
                            for(int k = 0 ; k< n ; k++){
                                a[i][k] += a[p][k];
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
            if(a[i][i]!=1){
                int factor = a[i][i];
                for(int j = 0 ; j< n; j++){
                    a[i][j] /= factor;
                    E.a[i][j] /= factor;
                }
            }
        }
        return E;
    }
}
