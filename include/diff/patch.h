//
//  patch.h
//  com.xm.xlib
//
//  Created by xiaominghe2014@gmail.com on 2024/5/10.
//

#ifndef patch_h
#define patch_h
#include <iostream>
#include <fstream>
#include <vector>
#include "macro/XBase.h"

namespace xlib {
    namespace patch{
        // 二进制文件差异数据结构
        struct Difference {
            size_t position;
            bool is_deleted;
            uint8 modified;
        };
    
        //Binary Difference Algorithm 二进制差异算法的实现
        std::vector<Difference> binary_difference(const std::string& old_file, const std::string& new_file);
    
        // create patch file from differences 通过差异创建补丁文件
        void create_patch(const std::string& patch_file, const std::vector<Difference>& differences);
        
        // 打补丁
        void apply_patch(const std::string& original_file, const std::string& patch_file, const std::string& output_file);
    }
}

#endif /* patch_h */
