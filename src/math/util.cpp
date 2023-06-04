//
//  util.cpp
//  com.xm.xlib
//
//  Created by apple on 2023/6/4.
//

#include "math/math.h"

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

    int gcd(int a,int b){
        if (b == 0) return a;
        return gcd(b, a % b);
    }

    int gcd(const std::vector<int>& nums){
        int n = nums.size();
        if(n==0) return 0;
        int max_gcd = nums[0];
        for (int i = 1; i < n; i++) {
            max_gcd = gcd(max_gcd, nums[i]);
        }
        return max_gcd;
    }

    int lcm(int a,int b){
        if(0==a || b==0) return 0;
        a = a<0? -a:a;
        b = b<0? -b:b;
        return a*b / gcd(a, b);
    }

    int lcm(const std::vector<int>& nums){
        int n = nums.size();
        if(n==0){
            return -1;
        }
        int result = nums[0]<0? -nums[0]:nums[0];
        for (int i = 0; i < n; i++) {
            result = lcm(result, nums[i]);
        }
        return result;
    }
}
