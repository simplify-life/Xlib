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
    

    void dfs_permutation(const std::vector<int>& arr,int m, int n,int low,int hight,std::vector<int>& buf,std::vector<int>& used,std::vector<std::vector<int>>& result){
        if(low==hight){
            std::vector<int> r;
            for(int i = 0 ; i< m ; i++){
                r.push_back(buf.at(i));
            }
            result.push_back(r);
        }else{
            for(int i = 0 ; i< n ; i++){
                if(!used.at(i)){
                    used.at(i) = 1;
                    buf.at(low) = arr.at(i);
                    dfs_permutation(arr,m,n,low+1,hight,buf,used,result);
                    used.at(i) = false;
                }
            }
        }
    }

    std::vector<std::vector<int>> permutation(const std::vector<int>& arr,const int m){
        int n = arr.size();
        int len = n-m;
        std::vector<std::vector<int>> result;
        if(len>=0){
            auto buf = std::vector<int>(m);
            auto used = std::vector<int>(n);
            dfs_permutation(arr, m, n, 0, m, buf, used, result);
        }
        return result;
    }

    void dfs_combination(const std::vector<int>& arr,int m, int n,int low,int hight,std::vector<int>& buf,std::vector<int>& used,std::vector<std::vector<int>>& result,std::vector<std::vector<int>>& first){
        if(low==hight){
            std::vector<int> r;
            for(int i = 0 ; i< m ; i++){
                r.push_back(buf.at(i));
            }
            result.push_back(r);
        }else{
            for(int i = 0 ; i< n ; i++){
                bool next = true;
                for(int l = 0 ; l<m && next; l++){
                    if (l < low) {
                        for (auto& e : first[l]) {
                            if (e == arr[i]) {
                                next = false;
                                break;
                            }
                        }
                    }
                }
                if (!used[i] && next) {
                    used[i] = true;
                    buf[low] = arr[i];
                    first[low].push_back(arr[i]);
                    for (int j = low + 1; j < m; j++) {
                        first[j].resize(0);
                    }
                    dfs_combination(arr,m,n,low + 1, hight,buf,used,result,first);
                    used[i] = false;
                }
            }
        }
    }
    
    std::vector<std::vector<int>> combination(const std::vector<int>& arr,const int m){
        int n = arr.size();
        int len = n-m;
        std::vector<std::vector<int>> result;
        if(len==0){
            result.push_back(arr);
            return result;
        }
        if(len>0){
            auto buf = std::vector<int>(m);
            auto used = std::vector<int>(n);
            auto first = std::vector<std::vector<int>>(m);
            dfs_combination(arr, m, n, 0, m, buf, used, result,first);
        }
        return result;
    }

    int reverse_count(const std::vector<int>& arr){
        if(arr.empty()) return 0;
        int count = 0;
        int len = arr.size();
        for(int i = 1 ; i< len ; i++){
            for(int j = 0 ; j< len ; j++){
                if(j<i){
                    if(arr[j]>arr[i]){
                        count++;
                    }
                }else break;
            }
        }
        return count;
    }

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
                mul *=a[i][pm[i]];
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
}
