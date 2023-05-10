//
//  sort.h
//  com.xm.xlib
//
//  Created by apple on 2023/5/10.
//

#ifndef sort_h
#define sort_h

#include <vector>
#include <algorithm>

namespace xlib {
    namespace sort{


        //冒泡排序
        template<typename T>
        void bubbleSort(std::vector<T>& arr) {
            int n = arr.size(); // 获取数组长度
            for (int i = 0; i < n - 1; i++) {
                for (int j = 0; j < n - i - 1; j++) {
                    if (arr[j] > arr[j + 1]) {
                        T temp = arr[j];
                        arr[j] = arr[j + 1];
                        arr[j + 1] = temp;
                    }
                }
            }
        }
    
        // 选择排序
        template<typename T>
        void selectionSort(std::vector<T>& arr) {
            int n = arr.size();
            for (int i = 0; i < n - 1; i++) {
                int min_index = i;
                for (int j = i + 1; j < n; j++) {
                    if (arr[j] < arr[min_index]) {
                        min_index = j;
                    }
                }
                if (min_index != i) {
                    std::swap(arr[i], arr[min_index]);
                }
            }
        }

        // 插入排序
        template<typename T>
        void insertionSort(std::vector<T>& arr) {
            int n = arr.size();
            for (int i = 1; i < n; i++) {
                T key = arr[i];
                int j = i - 1;
                while (j >= 0 && arr[j] > key) {
                    arr[j + 1] = arr[j];
                    j--;
                }
                arr[j + 1] = key;
            }
        }

        // 快速排序
        template<typename T>
        void quickSort(std::vector<T>& arr, int left, int right) {
            if (left < right) {
                int pivot_index = left + (right - left) / 2;
                T pivot = arr[pivot_index];
                int i = left;
                int j = right;
                while (i <= j) {
                    while (arr[i] < pivot) {
                        i++;
                    }
                    while (arr[j] > pivot) {
                        j--;
                    }
                    if (i <= j) {
                        std::swap(arr[i], arr[j]);
                        i++;
                        j--;
                    }
                }
                quickSort(arr, left, j);
                quickSort(arr, i, right);
            }
        }

        // 归并排序
        template<typename T>
        void mergeSort(std::vector<T>& arr, int left, int right) {
            if (left < right) {
                int mid = left + (right - left) / 2;
                mergeSort(arr, left, mid);
                mergeSort(arr, mid + 1, right);
                std::vector<T> temp(right - left + 1);
                int i = left;
                int j = mid + 1;
                int k = 0;
                while (i <= mid && j <= right) {
                    if (arr[i] < arr[j]) {
                        temp[k++] = arr[i++];
                    } else {
                        temp[k++] = arr[j++];
                    }
                }
                while (i <= mid) {
                    temp[k++] = arr[i++];
                }
                while (j <= right) {
                    temp[k++] = arr[j++];
                }
                for (int p = 0; p < k; p++) {
                    arr[left + p] = temp[p];
                }
            }
        }
    
        // 希尔排序
        template<typename T>
        void shellSort(std::vector<T>& arr) {
            int n = arr.size();
            for (int gap = n / 2; gap > 0; gap /= 2) {
                for (int i = gap; i < n; i++) {
                    T temp = arr[i];
                    int j = i;
                    while (j >= gap && arr[j - gap] > temp) {
                        arr[j] = arr[j - gap];
                        j -= gap;
                    }
                    arr[j] = temp;
                }
            }
        }

        // 堆排序
        template<typename T>
        void heapSort(std::vector<T>& arr) {
            std::make_heap(arr.begin(), arr.end());
            std::sort_heap(arr.begin(), arr.end());
        }

        // 计数排序
        template<typename T>
        void countingSort(std::vector<T>& arr) {
            T max_val = *std::max_element(arr.begin(), arr.end());
            std::vector<int> count(max_val + 1);
            for (T val : arr) {
                count[val]++;
            }
            int i = 0;
            for (int val = 0; val <= max_val; val++) {
                for (int j = 0; j < count[val]; j++) {
                    arr[i++] = val;
                }
            }
        }

        // 桶排序
        template<typename T>
        void bucketSort(std::vector<T>& arr) {
            T max_val = *std::max_element(arr.begin(), arr.end());
            int num_buckets = std::min(static_cast<int>(arr.size()), static_cast<int>(max_val) + 1);
            std::vector<std::vector<T>> buckets(num_buckets);
            for (T val : arr) {
                int bucket_index = static_cast<int>(val * num_buckets / (max_val + 1));
                buckets[bucket_index].push_back(val);
            }
            int i = 0;
            for (auto& bucket : buckets) {
                std::sort(bucket.begin(), bucket.end());
                for (T val : bucket) {
                    arr[i++] = val;
                }
            }
        }

        // 基数排序
        template<typename T>
        void radixSort(std::vector<T>& arr) {
            T max_val = *std::max_element(arr.begin(), arr.end());
            for (T exp = 1; max_val / exp > 0; exp *= 10) {
                std::vector<T> output(arr.size());
                std::vector<int> count(10);
                for (T val : arr) {
                    int digit = static_cast<int>((val / exp) % 10);
                    count[digit]++;
                }
                for (int i = 1; i < 10; i++) {
                    count[i] += count[i - 1];
                }
                for (int i = arr.size() - 1; i >= 0; i--) {
                    T val = arr[i];
                    int digit = static_cast<int>((val / exp) % 10);
                    output[count[digit] - 1] = val;
                    count[digit]--;
                }
                for (int i = 0; i < arr.size(); i++) {
                    arr[i] = output[i];
                }
            }
        }
    }
}

#endif /* sort_h */
