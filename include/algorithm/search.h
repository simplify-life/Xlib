//
//  search.h
//  com.xm.xlib
//
//  Created by apple on 2023/5/10.
//

#ifndef search_h
#define search_h

#include<vector>

namespace xlib {
    namespace search{
        // unordered_map 可以直接进行hash查找
    
        /**
         * 二分查找
         */
        template<typename T>
        int binarySearch(std::vector<T>& arr, T target) {
            int left = 0, right = arr.size() - 1;
            while (left <= right) {
                int mid = left + (right - left) / 2;
                if (arr[mid] == target) {
                    return mid;
                }
                else if (arr[mid] < target) {
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            }
            return -1;
        }
    
        // 插值查找
        // 适合有序数组
        template<typename T>
        int interpolationSearch(std::vector<T> arr, T x) {
            int lo = 0, hi = (arr.size() - 1);
            while (lo <= hi && x >= arr[lo] && x <= arr[hi]) {
                if (lo == hi) {
                    if (arr[lo] == x) return lo;
                    return -1;
                }
                int pos = lo + (((double)(hi - lo) / (arr[hi] - arr[lo])) * (x - arr[lo]));
                if (arr[pos] == x) return pos;
                if (arr[pos] < x) lo = pos + 1;
                else hi = pos - 1;
            }
            return -1;
        }
    }
}

#endif /* search_h */
