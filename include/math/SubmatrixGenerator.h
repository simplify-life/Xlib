//
//  SubmatrixGenerator.h
//  com.xm.xlib
//
//  Created by xiaominghe2014@gmail.com on 2023/11/17.
//

#ifndef SubmatrixGenerator_h
#define SubmatrixGenerator_h

#include "math/matrix.h"
#include <vector>

namespace xlib
{
    class SubmatrixGenerator
    {
    public:
        
        SubmatrixGenerator(const Matrix &matrix, int k);

        class iterator
        {
        public:
            iterator(const Matrix &matrix, int k, bool end);
            Matrix operator*() const;
            const iterator &operator++();
            bool operator!=(const iterator &other) const;

        private:
            const Matrix &matrix_;
            int k_;
            bool end_;
            std::vector<std::vector<int>> combM_;
            std::vector<std::vector<int>> combN_;
            std::vector<std::vector<int>>::iterator itM_;
            std::vector<std::vector<int>>::iterator itN_;;
        };

        iterator begin() const;
        iterator end() const;

    private:
        const Matrix &matrix_;
        int k_;
    };
}

#endif /* SubmatrixGenerator_h */
