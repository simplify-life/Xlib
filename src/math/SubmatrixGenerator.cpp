//
//  SubmatrixGenerator.cpp
//  com.xm.xlib
//
//  Created by apple on 2023/11/17.
//

#include "math/SubmatrixGenerator.h"
#include "math/util.h"
#include <numeric>

namespace xlib
{
    SubmatrixGenerator::SubmatrixGenerator(const Matrix &matrix, int k) : matrix_(matrix), k_(k)
    {
    }

    SubmatrixGenerator::iterator::iterator(const Matrix &matrix, int k, bool end) : matrix_(matrix), k_(k), end_(end)
    {
        // 初始化迭代器状态
        std::vector<int> arrM(matrix.m);
        std::iota(arrM.begin(), arrM.end(), 0);
        std::vector<int> arrN(matrix.n);
        std::iota(arrN.begin(), arrN.end(), 0);

        // m行任取k行
        combM_ = combination(arrM, k);
        // n列任取k列
        combN_ = combination(arrN, k);
        itM_ = combM_.begin();
        itN_ = combN_.begin();
    }

    Matrix SubmatrixGenerator::iterator::operator*() const
    {
        auto cm = *itM_;
        auto cn = *itN_;
        // 生成当前子矩阵
        Matrix submatrix(k_, k_);
        // ...填充子矩阵
        for (int r = 0; r < k_; r++)
        {
            for (int c = 0; c < k_; c++)
            {
                submatrix.a[r][c] = matrix_.a[cm[r]][cn[c]];
            }
        }
        return submatrix;
    }

    const SubmatrixGenerator::iterator &SubmatrixGenerator::iterator::operator++()
    {
        // 更新迭代器状态以生成下一个子矩阵
        // 如果没有更多的子矩阵，设置end_为true
        ++itN_;
        if(itN_ == combN_.end()){
            itN_ = combN_.begin();
            ++itM_;
            if(itM_ == combM_.end()){
                end_ = true;
            }
        }
        return *this;
    }

    bool SubmatrixGenerator::iterator::operator!=(const iterator &other) const
    {
        return end_ != other.end_;
    }

    SubmatrixGenerator::iterator SubmatrixGenerator::begin() const
    {
        return iterator(matrix_, k_, false);
    }

    SubmatrixGenerator::iterator SubmatrixGenerator::end() const
    {
        return iterator(matrix_, k_, true);
    }
}
