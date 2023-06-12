//
//  util.cpp
//  com.xm.xlib
//
//  Created by xiaominghe2014@gmail.com on 2023/6/4.
//

#include "math/math.h"

namespace xlib {

    void dfs_permutation(std::vector<int>& arr, int m, int low, std::vector<std::vector<int>>& result) {
        // 如果已经选择了 m 个元素，则将排列添加到结果中
        if (low == m) {
            result.push_back(std::vector<int>(arr.begin(), arr.begin() + m));
            return;
        }

        // 遍历剩余的元素并与当前元素交换
        for (int i = low; i < arr.size(); i++) {
            std::swap(arr[low], arr[i]);
            dfs_permutation(arr, m, low + 1, result);
            std::swap(arr[low], arr[i]);
        }
    }

    std::vector<std::vector<int>> permutation(const std::vector<int>& arr,const int m){
        int n = arr.size();
        int len = n - m;
        std::vector<std::vector<int>> result;
        if (len >= 0) {
            auto buf = std::vector<int>(arr);
            dfs_permutation(buf, m, 0, result);
        }
        return result;
    }

    void dfs_combination(const std::vector<int>& arr, int m, int n, int low, std::vector<int>& buf, std::vector<bool>& used, std::vector<std::vector<int>>& result) {
        // 当 low 等于 m 时，表示已经找到一个组合，将其添加到结果集中
        if (low == m) {
            result.push_back(buf);
            return;
        }

        // 从上一个元素的下一个位置开始遍历，避免重复
        for (int i = (low == 0 ? 0 : buf[low - 1] + 1); i < n; i++) {
            // 如果当前元素没有被使用过
            if (!used[i]) {
                // 标记为已使用
                used[i] = true;
                // 将当前元素添加到 buf 中
                buf[low] = arr[i];
                // 递归调用，继续寻找下一个元素
                dfs_combination(arr, m, n, low + 1, buf, used, result);
                // 回溯，将当前元素标记为未使用
                used[i] = false;
            }
        }
    }

    std::vector<std::vector<int>> combination(const std::vector<int>& arr,const int m){
        int n = arr.size();
        int len = n - m;
        std::vector<std::vector<int>> result;
        // 当 len 等于 0 时，表示只有一个组合，直接将 arr 添加到结果集中
        if (len == 0) {
            result.push_back(arr);
            return result;
        }
        // 当 len 大于 0 时，进行组合计算
        if (len > 0) {
            // 初始化 buf 和 used 变量
            auto buf = std::vector<int>(m);
            auto used = std::vector<bool>(n);
            // 调用优化后的 dfs_combination 函数
            dfs_combination(arr, m, n, 0, buf, used, result);
        }
        // 返回结果集
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
