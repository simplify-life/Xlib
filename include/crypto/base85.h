//----------------------------------------------//
//  base85.h
//  xlib
//
//  Copyright (c) __DATA__  Ximena.
//  Created by ximena on 16/6/21.
//  Copyright © 2016年 ximena. All rights reserved.
//  Contact Email: xiaominghe2014@gmail.com
//----------------------------------------------//

#ifndef base85_h
#define base85_h
#include <string>

namespace xlib {
 
    namespace crypto{
        
        std::string encode85(const std::string& in);
        std::string decode85(const std::string& in);
        
    }//! end crypto
    
}//!end xlib



#endif /* base85_h */
