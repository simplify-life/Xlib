//
//  util.h
//  com.xm.xlib
//
//  Created by xiaominghe2014@gmail.com on 2023/6/4.
//

#ifndef util_h
#define util_h

#include <vector>

namespace xlib {
    int reverse_count(const std::vector<int>&);
    //排列
    std::vector<std::vector<int>> permutation(const std::vector<int>&,const int m);
    //组合
    std::vector<std::vector<int>> combination(const std::vector<int>&,const int m);
    //最大公约数
    int gcd(int a,int b);
    int gcd(const std::vector<int>&);
    //最小公倍数
    int lcm(int a,int b);
    int lcm(const std::vector<int>&);

    //多线程并行计算和
    int parallelSum(const std::vector<int>& nums);
}

#endif /* util_h */
